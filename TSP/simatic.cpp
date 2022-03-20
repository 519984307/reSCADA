#include "simatic.h"
#include "snap7.h"
#include <qdebug.h>
//------------------------------------------------------------------------------
SimaticDriver::SimaticDriver(int id, QString name, QString address, int rack, int slot, QString comment)
{
    this->type = "simatic";
    this->id = id;
    this->setObjectName(name);
    this->address = address;
    this->rack = rack;
    this->slot = slot;
    this->comment = comment;
    initThread();
}
//------------------------------------------------------------------------------
SimaticDriver::~SimaticDriver()
{
    disconnect();

    foreach (Task * task, listOfTasks){
        delete task;
    }
    delete client;
    delete taskTimer;
}
//------------------------------------------------------------------------------
//Выделяет драйверу отдельный поток
void SimaticDriver::initThread()
{
    thread = new QThread;
    thread->start();
    this->moveToThread(thread);
    taskTimer = new QTimer();
    taskTimer->moveToThread(thread);
    client = new TS7Client();
    client->moveToThread(thread);
    QObject::connect(this, &Driver::s_connectDriver, this, &Driver::connect, Qt::QueuedConnection);
    QObject::connect(this, &Driver::s_disconnectDriver, this, &Driver::disconnect, Qt::QueuedConnection);
}
//------------------------------------------------------------------------------
//Проверяет соединение с ПЛК, если его нет - соединяет
void SimaticDriver::connect()
{
    if( !client ) {
        noError = false;
        emit s_logging(MessError, QDateTime::currentDateTime(),
            false, this->objectName(),
            "driver is null" );
        return;
    }

    if( !client->Connected() || !noError ){
        char charAddress[ address.size() ];
        memcpy( &charAddress, address.toStdString().c_str(), address.size() );
        charAddress[ address.size() ] = '\0';

        // Запрос на соединение
        int res = client->ConnectTo(charAddress, rack, slot);

        if( res == 0 ) {//Успешно соединился
            emit s_logging(MessInfo, QDateTime::currentDateTime(), false,
                this->objectName(), "Simatic driver connected");
            noError = true;
            PDULen = client->PDULength() - 64;
        }
        else {//Нет соединения. Сообщение об ошибке в лог.
            noError = false;
            if( errorCode != res )emit s_logging(MessError, QDateTime::currentDateTime(),
                    false, this->objectName(),
                    std::string("Simatic driver connection error: "
                        + ( res < 0 ? "libruary error" : CliErrorText(res)) ).c_str() );
            errorCode = res;
        }
        if (!started){
            started = true;
            emit s_onStartedChanged();
            scheduleHandler();
        }
    }
}
//------------------------------------------------------------------------------
void SimaticDriver::disconnect()
{
    if(!client) return;
    started = false;
    client->Disconnect();
    taskTimer->stop();
    emit s_logging(MessInfo, QDateTime::currentDateTime(), false, this->objectName(), "Simatic driver disconnected");
    foreach(Group * group, listOfGroups){
        qualityFiller(group->listOfTags, Bad);
        group->update();
    }
    emit s_onStartedChanged();
}

