#include "modbus.h"
#include <QDebug>
//------------------------------------------------------------------------------
Modbusdriver::Modbusdriver(int id, QString name, QString address, int port, int timeout, QString comment)
{
    initThread(new QModbusTcpClient());
    modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, address);
    modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, port);
    modbusDevice->setTimeout(timeout); //ms
    this->Id = id;
    this->setObjectName(name);
    this->Comment = comment;
    this->type = "modbusTCP";
}

//------------------------------------------------------------------------------
Modbusdriver::Modbusdriver(int id, QString name, QString port, QString baudrate, QSerialPort::DataBits databits, QSerialPort::Parity parity, QSerialPort::StopBits stopbits, int timeout, QString comment)
{
    initThread(new QModbusRtuSerialMaster());
    modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter, QVariant(port));
    modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, databits);
    modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter, parity);
    modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, QVariant(baudrate));
    modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, stopbits);
    modbusDevice->setTimeout(timeout); //ms
    this->Id = id;
    this->setObjectName(name);
    this->Comment = comment;
    this->type = "modbusRTU";
}
//------------------------------------------------------------------------------
Modbusdriver::~Modbusdriver()
{
    //Disconnect();

    foreach (Task * task, listOfTasks){
        delete task;
    }
    modbusDevice->deleteLater();
    modbusDevice = nullptr;
    delete taskTimer;
}

//------------------------------------------------------------------------------
void Modbusdriver::handleNextTask()
{
    if (listOfTasks.count() > 0){
        Task * task = listOfTasks.takeFirst();
        if (task->writeTask){
            write(task, [this, task]()
            {///Atantion Lambda!!
                delete task;
                scheduleHandler();
            });
        }
        else{
            read(task, [this, task]()
            {///Atantion Lambda!!
                Group * group = getGroupById(task->groupId);
                group->Update();
                delete task;
                scheduleHandler();
            });
        }
    }else{
        getTask();
        scheduleHandler();
    }
    //qDebug()<<listOfTasks.count();
}

//------------------------------------------------------------------------------
void Modbusdriver::getTask()///Переделать!!!
{
    MBaddress * curTagAdr = new MBaddress;
    Task * task = nullptr;
    foreach (Group * group, listOfGroups) {//перебираю группы

        if(group->LastUpdate.msecsTo(QDateTime::currentDateTime()) > group->Delay){///Что это бля?
            QList<Tag *> newTagList = prepareTagsList(group->ListOfTags);

            for(int i = 0; i < newTagList.count(); i++){//перебираю тэги

                bool send = false;

                if(!strToAddr(newTagList[i]->address, curTagAdr)){ //get address
                    emit LoggingSig(MessError, QDateTime::currentDateTime(), false, objectName(), "Modbus driver error: address isn\'t valid in getTask() function");
                }
                if(!task){
                    task = new Task;
                    task->quantity = 1;
                    task->devAddr = curTagAdr->devAddr;
                    task->regType = curTagAdr->regType;
                    task->regAddr = curTagAdr->regAddr;
                    task->groupId = group->Id;
                    task->listOfTags.append(newTagList[i]);
                }
                else{
                    if (task->devAddr != curTagAdr->devAddr || task->regType != curTagAdr->regType  ///Тут
                         || task->regAddr + task->quantity + group->OptimizerRangeInterval < curTagAdr->regAddr
                         || curTagAdr->regAddr != task->regAddr + task->quantity){ //dev addr or reg type or range interval exception
                        send = true;
                        i--;
                    }
                    else{
//                        for(int i = 0; i < curTagAdr->regAddr - (task->regAddr - task->quantity); i++){ //filler. Runs 0 times if current reg addr - prev reg addr = 1
//                            task->listOfTags.append(nullptr);
//                        }
                        //task->quantity = curTagAdr->regAddr - task->regAddr + 1;
                        task->quantity ++;
                        task->listOfTags.append(newTagList[i]);
                        if(task->quantity == group->OptimizerRangeMax)send = true; //range max exception
                    }
                }
                if(send || i + 1 == newTagList.count()){
                    listOfTasks.append(task);
                    task = nullptr;
                }
            }//перебираю тэги
        }
    }//перебираю группы
    delete curTagAdr;
}

