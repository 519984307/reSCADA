#include "simatic.h"
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
  if( !client ) return;

  if( !client->Connected() || !noError ){
    char charAddress[ address.size() ];//TODO сделать нормальный размер
    memcpy( &charAddress, address.toStdString().c_str(), address.size() );
    charAddress[ address.size() ] = '\0';

    // Запрос на соединение
    int res = client->ConnectTo(charAddress, rack, slot);

    if( res == 0 ) {//Успешно соединился
      emit s_logging(MessInfo, QDateTime::currentDateTime(), false,
                     this->objectName(), "Simatic driver connected");
      noError = true;
    }
    else {//Нет соединения. Сообщение об ошибке в лог.
      noError = false;
      emit s_logging(MessError, QDateTime::currentDateTime(),
                     false, this->objectName(),
                     std::string("Simatic driver connection error: "
                                 + ( res < 0 ? "libruary error" : CliErrorText(res)) ).c_str() );
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
  if (listOfTasks.count() > 0){
    Task * task = listOfTasks.takeFirst();
    if (task->writeTask)
      write(task);
    else
      read(task);
    delete task;
  }
  else{
    createReadTasks();
  }
  scheduleHandler();
}
//------------------------------------------------------------------------------
//Разбивает теги группы на задачи(task) с тэгами у которых адреса идут один за другим.
inline void SimaticDriver::createReadTasks()
{
  SimAddress * address;
  Task * task = nullptr;//Текущая задача.
  foreach (Group * group, listOfGroups) {//Перебор групп.
    //Если задержка группы на опрос прошла.
    if (group->lastUpdate.msecsTo(QDateTime::currentDateTime()) > group->delay){

      //Перебор тэгов в группе, для разделения на задачи.
      for (int i = 0; i < group->listOfTags.count(); i++){
        //Пулучение адреса тэга з приепленной к нему структуры.
        address = static_cast<SimAddress*>( group->listOfTags.at(i)->speshData );
        if( address->regAddr.memSlot == -1 ) continue; //Пропуск негодных тэгов

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
              && (task->memArea == S7AreaDB ? true : address->type == task->type)//! и тип совпадает TODO ужно оптимизировать, чтоб читать из одной DB разные типы скопом.
              && group->listOfTags.at(i)->getQuality() == Good//и тег уже успешно опрашивался
              && task->regAddr.memSlot + group->optimRangeMax//и адрес не очень далеко от 1-го тэга в задаче.
                   >= address->regAddr.memSlot) {
            task->listOfTags.append(group->listOfTags.at(i));//Добавление тэга в задачу.
          }
          else{//иначе завершаем группу
            i--;//Шаг назад, чтобы начать с текущего тэга новую задачу.
            task = nullptr;
          }
        }

      }
      group->update();//Не уместно: группа обновляет данные перед новым запросом
    }
  }
}
//------------------------------------------------------------------------------
void SimaticDriver::createWriteTask(Tag *tag)
{
  SimAddress *address;
  address = static_cast<SimAddress*>( tag->speshData );
  Task * task = new Task;
  task->writeTask = true;
  task->memArea = address->memArea;
  task->DBNumb = address->DBNumb;
  task->type = task->memArea == S7AreaDB ? S7WLByte : address->type;//address->type;
  task->regAddr = address->regAddr;
  task->listOfTags.append( tag );
  listOfTasks.append(task);
}
//------------------------------------------------------------------------------
inline int toBitAdr( SimAddress *SA )
{
  return SA->regAddr.memSlot * 8 + SA->regAddr.bit;
}
//------------------------------------------------------------------------------
inline int amtFromType( SimAddress *lastTagAdr, SimaticDriver::Task *task )
{
  if( lastTagAdr->memArea == S7AreaDB ){//Т.к. из BD читаем только байтами нужно добавить биты от типов данных длиннее байта
    return  (lastTagAdr->regAddr.memSlot - task->regAddr.memSlot)
           + dataSizeByte(lastTagAdr->type);
  }
  else if( task->type == S7WLBit ){
    return 1 + (lastTagAdr->regAddr.memSlot - task->regAddr.memSlot ) * 8
           + lastTagAdr->regAddr.bit - task->regAddr.bit;
  }
  else{
    return 1 + lastTagAdr->regAddr.memSlot - task->regAddr.memSlot;// +1 т.к. включая начальный элемент
  }
}
//------------------------------------------------------------------------------
void SimaticDriver::read( SimaticDriver::Task *task )
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
            tag->setValue( GetBitAt( data, tagAdr->regAddr.memSlot * sizeKf,
                                   tagAdr->regAddr.bit ) );
            break;
          case S7WLByte:
            tag->setValue( GetByteAt( data, tagAdr->regAddr.memSlot * sizeKf) );
            break;
          case S7WLWord:
            tag->setValue( GetIntAt( data, tagAdr->regAddr.memSlot * sizeKf ) );
            break;
          case S7WLCounter:
          case S7WLTimer:
            tag->setValue( GetWordAt( data, tagAdr->regAddr.memSlot * sizeKf ) );
            break;
          case S7WLDWord:
            if( tag->type == TFloat ) tag->setValue( GetRealAt( data, tagAdr->regAddr.memSlot * sizeKf ));
            else tag->setValue( GetDIntAt( data, tagAdr->regAddr.memSlot * sizeKf ) );
            break;
            //Такого варианта не будет, т.к. он кодируется в егах просто как Double Word
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
  else{
    errorFiller(task->listOfTags, "driver read error: driver isn\'t connected");
    qualityFiller(task->listOfTags, Bad);
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
#define offsetBuff (tagAdr->regAddr.memSlot - task->regAddr.memSlot) * sizeKf
      switch (tagAdr->type) {
        case S7WLBit:
          //!TODO попробовать, может ПЛК нормально отреагирует на установку бита в DB с task->type == S7WLBit?
          //! тогда не нужно будет предварительно считывать байт, чтоб не затереть соседние биты.

          if( task->memArea == S7AreaDB ){//Чтобы не обнулить рядомстоящие биты запрос их актуального состояния.
            res = client->ReadArea(task->memArea, task->DBNumb, tagAdr->regAddr.memSlot,
                                   amt, task->type,
                                   &data[offsetBuff]);
          }
          SetBitAt( data, offsetBuff, tagAdr->regAddr.bit, task->listOfTags.at(i)->newValue.toBool() );
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
  if (started)
    taskTimer->singleShot(1, this, &SimaticDriver::handleNextTask);
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
      if( *static_cast<SimAddress*>(listOfTags.at(i    )->speshData)> *static_cast<SimAddress*>(listOfTags.at(i + 1)->speshData) ){
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
