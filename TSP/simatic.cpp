#include "simatic.h"
//------------------------------------------------------------------------------
SimaticDriver::SimaticDriver(int id, QString name, QString address, int rack, int slot, QString comment)
{
    this->type = "simatic";
    this->Id = id;
    this->setObjectName(name);
    this->address = address;
    this->rack = rack;
    this->slot = slot;
    this->Comment = comment;
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
    QObject::connect(this, &Driver::connectDriver, this, &Driver::connect, Qt::QueuedConnection);
    QObject::connect(this, &Driver::disconnectDriver, this, &Driver::disconnect, Qt::QueuedConnection);
}
//------------------------------------------------------------------------------
//Проверяет соединение с ПЛК, если его нет - соединяет
void SimaticDriver::connect()
{
    if( !client ) return;

    if( !client->Connected() || !noError ){
        char charAddress[ 256 ];
        memcpy( charAddress, address.toStdString().c_str(), address.size() );
        charAddress[ address.size() ] = '\0';

        // Запрос на соединение
        int res = client->ConnectTo(charAddress, rack, slot);

        if( res == 0 ) {//Успешно соединился
            emit LoggingSig(MessInfo, QDateTime::currentDateTime(), false,
                this->objectName(), "Simatic driver connected");
            noError = true;
        }
        else {//Нет соединения. Сообщение об ошибке в лог.
            noError = false;
            emit LoggingSig(MessError, QDateTime::currentDateTime(),
                false, this->objectName(),
                std::string("Simatic driver connection error: "
                    + ( res < 0 ? "libruary error" : CliErrorText(res)) ).c_str() );
        }

        if (!started){
            started = true;
            emit onStartedChanged();
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
    emit LoggingSig(MessInfo, QDateTime::currentDateTime(), false, this->objectName(), "Simatic driver disconnected");
    foreach(Group * group, listOfGroups){
        qualityFiller(group->ListOfTags, Bad);
        group->Update();
    }
    emit onStartedChanged();
}

//------------------------------------------------------------------------------
void SimaticDriver::handleNextTask()
{
    if (listOfTasks.count() > 0){
        Task * task = listOfTasks.takeFirst();
        if (task->writeTask){
            write(task,
                [this, task](){
                    delete task;
                    scheduleHandler();
                }
            );
        }
        else{
            read(task, [this, task](){
                Group * group = getGroupById(task->groupId);
                group->Update();
                delete task;
                scheduleHandler();
            });
        }
    }
    else{
        getTask();
        scheduleHandler();
    }

}
//------------------------------------------------------------------------------
void SimaticDriver::getTask()
{
    SimAddress * address = new SimAddress;
    Task * task = nullptr;
    foreach (Group * group, listOfGroups) {
        if (group->LastUpdate.msecsTo(QDateTime::currentDateTime()) > group->Delay){
            //! переработать. Нахрена повторять одно и тоже каждый раз?
            //! ввести в группу параметр prepared
            QList<Tag *> newTagList = prepareTagsList(group->ListOfTags);
            int i = 0;
            while (i < newTagList.count()){
                bool send = false;
                if(!strToAddr(newTagList[i]->address, address)){ //get address
                    emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Simatic driver error: address isn\'t valid in getTask() function");
                }
                if (!task){
                    task = new Task;
                    task->quantity = 1;
                    task->area = address->area;
                    task->type = address->type;
                    task->regAddr = address->regAddr;
                    task->groupId = group->Id;
                    task->listOfTags.append(newTagList[i]);
                }else if(task->quantity == group->OptimizerRangeMax){ //range max exception
                    send = true;
                }else{
                    if (task->area != address->area || task->type != address->type ||
                        task->regAddr + task->quantity + group->OptimizerRangeInterval < address->regAddr){ //area or type or range interval exception
                        send = true;
                    }else{
                        for(int i = 0; i < address->regAddr - task->regAddr - task->quantity ; i++){ //filler. Runs 0 times if current reg addr - prev reg addr = 1
                            task->listOfTags.append(nullptr);
                        }
                        task->quantity = address->regAddr - task->regAddr + 1;
                        task->listOfTags.append(newTagList[i]);
                    }
                }
                if (i+1 == newTagList.count()){ //last tag exception
                    if(!send){
                        send = true;
                        i++;
                    }
                }
                if(send){
                    listOfTasks.append(task);
                    task = nullptr;
                }else{
                    i++;
                }
            }
        }
    }
    delete address;
}
//------------------------------------------------------------------------------
void SimaticDriver::valueFiller(SimaticDriver::Task *task, byte * data[])
{
    int start = task->regAddr / 8;
    for(int i = 0; i < task->listOfTags.count(); i++){
        if(task->listOfTags[i])
            task->listOfTags[i]->setValue(
                ((data[(task->regAddr + i) / 8 - start])
                    & (1 << ((task->regAddr + i) % 8))) == 0 ? false : true);
    }
}
//------------------------------------------------------------------------------
//void Simaticdriver::valueFiller(QList<Tag *> listOfTags, uint8_t * data)
template<typename Tarr>
void SimaticDriver::valueFiller(QList<Tag *> listOfTags, Tarr data[])
{
    for(int i = 0; i < listOfTags.count(); i++){
        if(listOfTags[i])
            listOfTags[i]->setValue(data[i]);
    }
}
//------------------------------------------------------------------------------
void SimaticDriver::read(SimaticDriver::Task *task, const std::function<void ()> doNext)
{
    if(!client) {
        errorFiller(task->listOfTags, "driver read error: driver is null"); qualityFiller(task->listOfTags, Bad);
        //emit LoggingSystem(MessError, QDateTime::currentDateTime(), this->objectName(), "Simatic driver read error: driver is null");
        return;
    }
    if (started) connect();
    if(!client->Connected()){
        errorFiller(task->listOfTags, "driver read error: driver isn\'t connected"); qualityFiller(task->listOfTags, Bad);
        doNext();
        //emit LoggingSystem(MessError, QDateTime::currentDateTime(), this->objectName(), "Simatic driver read error: driver isn\'t connected");
        return;
    }

    int typeLen{1};
    switch (task->type) {
    //case S7WLBit    : //убрал, т.к. typeLen по умолчанию = 1
    //case S7WLByte   : typeLen = 1;
    case S7WLWord   :
    case S7WLCounter:
    case S7WLTimer  : typeLen = 2;
    case S7WLDWord  :
    case S7WLReal   : typeLen = 4;
    default:;
    }
    byte * data;
    data = new byte[task->quantity * typeLen];
    for(int i = 0; i < task->quantity * typeLen; i++){
        data[i] = 0;
    }
    int res = 0;
    if ( task->quantity > 1 ){
        int start = task->regAddr / 8;
        int end = (task->regAddr + task->quantity) / 8;
        res = client->ReadArea(task->area, task->DBNumb, start, end - start + 1, task->type, &data);
        if(res == 0){
            valueFiller(task, &data);
            errorFiller(task->listOfTags, "");
            qualityFiller(task->listOfTags, Good);
        }
    }
    else{
        res = client->ReadArea(task->area, task->DBNumb, task->regAddr, task->quantity, task->type, &data);
        if(res == 0){
            valueFiller(task->listOfTags, data);
            errorFiller(task->listOfTags, "");
            qualityFiller(task->listOfTags, Good);
        }
    }
    if( res != 0 ){
        if (res == 589856)
            noError = false;
        errorFiller(task->listOfTags, "driver read error: " + QString(CliErrorText(res).c_str()));
        qualityFiller(task->listOfTags, Bad);
    }
    doNext();
    return;
}
//------------------------------------------------------------------------------
void SimaticDriver::write(SimaticDriver::Task *task, const std::function<void ()> doNext)
{
    if(!client) {
        errorFiller(task->listOfTags, "driver write error: driver is null"); qualityFiller(task->listOfTags, Bad);
        //emit LoggingSystem(MessError, QDateTime::currentDateTime(), this->objectName(), "Simatic driver read error: driver is null");
        return;
    }
    if(started) connect(); //Если драйвер запущен
    if(!client->Connected()){ //TODO переделать. Попытки подключиться должны крутиться в цикле в драйвере.
        errorFiller(task->listOfTags,
            "driver write error: driver isn\'t connected");
        qualityFiller(task->listOfTags, Bad);
        doNext();
        return;
    }
    //создаю массив для новых значений тэгов
    // TODO long long избыточно кадому типу своё
    long long data[task->quantity];
    for(int i = 0; i < task->quantity; i++){ //перебираю массив
        if(task->listOfTags[i])//TODO Странная проверка наверное убрать.
            //Запись новых значений тэгов в массив.
            data[i] = task->listOfTags[i]->newValue.toLongLong();
        else
            data[i] = 0;
    }
    TS7DataItem Item; //создаю Item(структура из snap7)
    //чтоб закинуть в него новые значения тэгов
    Item.Area     = task->area;
    Item.WordLen  = task->type;
    Item.DBNumber = task->DBNumb;
    Item.Start    = task->regAddr;
    Item.Amount   = task->quantity;
    Item.pdata    = &data;

    client->WriteMultiVars(&Item, 1);//запрос к ПЛК

    if (Item.Result == 0){//0 значит успешно выполнен.
        //Заполнение тэгов результатами запроса и служебной инфой.
        readyFiller(task->listOfTags, true);
        newValueFiller(task->listOfTags);
        errorFiller(task->listOfTags, "");
        qualityFiller(task->listOfTags, Good);
    }
    else{
        errorFiller(task->listOfTags, "driver write error: "
                + QString(CliErrorText(Item.Result).c_str()));
        qualityFiller(task->listOfTags, Bad);
        if (Item.Result == 589856)
            noError = false;
    }

    doNext();//переход к следующей tack
    //TODO убрать этот дебильный переход к след. задаче ерез ямбду
}

//------------------------------------------------------------------------------
void SimaticDriver::scheduleHandler()
{
    if (started)
        taskTimer->singleShot(1, this, &SimaticDriver::handleNextTask);
}
//------------------------------------------------------------------------------

QList<Tag *> SimaticDriver::sortTags(QList<Tag *> listOfTags)
{
    bool ready = false;
    if (listOfTags.count() < 2) return listOfTags;
    int i = 0;
    SimAddress * address = new SimAddress;
    while (i < listOfTags.count()) { // bad tags cleaning
        if (!strToAddr(listOfTags[i]->address, address)){
            listOfTags.removeAt(i);
        }else{
            i++;
        }
    }
    delete address;
    SimAddress * address1 = new SimAddress;
    SimAddress * address2 = new SimAddress;
    while (1){ //bubble sorting

        for (int i = 0; i < listOfTags.count() - 1; i++){
            strToAddr(listOfTags[i  ]->address, address1);
            strToAddr(listOfTags[i+1]->address, address2);
            if (compare(*address1, *address2)){
                listOfTags.swapItemsAt(i, i+1);
                ready = false;
            }
        }
        if (ready)
            break;
        else
            ready = true;

    }
    delete address1;
    delete address2;
    return listOfTags;
}
//------------------------------------------------------------------------------
bool SimaticDriver::strToAddr(QString str, SimAddress *address)
{
    if(!address) return false;
    if (str.count() < 2) return false;
    str = str.toLower();
    QString MemType = str.left(1);
    str.remove(0, 1);
    if (MemType == "m"){
        address->area = S7AreaMK;
    }
    else if(MemType == "i"){
        address->area = S7AreaPE;
    }
    else if(MemType == "q"){
        address->area = S7AreaPA;
    }
    else if(MemType == "d"){
        address->area = S7AreaDB;
    }
    else if(MemType == "c"){
        address->area = S7AreaCT;
    }
    else if(MemType == "t"){
        address->area = S7AreaTM;
    }
    else return false;
    if(str == "") return false;
    QStringList addressParts = str.split(".", Qt::SkipEmptyParts);
    if(addressParts.count() == 1){
        if (addressParts[0].count() < 2) return false;
        QString DataType = addressParts[0].left(1);
        addressParts[0].remove(0, 1);
        if (DataType == "b"){
            address->type = S7WLBit;
        }else if(DataType == "B"){
            address->type = S7WLByte;
        }else if(DataType == "w"){
            address->type = S7WLWord;
        }else if(DataType == "d"){
            address->type = S7WLDWord;
        }else if(DataType == "r"){
            address->type = S7WLReal;
        }else if(DataType == "c"){
            address->type = S7WLCounter;
        }else if(DataType == "t"){
            address->type = S7WLTimer;
        }else return false;
        bool ok = false;
        address->regAddr = addressParts[0].toInt(&ok);
        return ok;
    }else if(addressParts.count() == 2){
        address->type = S7WLBit;
        bool ok = false;
        address->regAddr = addressParts[0].toInt(&ok) * 8;
        if (!ok) return false;
        address->regAddr += addressParts[1].toInt(&ok);
        return ok;
    }else return false;
}
//------------------------------------------------------------------------------
bool SimaticDriver::compare(SimAddress a1, SimAddress a2)
{
    if (a1.area > a2.area)
        return true;
    else if (a1.area < a2.area)
        return false;
    else if (a1.type > a2.type)
        return true;
    else if (a1.type < a2.type)
        return false;
    else if (a1.regAddr > a2.regAddr)
        return true;
    else
        return false;
}
//------------------------------------------------------------------------------
void SimaticDriver::WriteRequested(Tag *tag)
{
    SimAddress address;
    if(!strToAddr(tag->address, &address)){
        emit LoggingSig(MessError, QDateTime::currentDateTime(), false, objectName(), "Modbus driver write error: address isn\'t valid");
        return;
    }

    Task * task = new Task;
    task->writeTask = true;
    task->area = address.area;
    task->regAddr = address.regAddr;
    task->type = address.type;
    task->quantity = 1;
    task->listOfTags.append(tag);

    foreach (Task * task, listOfTasks) {
        if(task->area == address.area && task->regAddr == address.regAddr && task->type == address.type && task->writeTask){
            listOfTasks.removeOne(task);
        }
    }
    listOfTasks.prepend(task);
}
//------------------------------------------------------------------------------