//------------------------------------------------------------------------------
void SimaticDriver::handleNextTask()
{
    if( started ){
        cycleDone = true;
        if( client->Connected() ){
            if (listOfTasks.count() > 0){

                if (listOfTasks.last()->writeTask){
                    Task * task = listOfTasks.takeLast();
                    write(task);
                    delete task;
                }
                else{
                    //                    Task * task = listOfTasks.takeFirst();
                    //                    read(task);
                    //                    delete task;
                    readInList();
                    scheduleHandler();
                }
            }
            else{
                createReadTasks();
            }
        }
        else{
            foreach (Group * group, listOfGroups) {//Перебор групп.
                errorFiller(group->listOfTags, "driver read error: driver isn\'t connected");
                qualityFiller(group->listOfTags, Bad);
                group->update();
            }
            connect();
        }
        scheduleHandler();
    }
}
//------------------------------------------------------------------------------
inline int amtFromType( SimAddress *lastTagAdr, SimaticDriver::Task *task )
{
    if( lastTagAdr->memArea == S7AreaDB ){//Т.к. из BD читаем только байтами нужно добавить биты от типов данных длиннее байта
        return  /*(*/(lastTagAdr->regAddr.memSlot - task->regAddr.memSlot)
            + dataSizeByte(lastTagAdr->type)/* + 1) / 2*/;// +1 чтобы не округлило в меньшую сторону, /2 т.к. в word 2 byte
    }
    //Запрашивать отдельные биты группами нельзя поэтому убрал этот кусок
    //  else if( task->listOfTags.count() == 1 && task->type == S7WLBit && task->memArea == S7AreaMK){
    //    return 1 + (lastTagAdr->regAddr.memSlot - task->regAddr.memSlot ) * 8 + lastTagAdr->regAddr.bit - task->regAddr.bit;
    //  }
    else{
        return (1 + lastTagAdr->regAddr.memSlot - task->regAddr.memSlot) /*/ 2*/;// +1 т.к. включая начальный элемент и +1 чтобы не округлило в меньшую сторону. Итого +2.
    }
}
//------------------------------------------------------------------------------
//Разбивает теги группы на задачи(task) с тэгами у которых адреса идут один за другим.
inline void SimaticDriver::createReadTasks()
{
    SimAddress * address;
    Task * task;//Текущая задача.
    foreach (Group * group, listOfGroups) {//Перебор групп.
        //Если задержка группы на опрос прошла.
        if (group->lastUpdate.msecsTo(QDateTime::currentDateTime()) > group->delay
            && group->listOfTags.count() > 0 ){
            //Перебор тэгов в группе, для разделения на задачи.
            task = nullptr;

            for (int i = 0; i < group->listOfTags.count(); i++){
                //Пулучение адреса тэга з приепленной к нему структуры.
                address = static_cast<SimAddress*>( group->listOfTags.at(i)->speshData );
                //Пропуск негодных или заккрытых для чтения тэгов
                if( address->regAddr.memSlot == -1
                    || group->listOfTags.at(i)->access == WO
                    || group->listOfTags.at(i)->access == NA ) continue;

                if ( !task ){//Если формируется новая задача...
                    task = new Task;
                    task->memArea = address->memArea;
                    task->DBNumb = address->DBNumb;
                    task->type = task->memArea == S7AreaDB ? S7WLByte : address->type;
                    task->regAddr = address->regAddr;
                    task->groupId = group->id;
                    task->listOfTags.append( group->listOfTags.at(i) );
                    listOfTasks.append(task);
                }
                else{//Добавление новых тегов в текущую задачу.
                    if( address->DBNumb == task->DBNumb // DB совпадает
                        && address->memArea == task->memArea// и обл. памяти совпадает
                        && (task->memArea == S7AreaDB ? true : address->type == task->type)// и тип совпадает
                        && group->listOfTags.at(i)->getQuality() == Good//и тег уже успешно опрашивался
                        && task->regAddr.memSlot + group->optimRangeMax >= address->regAddr.memSlot//и адрес не очень далеко от 1-го тэга в задаче.
                        && amtFromType(address,task ) <= 32 )
                    {
                        task->listOfTags.append(group->listOfTags.at(i));//Добавление тэга в задачу.
                    }
                    else{//иначе завершаем группу
                        i--;//Шаг назад, чтобы начать с текущего тэга новую задачу.
                        if(task->listOfTags.count()>1 && task->type == S7WLBit ){
                            task->type = S7WLByte;
                        }
                        task = nullptr;
                    }
                }
            }
            if(group->listOfTags.count()){
                group->update();//TODO Переделать. Не уместно: группа обновляет данные перед новым запросом
            }
        }
    }
}
//------------------------------------------------------------------------------
void SimaticDriver::createWriteTask(Tag *tag, QVariant NewValue)
{
    SimAddress * address;
    address = static_cast<SimAddress*>( tag->speshData );
    //Пропуск негодных или заккрытых для записи тэгов
    if( address->regAddr.memSlot == -1
        || tag->access == RO
        || tag->access == NA ) return;

    writeTag(tag,NewValue);
    //    Task * wTask = new Task;
    //    wTask->writeTask = true;
    //    wTask->memArea = address->memArea;
    //    wTask->DBNumb = address->DBNumb;
    //    wTask->type = wTask->memArea == S7AreaDB ? S7WLByte : address->type;//address->type;
    //    wTask->regAddr = address->regAddr;
    //    wTask->listOfTags.append( tag );
    //    listOfTasks.append(wTask);
    //    Task * rTask = new Task;
    //    rTask->writeTask = false;
    //    rTask->memArea = address->memArea;
    //    rTask->DBNumb = address->DBNumb;
    //    rTask->type = rTask->memArea == S7AreaDB ? S7WLByte : address->type;//address->type;
    //    rTask->regAddr = address->regAddr;
    //    rTask->listOfTags.append( tag );
    //    listOfTasks.append(rTask);
}
//------------------------------------------------------------------------------
inline int toBitAdr( SimAddress *SA )
{
    return SA->regAddr.memSlot * 8 + SA->regAddr.bit;
}