//------------------------------------------------------------------------------
void Modbusdriver::connect()
{
    if(!modbusDevice) return;
    if(modbusDevice->state() != QModbusDevice::ConnectedState) {
        if(!modbusDevice->connectDevice()) {
            if(noError){
                qualityFiller(listOfGroups, Bad);
                emit LoggingSig(MessError, QDateTime::currentDateTime(), false, objectName(), "Modbus driver connection error: " + modbusDevice->errorString());
                noError = false;
            }
        }else{
            if(modbusDevice->state() == QModbusDevice::ConnectedState){
                emit LoggingSig(MessInfo, QDateTime::currentDateTime(), false, objectName(), "Modbus driver connected");
                noError = true;
            }
            if (!started){
                started = true;
                emit onStartedChanged();
                scheduleHandler();
            }
        }
    }
}

//------------------------------------------------------------------------------
void Modbusdriver::disconnect()
{
    if(!modbusDevice) return;
    started = false;
    modbusDevice->disconnectDevice();
    taskTimer->stop();
    emit LoggingSig(MessInfo, QDateTime::currentDateTime(), false, objectName(), "Modbus driver disconnected");
    foreach(Group * group, listOfGroups){
        qualityFiller(group->ListOfTags, Bad);
        group->Update();
    }
    emit onStartedChanged();
}

//------------------------------------------------------------------------------
QList<Tag *> Modbusdriver::sortTags(QList<Tag *> listOfTags)
{
    bool ready = false;
    if (listOfTags.count() < 2) return listOfTags;
    QList<Tag *> newTagList = listOfTags;
    int i = 0;
    MBaddress * address = new MBaddress;
    while (i < newTagList.count()) { // bad tags cleaning
        if (!strToAddr(newTagList[i]->address, address)){
            newTagList.removeAt(i);
        }else{
            i++;
        }
    }
    delete address;
    MBaddress * address1 = new MBaddress;
    MBaddress * address2 = new MBaddress;
    while (! ready){ //bubble sorting
        ready = true;
        for (int i = 0; i < newTagList.count() - 1; i++){
            strToAddr(newTagList[i  ]->address, address1);
            strToAddr(newTagList[i+1]->address, address2);
            if (compare(*address1, *address2)){
                newTagList.swapItemsAt(i, i+1);
                ready = false;
            }
        }
    }
    delete address1;
    delete address2;
    return newTagList;
}

//------------------------------------------------------------------------------
void Modbusdriver::valueFiller(QList<Tag *> listOfTags, QModbusDataUnit unit)
{
    for (uint16_t i = 0; i < unit.valueCount(); i++) {
        if(listOfTags[i])
            listOfTags[i]->setValue(short(unit.value(i))); //signed fix
    }
}

//------------------------------------------------------------------------------
void Modbusdriver::read(Modbusdriver::Task *task, const std::function<void ()> doNext)
{  
    if (!modbusDevice) {
        errorFiller(task->listOfTags, "driver read error: driver is null");
        qualityFiller(task->listOfTags, Bad);
        //emit LoggingSig(MessError, QDateTime::currentDateTime(), objectName(), "Modbus driver read error: driver is null");
        return;
    }
    if (started) connect();
    if(modbusDevice->state() != QModbusDevice::ConnectedState){
        errorFiller(task->listOfTags, "driver read error: driver isn\'t connected");
        qualityFiller(task->listOfTags, Bad);
        doNext();
        //emit LoggingSig(MessError, QDateTime::currentDateTime(), objectName(), "Modbus driver read error: driver isn\'t connected");
        return;
    }
    if (auto *lastRequest = modbusDevice->sendReadRequest(QModbusDataUnit(task->regType, task->regAddr, task->quantity), task->devAddr)) {
        lastRequest->setProperty("twice", false);
        if (!lastRequest->isFinished()){
            QObject::connect(lastRequest, &QModbusReply::finished, this, [this, task, lastRequest,
                             doNext]()
                                {///Atantion Lambda!!
                                    if (lastRequest->property("twice").toBool()) //sometimes this solution helps to avoid QSerialBus bug
                                        return;
                                    lastRequest->setProperty("twice", true);

                                    if (!lastRequest){
                                        errorFiller(task->listOfTags, "driver read error: unknown error");
                                        qualityFiller(task->listOfTags, Bad);
                                        doNext();
                                        return;
                                    }

                                    if (lastRequest->error() == QModbusDevice::NoError) {
                                        //const QModbusDataUnit unit = lastRequest->result();
                                        valueFiller(task->listOfTags, lastRequest->result());
                                        errorFiller(task->listOfTags, "");
                                        qualityFiller(task->listOfTags, Good);
                                    }
                                    else if (lastRequest->error() == QModbusDevice::ProtocolError) {
                                        //emit LoggingSig(MessError, QDateTime::currentDateTime(), objectName(), "Modbus driver read error: " + lastRequest->errorString() + " (modbus exception: " + QString::number(int(lastRequest->rawResult().exceptionCode())) + ")");
                                        errorFiller(task->listOfTags, "driver read error: "
                                            + lastRequest->errorString() + " (modbus exception: "
                                            + QString::number(int(lastRequest->rawResult().exceptionCode())) + ")");
                                        qualityFiller(task->listOfTags, Bad);
                                    }
                                    else {
                                        //emit LoggingSig(MessError, QDateTime::currentDateTime(), objectName(), "Modbus driver read error: " + lastRequest->errorString() + " (code: " + lastRequest->error() + ")");
                                        errorFiller(task->listOfTags, "driver read error: "
                                            + lastRequest->errorString()
                                            + " (code: " + lastRequest->error() + ")");
                                        qualityFiller(task->listOfTags, Bad);
                                    }
                                    doNext();
                                    lastRequest->deleteLater();
                                });
        }else{
            errorFiller(task->listOfTags, "");
            qualityFiller(task->listOfTags, Good);
            doNext();
            delete lastRequest;
        }
    } else {
        errorFiller(task->listOfTags, "driver read error: " + modbusDevice->errorString());
        qualityFiller(task->listOfTags, Bad);
        doNext();
        //emit LoggingSig(MessError, QDateTime::currentDateTime(), objectName(), "Modbus driver read error: " + modbusDevice->errorString());
    }
}

