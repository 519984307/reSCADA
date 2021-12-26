#include "simatic.h"
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
    char charAddress[ 256 ];
    memcpy( charAddress, address.toStdString().c_str(), address.size() );
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
        group->update();
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
//Разбивает теги группы на задачи(task) с тэгами у которых адреса идут один за другим.
void SimaticDriver::getTask()
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
          task->DBNumb = address->DBNumb;
          task->memArea = address->memArea;
          task->type = task->memArea == S7AreaDB ? S7WLByte : address->type;
          task->regAddr = address->regAddr;
          task->groupId = group->id;
          task->listOfTags.append( group->listOfTags.at(i) );
          listOfTasks.append(task);
        }
        else{//Добавление новых тегов в текущую задачу.
          if( address->DBNumb == task->DBNumb // DB совпадает
              && address->memArea == task->memArea// и обл. памяти совпадает
              && address->type == task->type//! и тип совпадает TODO ужно оптимизировать, чтоб читать из одной DB разные типы скопом.
              && task->listOfTags.at(i)->getQuality() == Good//и тег уже успешно опрашивался
              && task->regAddr.memSlot + group->optimRangeMax//и адрес не очень далеко от 1-го тэга в задаче.
                   <= address->regAddr.memSlot) {
            //task->buffByteCount += DataSizeByte(address->type);//Прибавление длинны новогот эга к общей длинне буфера.
            task->listOfTags.append(group->listOfTags.at(i));//Добавление тэга в задачу.
          }
          else{//иначе завершаем группу
            i--;//Шаг назад, чтобы начать с текущего тэга новую задачу.
            task = nullptr;
          }
        }

      }
    }
  }
  delete address;
}
//------------------------------------------------------------------------------
void SimaticDriver::valueFiller(SimaticDriver::Task *task, byte * data[])
{
  //  int start = task->regAddr / 8;
  //  for(int i = 0; i < task->listOfTags.count(); i++){
  //    if(task->listOfTags[i])
  //      task->listOfTags[i]->setValue(
  //        ((data[(task->regAddr + i) / 8 - start])
  //         & (1 << ((task->regAddr + i) % 8))) == 0 ? false : true);
  //  }
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
inline int toBitAdr( SimAddress *SA )
{
  return SA->regAddr.memSlot * 8 + SA->regAddr.bit;
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
  SimAddress * lastTagAdr = static_cast<SimAddress*>(task->listOfTags.last()->speshData);

  int amt{0};
  if( task->listOfTags.count() == 1 ) amt = 1;
  else if( task->type == S7WLBit){
    amt = 1 + (lastTagAdr->regAddr.memSlot - task->regAddr.memSlot ) * 8 + lastTagAdr->regAddr.bit - task->regAddr.bit;
  }
  else{
    amt = 1 + lastTagAdr->regAddr.memSlot - task->regAddr.memSlot;
  }
  byte data[amt * dataSizeByte(task->type)];
  int res = 0;
  //int start = task->regAddr;
  res = client->ReadArea(task->memArea, task->DBNumb,
                         task->type == S7WLBit ? toBitAdr( task )
                                               : task->regAddr.memSlot,
                         amt, task->type, data);
  if(res == 0){

    foreach (Tag *tag, task->listOfTags) {

    }





    //valueFiller(task, &data);
    errorFiller(task->listOfTags, "");
    qualityFiller(task->listOfTags, Good);
  }

  if( res != 0 ){
    if (res == 589856)
      noError = false;
    errorFiller(task->listOfTags, "driver read error: " + QString(CliErrorText(res).c_str()));
    qualityFiller(task->listOfTags, Bad);
  }
  //delete []data;
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
  long long data[task->listOfTags.count()];
  for(int i = 0; i < task->listOfTags.count(); i++){ //перебираю массив
    if(task->listOfTags[i])//TODO Странная проверка наверное убрать.
      //Запись новых значений тэгов в массив.
      data[i] = task->listOfTags[i]->newValue.toLongLong();
    else
      data[i] = 0;
  }
  TS7DataItem Item; //создаю Item(структура из snap7)
  //чтоб закинуть в него новые значения тэгов
  Item.Area     = task->memArea;
  Item.WordLen  = task->type;
  Item.DBNumber = task->DBNumb;
  Item.Start    = task->regAddr.memSlot;
  Item.Amount   = task->listOfTags.count();
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
void SimaticDriver::sortTags(QList<Tag *> &listOfTags)
{
  //TODO добавить добавление данных в speshData

  if( listOfTags.count() < 2 ) return;
  //SimAddress * address = new SimAddress;
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
  bool ready {false};
  while( !ready ){ //bubble sorting
    ready = true;
    for( int i = 0; i < listOfTags.count() - 1; i++ ){
      if( compare(static_cast<SimAddress*>(listOfTags.at(i    )->speshData),
                  static_cast<SimAddress*>(listOfTags.at(i + 1)->speshData)) ){
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
  else if(str.right(2)== "DB"){//DB
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
    if( str[1] == "W" ){
      address->type = S7WLWord;
    }
    else if( str[1] == "D" ){
      address->type = S7WLDWord;
    }
    else if( str[1] == "X" ){
      address->type = S7WLBit;
    }
    else return false;
    str.remove(0,1);//Удаление прочитанного символа, чтоб остался только номер адреса.
  }
  //Определение длинны данных от типа тэга.
  if (address->type == S7WLBit ){
    address->regAddr.bit = str.left(1).toInt(&ok);//Чтение номера бита.
    if(!ok) return false;
    str.remove(str.size() - 2, 2);//Удаление номера бита и точки.
    address->regAddr.memSlot = str.toInt(&ok);//Чтение номера байта.
    if(!ok) return false;
  }
  else {
    address->regAddr.memSlot = str.toDouble(&ok);//Чтение номера байта
    if(!ok) return false;
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
bool SimaticDriver::compare(SimAddress *a1, SimAddress *a2)
{
  if (a1->memArea > a2->memArea)
    return true;
  else if (a1->memArea < a2->memArea)
    return false;
  else if(a1->memArea == S7AreaDB ){
    if (a1->DBNumb > a2->DBNumb)
      return true;
    else if (a1->DBNumb < a2->DBNumb)
      return false;
    else if (a1->regAddr.memSlot > a2->regAddr.memSlot)
      return true;
    else if (a1->regAddr.memSlot < a2->regAddr.memSlot)
      return true;
    else return (a1->regAddr.bit > a2->regAddr.bit);
  }
  else{
    if (a1->type > a2->type)
      return true;
    else if (a1->type < a2->type)
      return false;
    else if (a1->regAddr.memSlot > a2->regAddr.memSlot)
      return true;
    else if (a1->regAddr.memSlot < a2->regAddr.memSlot)
      return true;
    else return (a1->regAddr.bit > a2->regAddr.bit);
  }
  return false;
}
//------------------------------------------------------------------------------
void SimaticDriver::writeRequest(Tag *tag)
{
  SimAddress address;
  if(!strToAddr(tag->address, &address)){
    emit s_logging(MessError, QDateTime::currentDateTime(), false, objectName(), "Simatic driver write error: address isn\'t valid");
    return;
  }

  Task * task = new Task;
  task->writeTask = true;
  task->memArea = address.memArea;
  task->DBNumb = address.DBNumb;
  task->regAddr = address.regAddr;
  task->type = address.type;
  task->listOfTags.append(tag);

  foreach (Task * task, listOfTasks) {
    if(task->memArea == address.memArea
        && task->regAddr.memSlot == address.regAddr.memSlot
        && task->regAddr.bit == address.regAddr.bit
        && task->type == address.type
        && task->writeTask){
      listOfTasks.removeOne(task);
      delete task;
    }
  }
  listOfTasks.prepend(task);
}
//------------------------------------------------------------------------------