//NOTE просто захотелось побаловаться с #define))) Ни разу не пробовал.
#define offsetBuff (tagAdr->regAddr.memSlot - task->regAddr.memSlot) * sizeKf
//------------------------------------------------------------------------------
void SimaticDriver::read( SimaticDriver::Task *task )
{
    if(!client) {
        errorFiller(task->listOfTags, "driver read error: driver is null");
        qualityFiller(task->listOfTags, Bad);
        //emit LoggingSystem(MessError, QDateTime::currentDateTime(), this->objectName(), "Simatic driver read error: driver is null");
        return;
    }
    if(client->Connected()){
        SimAddress * lastTagAdr = static_cast<SimAddress*>(task->listOfTags.last()->speshData);//Получение адреса тэга
        int_fast8_t sizeKf = dataSizeByte( task->type );//Вычисление поправ-го коэф-та в зависимости от типа
        int amt = amtFromType(lastTagAdr,task );//Вычисление кол-ва единиц на считывание
        byte data[amt * sizeKf];//Выделение буфера чтения
        //Запрос на чтение
        int res = client->ReadArea(task->memArea, task->DBNumb,
            task->type == S7WLBit ? toBitAdr( task )
                                  : task->regAddr.memSlot,
            amt, task->type, data);
        if(res == 0){
            //Запись результата в тэги
            foreach (Tag *tag, task->listOfTags) {
                SimAddress * tagAdr = static_cast<SimAddress*>(tag->speshData);

                switch (tagAdr->type) {
                case S7WLBit:
                    if(task->type == S7WLBit){
                        tag->setValue( GetBitAt( data, offsetBuff, 0 ) );
                    }
                    else{
                        tag->setValue( GetBitAt( data, offsetBuff,
                            tagAdr->regAddr.bit ) );
                    }
                    break;
                case S7WLByte:
                    tag->setValue( GetByteAt( data,offsetBuff) );
                    break;
                case S7WLWord:
                    tag->setValue( GetIntAt( data, offsetBuff ) );
                    break;
                case S7WLCounter:
                case S7WLTimer:
                    tag->setValue( GetWordAt( data, offsetBuff ) );
                    break;
                case S7WLDWord:
                    if( tag->type == TFloat ) tag->setValue( GetRealAt( data, offsetBuff ));
                    else tag->setValue( GetDIntAt( data, offsetBuff ) );
                    break;
                    //Такого варианта не будет, т.к. он кодируется в тегах просто как Double Word
                    //          case S7WLReal:
                    //            tag->setValue( (double)GetRealAt( data, tagAdr->regAddr.memSlot * kf ) );
                    //            break;
                default:
                    break;
                }
            }
            errorFiller(task->listOfTags, "");
            qualityFiller(task->listOfTags, Good);
        }

        if( res != 0 ){
            if (res == 589856)
                noError = false;
            errorFiller(task->listOfTags, "driver read error: " + QString(CliErrorText(res).c_str()));
            qualityFiller(task->listOfTags, Bad);
        }
    }
}
//------------------------------------------------------------------------------
#define offsetBuff2 (tagAdr->regAddr.memSlot - tmpTask->regAddr.memSlot) * sizeKf
void SimaticDriver::readInList(int FstTaskInd,  QList<Task*> *TaskList)
{
    if( TaskList == nullptr){
        TaskList = &listOfTasks;
    }
    if(!client) {
        for (int i = FstTaskInd; i < TaskList->count(); i++ ) {
            errorFiller(TaskList->at(i)->listOfTags, "driver read error: driver is null");
            qualityFiller(TaskList->at(i)->listOfTags, Bad);
        }
        return;
    }
    if(client->Connected()){
        byte buf[20][500];
        int PDUsumm = 0;
        TS7DataItem Items[20];
        SimAddress * lastTagAdr;//Получение адреса тэга
        int_fast8_t sizeKf;//Вычисление поправ-го коэф-та в зависимости от типа
        int amt;//Вычисление кол-ва единиц на считывание
        // byte *data;//Выделение буфера чтения
        int lastTaskInd;
        lastTaskInd = FstTaskInd;
        Task *tmpTask;
        //Перебор задач и формирование из них Item для запроса. Если задача на запись,
        //останавливаем перебор.

        int iN = 0;
        while ( lastTaskInd < TaskList->count()
            && iN < 12 //Можно и 20, но я пока на примере WinCC видел только 12
            && !TaskList->at(lastTaskInd)->writeTask ) {//Перебор задач
            tmpTask = TaskList->at(lastTaskInd);
            sizeKf = dataSizeByte( tmpTask->type );//Вычисление поправ-го коэф-та в зависимости от типа

            //Если задача запрашивает больше чем влазит в PDU Lenght, то запрашиваем
            //функцией read() она умеет дробить запросы и возвращаем номер этой
            //задачи как последней прочитанной
            lastTagAdr = static_cast<SimAddress*>(tmpTask->listOfTags.last()->speshData);//Получение адреса тэга
            amt = amtFromType(lastTagAdr,tmpTask );//Вычисление кол-ва единиц на считывание
            PDUsumm += amt * sizeKf;

            if( PDUsumm > PDULen ){
                if( iN == 0 && amt * sizeKf > PDULen ){
                    read( tmpTask );
                    delete tmpTask;
                    return;
                }
                break;
            }
            //Проверка на превышение PDU Lenght суммой буферов задач в запросе

            Items[iN].Area = tmpTask->memArea;
            Items[iN].WordLen = tmpTask->type;
            Items[iN].Result = 0;
            Items[iN].DBNumber = tmpTask->DBNumb;
            Items[iN].Start = tmpTask->type == S7WLBit ? toBitAdr( tmpTask ) : tmpTask->regAddr.memSlot;
            Items[iN].Amount = amt;// -1 т.к. если пприбавить к адресу начала считывания колличество нужных байт,то получит конечную ячейку на 1 дальше необходимой
            Items[iN].pdata = buf[iN];//byte[amt * sizeKf * 2];
            lastTaskInd++;
            iN++;
        }


        if(iN > 0){
            int res = client->ReadMultiVars( &Items[0], iN );

            if(res == 0){

                for ( int i = 0 ;i < iN; i++) {
                    tmpTask = TaskList->takeAt(FstTaskInd);//Задача соответ-я Item. Изымается из списка!!!
                    sizeKf = dataSizeByte( tmpTask->type );//TODO возможно лучше поле в структуре сделать, а не вычислять. Вычисление поправ-го коэф-та в зависимости от типа

                    if( Items[i].Result == 0){

                        foreach (Tag *tag, tmpTask->listOfTags) {//Запись результата в тэги
                            SimAddress * tagAdr;
                            tagAdr = static_cast<SimAddress*>(tag->speshData);

                            switch (tagAdr->type) {
                            case S7WLBit:
                                if(tmpTask->type == S7WLBit){
                                    tag->setValue( GetBitAt( Items[i].pdata, offsetBuff2, 0 ) );
                                }
                                else{
                                    tag->setValue( GetBitAt( Items[i].pdata, offsetBuff2,
                                        tagAdr->regAddr.bit ) );
                                }
                                break;
                            case S7WLByte:
                                tag->setValue( GetByteAt( Items[i].pdata,offsetBuff2) );
                                break;
                            case S7WLWord:
                                tag->setValue( GetIntAt( Items[i].pdata, offsetBuff2 ) );
                                break;
                            case S7WLCounter:
                            case S7WLTimer:
                                tag->setValue( GetWordAt( Items[i].pdata, offsetBuff2 ) );
                                break;
                            case S7WLDWord:
                                if( tag->type == TFloat ) tag->setValue( GetRealAt( Items[i].pdata, (tagAdr->regAddr.memSlot - tmpTask->regAddr.memSlot) * sizeKf ));
                                else tag->setValue( GetDIntAt( Items[i].pdata, offsetBuff2 ) );
                                break;
                                //Такого варианта не будет, т.к. он кодируется в тегах просто как Double Word
                                //          case S7WLReal:
                                //            tag->setValue( (double)GetRealAt( data, tagAdr->regAddr.memSlot * kf ) );
                                //            break;
                            default:
                                break;
                            }
                            tag->setError("");
                            tag->setQuality(Good);
                        }
                    }
                    else{
                        errorFiller(tmpTask->listOfTags, "Item address read error: " + QString(CliErrorText(res).c_str()));
                        qualityFiller(tmpTask->listOfTags, Check);
                    }

                    //delete [] static_cast<byte*>(Items[i].pdata);//Зачищаю память под данные, т.к. она выделялась чрез new
                    delete tmpTask;
                }
            }
            else{
                if (res == 589856){
                    noError = false;
                }
                iN--;
                while ( iN >= 0) {
                    tmpTask = TaskList->takeAt(FstTaskInd);//Задача соответ-я Item
                    errorFiller(tmpTask->listOfTags, "Read error: " + QString(CliErrorText(res).c_str()));
                    qualityFiller(tmpTask->listOfTags, Bad);
                    //delete [] static_cast<byte*>(Items[iN].pdata);
                    delete tmpTask;
                    iN--;
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
#define offsetBuffwrite (tagAdr->regAddr.memSlot - task->regAddr.memSlot) * sizeKf
void SimaticDriver::writeTag(Tag *Tag, QVariant NewValue )
{
    if(!client) {
        Tag->setError("driver read error: driver is null");
        Tag->setQuality(Bad);
        //emit LoggingSystem(MessError, QDateTime::currentDateTime(), this->objectName(), "Simatic driver read error: driver is null");
        return;
    }
    if (started) connect();
    if(client->Connected()){
        SimAddress *tagAdr = static_cast<SimAddress*>(Tag->speshData);//Получение адреса тэга
        int amt = dataSizeByte(tagAdr->type);//Вычисление кол-ва единиц на считывание
        byte data[amt];
        for (int i = 0; i < amt; i++) {
            data[i] = 0;
        }
        amt = 1;
        //Запись значений тэгов в буфер для отправки
        int res = 0;

        switch (tagAdr->type) {
        case S7WLBit:
            data[0] = NewValue.toBool();
            break;
        case S7WLWord:
            SetIntAt( data, 0, NewValue.toInt() );
            break;
        case S7WLByte:
        case S7WLCounter:
        case S7WLTimer:
            SetWordAt( data, 0, NewValue.toUInt() );
            break;
        case S7WLDWord:
            //amt = 2;
            if( Tag->type == TFloat )
                SetRealAt( data, 0,  NewValue.toFloat() );
            else
                SetDIntAt( data, 0,  NewValue.toUInt() );
            break;
        default:
            break;
        }

        if(res == 0){//Проверка, что в случае S7WLBit запрос байта со значениями рядомстоящих битов успешен.
            res = client->WriteArea(tagAdr->memArea, tagAdr->DBNumb,
                tagAdr->type == S7WLBit ? toBitAdr( tagAdr )
                                        : tagAdr->regAddr.memSlot,
                amt, tagAdr->type, data);
        }
        if(res == 0){
            Tag->setError("");
            Tag->setQuality(Good);
            Tag->setValue(NewValue);
            Tag->ready = true;
        }
        if( res != 0 ){
            if (res == 589856)
                noError = false;
            Tag->setError("driver write error: " + QString(CliErrorText(res).c_str()));
            Tag->setQuality(Bad);
        }
    }
    else{
        Tag->setError("driver write error: driver isn\'t connected");
        Tag->setQuality(Bad);
    }
}

//------------------------------------------------------------------------------
void SimaticDriver::write(SimaticDriver::Task *task )
{
    if(!client) {
        errorFiller(task->listOfTags, "driver read error: driver is null"); qualityFiller(task->listOfTags, Bad);
        //emit LoggingSystem(MessError, QDateTime::currentDateTime(), this->objectName(), "Simatic driver read error: driver is null");
        return;
    }
    if (started) connect();
    if(client->Connected()){
        SimAddress * lastTagAdr = static_cast<SimAddress*>(task->listOfTags.last()->speshData);//Получение адреса тэга
        int_fast8_t sizeKf = dataSizeByte( task->type );//Вычисление поправ-го коэф-та в зависимости от типа
        int amt = amtFromType(lastTagAdr,task);//Вычисление кол-ва единиц на считывание
        byte data[amt * sizeKf];
        for (int i = 0; i < (amt * sizeKf); i++) {
            data[i] = 0;
        }
        //Запись значений тэгов в буфер для отправки
        int res = 0;

        for (int i = 0; i < task->listOfTags.count(); i++ ) {
            SimAddress * tagAdr = static_cast<SimAddress*>(task->listOfTags.at(i)->speshData);
            //Смещение внутри буфера записи

            switch (tagAdr->type) {
            case S7WLBit:
                if(task->listOfTags.count() == 1 ){
                    data[0] = task->listOfTags.at(i)->newValue.toBool();
                }
                else{
                    //!NOTE не тестировалось!!!
                    byte tmp = 0;
                    data[offsetBuff] = task->listOfTags.at(i)->newValue.toBool();
                    if( task->memArea == S7AreaDB ){//Чтобы не обнулить рядомстоящие биты запрос их актуального состояния.
                        res = client->ReadArea(task->memArea, task->DBNumb,
                        tagAdr->regAddr.memSlot, 1, task->type, &tmp);
                        data[offsetBuff] |= tmp; //считанные биты вставляем в байт с записываемым битом
                    }
                    SetBitAt( data, offsetBuff, tagAdr->regAddr.bit, task->listOfTags.at(i)->newValue.toBool() );
                }
                break;
            case S7WLWord:
                SetIntAt( data, offsetBuff, task->listOfTags.at(i)->newValue.toInt() );
                break;
            case S7WLByte:
            case S7WLCounter:
            case S7WLTimer:
                SetWordAt( data, offsetBuff, task->listOfTags.at(i)->readValue().toUInt() );
                break;
            case S7WLDWord:
                if( task->listOfTags.at(i)->type == TFloat )
                    SetRealAt( data, offsetBuff, task->listOfTags.at(i)->newValue.toFloat() );
                else
                    SetDIntAt( data, offsetBuff, task->listOfTags.at(i)->newValue.toUInt() );
                break;
            default:
                break;
            }
        }
        if(res == 0){//Проверка, что в случае S7WLBit запрос байта со значениями рядомстоящих битов успешен.
            res = client->WriteArea(task->memArea, task->DBNumb,
                task->type == S7WLBit ? toBitAdr( task )
                                      : task->regAddr.memSlot,
                amt, task->type, data);
        }
        if(res == 0){
            readyFiller(task->listOfTags, true);
            newValueFiller(task->listOfTags);
            errorFiller(task->listOfTags, "");
            qualityFiller(task->listOfTags, Good);
        }
        if( res != 0 ){
            if (res == 589856)
                noError = false;
            errorFiller(task->listOfTags, "driver write error: " + QString(CliErrorText(res).c_str()));
            qualityFiller(task->listOfTags, Bad);
        }
    }
    else{
        errorFiller(task->listOfTags, "driver write error: driver isn\'t connected");
        qualityFiller(task->listOfTags, Bad);
    }
}

//------------------------------------------------------------------------------
void SimaticDriver::scheduleHandler()
{
    if (started && cycleDone){
        cycleDone = false;
        taskTimer->singleShot(1, this, &SimaticDriver::handleNextTask);
    }
}

//------------------------------------------------------------------------------
void SimaticDriver::sortTags(QList<Tag *> &listOfTags)
{
    foreach (Tag * tag, listOfTags) {
        if( tag->speshData == nullptr ){//Если тэгу еще не прицепили SimAdress
            tag->speshData = new SimAddress;
        }
        //Если SimAdress пустой, то заполняем его
        if( static_cast<SimAddress*>(tag->speshData)->regAddr.memSlot == -1 ){
            SimAddress sa;
            if( strToAddr(tag->address, &sa ) ){
                *(static_cast<SimAddress*>(tag->speshData)) = sa;//!Проверить!!
            }
            else{
                tag->setQuality( NotValid );
            }
        }
    }
    if( listOfTags.count() < 2 ) return; //Сортировка не нужна.
    bool ready {false};
    while( !ready ){ //bubble sorting
        ready = true;
        for( int i = 0; i < listOfTags.count() - 1; i++ ){
            if( *static_cast<SimAddress*>(listOfTags.at(i)->speshData) > *static_cast<SimAddress*>(listOfTags.at(i + 1)->speshData) ){
                listOfTags.swapItemsAt( i, i+1 );
                ready = false;
            }
        }
    }
}
//------------------------------------------------------------------------------
//TODO переделать,чтоб возвращала Simadress, а не bool
bool SimaticDriver::strToAddr(QString str, SimAddress *address)
{
    if(!address) return false;
    if(str.count() < 2 || str == "") return false;
    str = str.toUpper();
    //Удаление % и P# в начале строки
    if( str[0] == '%' ) str.remove(0,1);
    else if( str.left(2) == "P#" )str.remove(0,2);

    bool ok = false;

    if (str[0] == "M"){//Меркерная память M
        address->memArea = S7AreaMK;
    }
    else if(str[0] == "I"){//Диск. входы I
        address->memArea = S7AreaPE;
    }
    else if(str[0] == "Q"){//Диск. выходы Q
        address->memArea = S7AreaPA;
    }
    else if(str.left(2)== "DB"){//DB
        address->memArea = S7AreaDB;
        str.remove(0, 1);//Удаление прочитанного символа, т.к. их 2
    }
    else if(str[0] == "C"){//Счетчики C
        address->memArea = S7AreaCT;
        address->type = S7WLCounter;
    }
    else if(str[0] == "T"){//Таймеры T
        address->memArea = S7AreaTM;
        address->type = S7WLTimer;
    }
    else return false;
    str.remove(0, 1);//Удаление маркеров типа памяти

    if( address->memArea == S7AreaMK
        ||address->memArea == S7AreaPE
        ||address->memArea == S7AreaPA ){
        //Тип данных
        if( str[1] == "W" ){
            address->type = S7WLWord;
            str.remove(0,1);//Удаление прочитанного символа, чтоб остался только номер адреса.
        }
        else if( str[1] == "D" ){
            address->type = S7WLDWord;
            str.remove(0,1);//Удаление прочитанного символа, чтоб остался только номер адреса.
        }
        else{
            address->type = S7WLBit;
        }
    }
    else if(address->memArea == S7AreaDB){
        address->DBNumb = str.left( str.indexOf('.', 0)).toInt(&ok);
        str.remove(0, str.indexOf('.', 0) + 3);//Удаление номера DB с точкой и началом адреса "23.DBX0.4" ->"X0.4".
        if(!ok) return false;
        if( str[0] == "W" ){
            address->type = S7WLWord;
        }
        else if( str[0] == "D" ){
            address->type = S7WLDWord;
        }
        else if( str[0] == "X" ){
            address->type = S7WLBit;
        }
        else return false;
        str.remove(0,1);//Удаление прочитанного символа, чтоб остался только номер адреса.
    }
    //Определение длинны данных от типа тэга.
    if (address->type == S7WLBit ){
        address->regAddr.bit = str.right(1).toInt(&ok);//Чтение номера бита.
        if(!ok) return false;
        str.remove(str.size() - 2, 2);//Удаление номера бита и точки.
        address->regAddr.memSlot = str.toInt(&ok);//Чтение номера байта.
    }
    else {
        address->regAddr.memSlot = str.toDouble(&ok);//Чтение номера байта
    }
    return ok;

    //!    Bool	    0.0 - 1
    //!    Byte	    1.0 - 1
    //!    Char	    2.0 - 1
    //!    Char	    3.0
    //!    DInt	    4.0 - 4
    //!    DWord	    8.0 - 4
    //!    Date	    12.0 - 2
    //    Date_And_Time 14.0 - 8
    //    Date_And_Time 22.0
    //!    Int	    30.0 - 2
    //!    Real	    32.0 - 4
    //!    S5Time	    36.0 - 2
    //    String	    38.0 - 256
    //    String	    294.0
    //!    Time	    550.0 - 4
    //!    Time_Of_Day   554.0 - 4
    //!    Word	    558.0 - 2
    //    Word	    560.0
}

//------------------------------------------------------------------------------
bool SimaticDriver::insertGroup(Group *group)
{
    if( Driver::insertGroup( group ) ){
        group->listOfTags.sort = SimaticDriver::sortTags;
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
SimAddress &SimAddress::operator=(const SimAddress &SA){
    memArea = SA.memArea;
    type = SA.type;
    DBNumb = SA.DBNumb;
    regAddr = SA.regAddr;
    return *this;
}

//------------------------------------------------------------------------------
bool SimAddress::operator> ( SimAddress const& SA)
{
    if (memArea > SA.memArea)
        return true;
    else if (memArea < SA.memArea)
        return false;
    else if(memArea == S7AreaDB ){
        if (DBNumb > SA.DBNumb)
            return true;
        else if (DBNumb < SA.DBNumb)
            return false;
        else if (regAddr.memSlot > SA.regAddr.memSlot)
            return true;
        else if (regAddr.memSlot < SA.regAddr.memSlot)
            return false;
        else return (regAddr.bit > SA.regAddr.bit);
    }
    else{
        if (type > SA.type)
            return true;
        else if (type < SA.type)
            return false;
        else if (regAddr.memSlot > SA.regAddr.memSlot)
            return true;
        else if (regAddr.memSlot < SA.regAddr.memSlot)
            return false;
        else return (regAddr.bit > SA.regAddr.bit);
    }
    return false;
}