//------------------------------------------------------------------------------
void Modbusdriver::write(Task * task, const std::function<void()> doNext)
{
    if (!modbusDevice) {
        //emit LoggingSig(MessError, QDateTime::currentDateTime(), objectName(), "Modbus driver write error: driver is null");
        errorFiller(task->listOfTags, "driver write error: driver is null");
        qualityFiller(task->listOfTags, Bad);
        doNext();
    }
    if (started) connect();
    if(modbusDevice->state() != QModbusDevice::ConnectedState){
        errorFiller(task->listOfTags, "driver write error: driver isn\'t connected");
        qualityFiller(task->listOfTags, Bad);
        doNext();
        //emit LoggingSig(MessError, QDateTime::currentDateTime(), objectName(), "Modbus driver write error: driver isn\'t connected");
        return;
    }
    QModbusDataUnit writeUnit = QModbusDataUnit(task->regType, task->regAddr, task->quantity);
    for (int i = 0; i < task->quantity; i++) {
        writeUnit.setValue(i, task->listOfTags[i]->newValue.toInt());
    }
    if (auto *lastRequest = modbusDevice->sendWriteRequest(writeUnit, task->devAddr)) {
        lastRequest->setProperty("twice", false); //sometimes this solution helps to avoid QSerialBus bug
        if (!lastRequest->isFinished()) {
            QObject::connect(lastRequest, &QModbusReply::finished, this,
                             [this, task, lastRequest, doNext]()
                                {///Atantion Lambda!!
                                    if (lastRequest->property("twice").toBool())
                                        return;
                                    lastRequest->setProperty("twice", true);
                                    if (lastRequest->error() == QModbusDevice::ProtocolError) {
                                        //emit LoggingSig(MessError, QDateTime::currentDateTime(), objectName(), "Modbus driver write response error: " + lastRequest->errorString() + " (modbus exception: " + QString::number(int(lastRequest->rawResult().exceptionCode())) + ")");
                                        errorFiller(task->listOfTags, "driver write response error: "
                                            + lastRequest->errorString() + " (modbus exception: "
                                            + QString::number(int(lastRequest->rawResult().exceptionCode())) + ")");
                                        qualityFiller(task->listOfTags, Bad);
                                        readyFiller(task->listOfTags ,true);
                                        doNext();
                                    }
                                    else if (lastRequest->error() != QModbusDevice::NoError) {
                                        //emit LoggingSig(MessError, QDateTime::currentDateTime(), objectName(), "Modbus driver write response error: "  + lastRequest->errorString() + " (code: " + lastRequest->error() + ")");
                                        errorFiller(task->listOfTags, "driver write response error: "
                                            + lastRequest->errorString() + " (code: " + lastRequest->error() + ")");
                                        qualityFiller(task->listOfTags, Bad);
                                        readyFiller(task->listOfTags ,true);
                                        doNext();
                                    }
                                    else{
                                        errorFiller(task->listOfTags, "");
                                        qualityFiller(task->listOfTags, Good);
                                        readyFiller(task->listOfTags ,true);
                                        newValueFiller(task->listOfTags);
                                        doNext();
                                    }
                                    lastRequest->deleteLater();
                                });
        }else{
            errorFiller(task->listOfTags, "");
            qualityFiller(task->listOfTags, Good);
            readyFiller(task->listOfTags ,true);
            newValueFiller(task->listOfTags);
            doNext();
            lastRequest->deleteLater();
            return;
        }
    }else{
        //emit LoggingSig(MessError, QDateTime::currentDateTime(), objectName(), "Modbus driver write error: unknown error");
        errorFiller(task->listOfTags, "driver write error: unknown error");
        qualityFiller(task->listOfTags, Bad);
        readyFiller(task->listOfTags ,true);
        doNext();
    }
}

//------------------------------------------------------------------------------
bool Modbusdriver::strToAddr(QString str, MBaddress * address)
{
    if(!address) return false;
    QStringList addrList = str.split(':', QString::SkipEmptyParts);
    if (addrList.count() != 3){
        emit LoggingSig(MessError, QDateTime::currentDateTime(), false, objectName(), "Modbus driver optimizer error: address isn\'t valid");
        return false;
    }else{
        bool ok = false;
        address->devAddr = addrList[0].toInt(&ok);
        if(!ok){
            emit LoggingSig(MessError, QDateTime::currentDateTime(), false, objectName(), "Modbus driver optimizer error: device address isn\'t valid");
            return false;
        }
        if (addrList[1] == "DC")
            address->regType = QModbusDataUnit::Coils;
        else if (addrList[1] == "DI")
            address->regType = QModbusDataUnit::DiscreteInputs;
        else if (addrList[1] == "IR")
            address->regType = QModbusDataUnit::InputRegisters;
        else if (addrList[1] == "HR")
            address->regType = QModbusDataUnit::HoldingRegisters;
        else{
            emit LoggingSig(MessError, QDateTime::currentDateTime(), false, objectName(), "Modbus driver optimizer error: register type isn\'t valid");
            return false;
        }
        address->regAddr = addrList[2].toInt(&ok);
        if(!ok){
            emit LoggingSig(MessError, QDateTime::currentDateTime(), false, objectName(), "Modbus driver optimizer error: register address isn\'t valid");
            return false;
        }
        return true;
    }
}

//------------------------------------------------------------------------------
void Modbusdriver::scheduleHandler()
{
    if (started)
        taskTimer->singleShot(1, this, &Modbusdriver::handleNextTask);
}

//------------------------------------------------------------------------------
bool Modbusdriver::compare(Modbusdriver::MBaddress a1, Modbusdriver::MBaddress a2)
{

    if (a1.devAddr > a2.devAddr)
        return true;
    else if (a1.devAddr < a2.devAddr)
        return false;
    else if (a1.regType > a2.regType)
        return true;
    else if (a1.regType < a2.regType)
        return false;
    else if (a1.regAddr > a2.regAddr)
        return true;
    else
        return false;
}

//------------------------------------------------------------------------------
void Modbusdriver::WriteRequested(Tag *tag)
{
    MBaddress address;
    if(!strToAddr(tag->address, &address)){
        emit LoggingSig(MessError, QDateTime::currentDateTime(), false, objectName(), "Modbus driver write error: address isn\'t valid");
        return;
    }

    Task * task = new Task;
    task->writeTask = true;
    task->devAddr = address.devAddr;
    task->regAddr = address.regAddr;
    task->regType = address.regType;
    task->quantity = 1;
    task->listOfTags.append(tag);


    foreach (Task * task, listOfTasks) {
        if(task->devAddr == address.devAddr && task->regAddr == address.regAddr && task->regType == address.regType && task->writeTask){
            listOfTasks.removeOne(task);
        }
    }
    listOfTasks.prepend(task);
}

//------------------------------------------------------------------------------
void Modbusdriver::initThread(QModbusClient *modbusDevice)
{
    thread = new QThread;
    thread->start();
    this->moveToThread(thread);
    taskTimer = new QTimer();
    taskTimer->moveToThread(thread);
    this->modbusDevice = modbusDevice;
    modbusDevice->moveToThread(thread);
    QObject::connect(this, &Driver::connectDriver, this, &Driver::connect, Qt::QueuedConnection);
    QObject::connect(this, &Driver::disconnectDriver, this, &Driver::disconnect, Qt::QueuedConnection);
}
//------------------------------------------------------------------------------
