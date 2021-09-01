#include "tsp.h"
#include "QFile"
#include <QTextCodec>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonObject>

//------------------------------------------------------------------------------
TSP::TSP(QObject *parent, QString name) : QObject(parent)
{
    qRegisterMetaType<tsp_enums::MessType>("MessType");
    this->setObjectName(name);
    if(QFile("./tags.json").exists())
        LoadJson();
    else if(QFile("./tagsNEW.ini").exists())
        LoadIni();
    else if(QFile("./tags.ini").exists())
        LoadIniFromOld();
}
//------------------------------------------------------------------------------
TSP::~TSP()
{
    foreach (Driver * driver, listOfDrivers) {
        delete driver;
    }
}
//------------------------------------------------------------------------------
bool TSP::AddDriver(int id, QString  name, QString type, QString options, QString comment)
{
    DriverConfig * config = new DriverConfig{id, name, type, options, comment};
    listOfDriverConfigs.append(config);
    if (name == "") {
        emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Driver creation error: name isn\'t valid"); config->errorString = "name isn\'t valid";
        return false;
    }
    if (type == "modbusTCP"){
        options = options.simplified();
        options.replace(" ", "");
        QStringList optionsList = options.split(',', QString::SkipEmptyParts);
        int timeout = 100;
        QString addr = "127.0.0.1";
        int port = 502;
        foreach (QString option, optionsList) {
            QStringList optionPart = option.split('=', QString::SkipEmptyParts);
            if (optionPart.count() == 2){
                if(optionPart[0]=="address"){
                    QStringList addressPartsList = optionPart[1].split(':', QString::SkipEmptyParts);
                    if (addressPartsList.count() == 1) {
                        addr = addressPartsList[0];
                    }else if (addressPartsList.count() == 2){
                        bool ok = false;
                        addr = addressPartsList[0];
                        port = addressPartsList[1].toInt(&ok, 10);
                        if (!ok){
                            emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Driver " + name + " creation tcp address error: port isn\'t valid"); config->errorString = "port isn\'t valid";
                            return false;
                        }
                    }else{
                        emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Driver " + name + " creation error: options isn\'t valid"); config->errorString = "options isn\'t valid";
                        return false;
                    }
                }else if(optionPart[0] == "timeout"){
                    bool ok = false;
                    timeout = optionPart[1].toInt(&ok);
                    if (!ok){
                        emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Driver " + name + " creation error: timeout isn\'t valid " + optionPart[1]); config->errorString = "timeout isn\'t valid " + optionPart[1];
                        return false;
                    }
                }else{
                    emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Driver " + name + " creation error: unknown option " + optionPart[0]); config->errorString = "unknown option " + optionPart[0];
                    return false;
                }
            }else{
                emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Driver " + name + " creation error: options aren\'t valid"); config->errorString = "options aren\'t valid";
                return false;
            }
        }
        Driver * driver = GetDriverById(id);
        if (driver){
            emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Driver " + name + " creation error: driver id isn\'t unique"); config->errorString = "driver id isn\'t unique";
            return false;
        }
        driver = new Modbusdriver(id, name, addr, port, timeout, comment);
        driver->Options = options;
        emit LoggingSig(MessVerbose, QDateTime::currentDateTime(), false, this->objectName(), "Driver " + name + " created");
        config->driver = driver; listOfDrivers.append(driver);
        QObject::connect(driver, &Driver::LoggingSig, this, &TSP::LoggingSig);
        return true;
    }else if(type == "modbusRTU"){
        options = options.simplified();
        options.replace(" ", "");
        QStringList optionsList = options.split(',', QString::SkipEmptyParts);
        QString port = "COM1";
        QString baudrate = "19200";
        QSerialPort::DataBits databits = QSerialPort::Data8;
        QSerialPort::Parity parity = QSerialPort::EvenParity;
        QSerialPort::StopBits stopbits = QSerialPort::OneStop;
        int timeout = 100;
        foreach (QString option, optionsList) {
            QStringList optionPart = option.split('=', QString::SkipEmptyParts);
            if (optionPart.count() == 2){
                if (optionPart[0] == "port"){
                    port = optionPart[1];
                }else if(optionPart[0] == "baudrate"){
                    baudrate = optionPart[1];
                }else if(optionPart[0] == "databits"){
                    if (optionPart[1] == "5")
                        databits = QSerialPort::Data5;
                    else if (optionPart[1] == "6")
                        databits = QSerialPort::Data6;
                    else if (optionPart[1] == "7")
                        databits = QSerialPort::Data7;
                    else if (optionPart[1] == "8")
                        databits = QSerialPort::Data8;
                    else{
                        emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Driver " + name + " creation error: unknown data bits " + optionPart[1]); config->errorString = "unknown data bits " + optionPart[1];
                        return false;
                    }
                }else if(optionPart[0] == "parity"){
                    if (optionPart[1] == "N")
                        parity = QSerialPort::NoParity;
                    else if (optionPart[1] == "E")
                        parity = QSerialPort::EvenParity;
                    else if (optionPart[1] == "O")
                        parity = QSerialPort::OddParity;
                    else if (optionPart[1] == "S")
                        parity = QSerialPort::SpaceParity;
                    else if (optionPart[1] == "M")
                        parity = QSerialPort::MarkParity;
                    else{
                        emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Driver " + name + " creation error: unknown parity " + optionPart[1]); config->errorString = "unknown parity " + optionPart[1];
                        return false;
                    }
                }else if(optionPart[0] == "stopbits"){
                    if (optionPart[1] == "1")
                        stopbits = QSerialPort::OneStop;
                    else if (optionPart[1] == "1.5")
                        stopbits = QSerialPort::OneAndHalfStop;
                    else if (optionPart[1] == "2")
                        stopbits = QSerialPort::TwoStop;
                    else{
                        emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Driver " + name + " creation error: unknown stop bits " + optionPart[1]); config->errorString = "unknown stop bits " + optionPart[1];
                        return false;
                    }
                }else if(optionPart[0] == "timeout"){
                    bool ok = false;
                    timeout = optionPart[0].toInt(&ok);
                    if (!ok){
                        emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Driver " + name + " creation error: timeout isn\'t valid " + optionPart[1]); config->errorString = "timeout isn\'t valid" + optionPart[1];
                        return false;
                    }
                }else{
                    emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Driver " + name + " creation error: unknown option " + optionPart[0]); config->errorString = "unknown option " + optionPart[0];
                    return false;
                }
            }else{
                emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Driver " + name + " creation error: options aren\'t valid"); config->errorString = "options aren\'t valid";
                return false;
            }
        }
        Driver * driver = GetDriverById(id);
        if (driver){
            emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Driver " + name + " creation error: driver id isn\'t unique"); config->errorString = "driver id isn\'t unique";
            return false;
        }
        driver = new Modbusdriver(id, name, port, baudrate, databits, parity, stopbits, timeout, comment);
        driver->Options = options;
        emit LoggingSig(MessVerbose, QDateTime::currentDateTime(), false, this->objectName(), "Driver " + name + " created");
        config->driver = driver; listOfDrivers.append(driver);
        QObject::connect(driver, &Driver::LoggingSig, this, &TSP::LoggingSig);
        return true;
    }
    else if(type == "simatic"){
        options = options.simplified();
        options.replace(" ", "");
        QString addr = "127.0.0.1";
        int rack = 0;
        int slot = 2;
        QStringList optionsList = options.split(',', QString::SkipEmptyParts);
        foreach (QString option, optionsList) {
            QStringList optionPart = option.split('=', QString::SkipEmptyParts);
            if (optionPart.count() == 2){
                if(optionPart[0] == "address"){
                    addr = optionPart[1];
                }else if(optionPart[0] == "rack"){
                    bool ok = false;
                    rack = optionPart[1].toInt(&ok);
                    if(!ok){
                        emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Driver " + name + " creation error: rack isn\'t valid"); config->errorString = "rack isn\'t valid";
                        return false;
                    }
                }else if(optionPart[0] == "slot"){
                    bool ok = false;
                    slot = optionPart[1].toInt(&ok);
                    if(!ok){
                        emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Driver " + name + " creation error: slot isn\'t valid"); config->errorString = "slot isn\'t valid";
                        return false;
                    }
                }else{
                    emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Driver " + name + " creation error: unknown option " + optionPart[0]); config->errorString = "unknown option " + optionPart[0];
                    return false;
                }
            }else{
                emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Driver " + name + " creation error: options aren\'t valid"); config->errorString = "options aren\'t valid";
                return false;
            }
        }
        Driver * driver = GetDriverById(id);
        if (driver){
            emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Driver " + name + " creation error: driver id isn\'t unique"); config->errorString = "driver id isn\'t unique";
            return false;
        }
        driver = new Simaticdriver(id, name, addr, rack, slot, comment);
        driver->Options = options;
        emit LoggingSig(MessVerbose, QDateTime::currentDateTime(), false, this->objectName(), "Driver " + name + " created");
        config->driver = driver; listOfDrivers.append(driver);
        QObject::connect(driver, &Driver::LoggingSig, this, &TSP::LoggingSig);
        return true;

    }
    else{
        emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Driver " + name + " creation error: type isn\'t valid"); config->errorString = "type isn\'t valid";
        return false;
    }
}
//------------------------------------------------------------------------------
bool TSP::AddGroup(int id, QString name, QString options, int delay, int driverId, QString comment)
{
    GroupConfig * config = new GroupConfig{id, name, options, delay, driverId, comment};
    listOfGroupConfigs.append(config);
    int optimizerRangeInterval = 4;
    int optimizerRangeMax = 70;
    if (name == "") {
        emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Group creation error: name isn\'t valid"); config->errorString = "name isn\'t valid";
        return false;
    }
    options = options.simplified();

    options.replace(" ", "");
    options = options.toLower();
    QStringList optionsList = options.split(',', QString::SkipEmptyParts);
    foreach (QString option, optionsList) {
        QStringList optionParts = option.split('=', QString::SkipEmptyParts);
        if (optionParts.size() != 2){
            emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Group " + name + " creation error: options aren\'t valid"); config->errorString = "options aren\'t valid";
            return false;
        }else{
            bool ok = false;
            int val =  optionParts[1].toInt(&ok, 10);
            if (!ok){
                emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Group " + name + " creation error: options aren\'t valid"); config->errorString = "options aren\'t valid";
                return false;
            }
            if (optionParts[0] == "ri"){
                optimizerRangeInterval = val;
            }else if (optionParts[0] == "rm"){
                optimizerRangeMax = val;
            }else{
                emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Group " + name + " creation error: options aren\'t valid"); config->errorString = "options aren\'t valid";
                return false;
            }
        }
    }
    Driver * driver = GetDriverById(driverId);
    if (!driver){
        emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Group " + name + " creation error: driverId isn\'t valid"); config->errorString = "driverId isn\'t valid";
        return false;
    }
    Group * group = GetGroupById(id);
    if (group){
        emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Group " + name + " creation error: group id isn\'t unique"); config->errorString = "group id isn\'t valid";
        return false;
    }
    group = new Group(id, driverId, name, delay, optimizerRangeInterval, optimizerRangeMax, comment);
    if(!driver->InsertGroup(group)){
        delete group;
        return false;
    }
    emit LoggingSig(MessVerbose, QDateTime::currentDateTime(), false, this->objectName(), "Group " + name + " created");
    group->Options = options;
    config->group = group; listOfGroups.append(group);
    return true;
}
//------------------------------------------------------------------------------
bool TSP::addTag(int id, QString name, QString address, QString options, int groupId, QString comment)
{
    TagConfig * config = new TagConfig{ id, name, address, options, groupId, comment };
    if (name == "") {
        emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Tag creation error: name isn\'t valid");
        config->errorString = "name isn\'t valid";
        return false;
    }
    Access access = RW;
    Virtualization virtualization = No;
    DataType type = TInt;
    options = options.simplified();
    options.replace(" ", "");
    options = options.toLower();
    QStringList optionsList = options.split(',', QString::SkipEmptyParts);

    foreach (QString option, optionsList) {
        if (option == "rw")
            access = RW;
        else if (option == "wo")
            access = WO;
        else if (option == "ro")
            access = RO;
        else if (option == "na")
            access = NA;
        else if (option == "virtual_value")
            virtualization = VValue;
        else if (option == "virtual_random")
            virtualization = VRandom;
        else if (option == "virtual_bittag")
            virtualization = VBitTag;
        else if (option == "virtual_dwordtag")
            virtualization = VDWordTag;
        else if (option == "bool")
            type = TBool;
        else if (option == "int")
            type = TInt;
        else if (option == "float")
            type = TFloat;
        else {
            emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Tag " + name + " creation error: unknown option");
            config->errorString = "unknown option";
            return false;
        }
    }
    if (address == "") {
        emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Tag " + name + " creation error: address isn\'t valid");
        config->errorString = "address isn\'t valid";
        return false;
    }

    Tag * tag = new Tag(id, name, address, groupId, type, access, virtualization, comment, config);
    if(addTag(tag)){
        config->id = tag->id;
        config->tag = tag;
        listOfTagConfigs.append(config);
        return true;
    }
    delete tag;
    delete config;
    return false;
}
//------------------------------------------------------------------------------
bool TSP::addTag(Tag *tag)
{
    if (!tag){
        emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Tag " + tag->objectName() + " inserting error: tag is null");
        return false;
    }
    Group * group = GetGroupById(tag->groupId);
    if (!group){
        emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Tag " + tag->objectName() + " inserting error: groupId isn\'t valid");
        return false;
    }

    if(tag->id < 1){
        tag->id = maxTagId();
    }
    Tag * twin = nullptr;
    twin = GetTagById(tag->id);
    if (twin){
        emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Tag " + tag->objectName() + " creation error: tag id isn\'t unique");
        //        twin->setError("tag id isn\'t unique"); twin->setQuality(Bad); twin->access = NA;
        tag->setError("tag id isn\'t unique");
        tag->setQuality(Bad); tag->access = NA;
    }
    twin = GetTagByName(tag->objectName());
    if (twin){
        emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Tag " + tag->objectName() + " creation error: tag name isn\'t unique");
        //        twin->setError("tag name isn\'t unique"); twin->setQuality(Bad); twin->access = NA;
        tag->setError("tag name isn\'t unique");
        tag->setQuality(Bad); tag->access = NA;
    }
    twin = GetTagByAddress(tag->groupId, tag->address);
    if (twin){
        emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Tag " + tag->objectName() + " creation error: tag address isn\'t unique");
        //        twin->setError("tag address isn\'t unique"); twin->setQuality(Bad); twin->access = NA;
        tag->setError("tag address isn\'t unique");
        tag->setQuality(Bad); tag->access = NA;
    }
    if(tag->access != NA && tag->access != WO && !twin){
        if(!group->InsertTag(tag)){
            return false;
        }
    }
    emit LoggingSig(MessVerbose, QDateTime::currentDateTime(), false, this->objectName(), "Tag " + tag->objectName() + " created" + (twin ? ", but the error occured" : "") );
    emit LoggingSig(MessVerbose, QDateTime::currentDateTime(), false, this->objectName(), "Tag " + tag->objectName() + " added");
    listOfTags.append(tag);
    return true;
}
//------------------------------------------------------------------------------
void TSP::Init()
{

}
//------------------------------------------------------------------------------
void TSP::Start()
{
    foreach (Group * group, listOfGroups) {
        emit group->onTagInserted(group);
    }
    foreach (Driver * driver, listOfDrivers) {
        emit driver->connectDriver();
    }
}
//------------------------------------------------------------------------------

void TSP::Stop()
{
    foreach (Driver * driver, listOfDrivers) {
        emit driver->disconnectDriver();
    }
}
//------------------------------------------------------------------------------
Driver *TSP::GetDriverById(int id)
{
    foreach (Driver * thisDriver, listOfDrivers) {
        if(thisDriver->Id == id){
            return thisDriver;
        }
    }
    return nullptr;
}
//------------------------------------------------------------------------------
Group *TSP::GetGroupById(int id)
{
    foreach (Group * thisGroup, listOfGroups) {
        if(thisGroup->Id == id){
            return thisGroup;
        }
    }
    return nullptr;
}
//------------------------------------------------------------------------------

int TSP::maxTagId()
{
    static int maxId;
    maxId = 1;
    foreach (Tag * thisTag, listOfTags) {
        if(thisTag->id >= maxId){
            maxId ++;
        }
    }
    return maxId;
}
//------------------------------------------------------------------------------
int TSP::nextTagId(int preId)
{
    do preId ++;
    while (GetTagById(preId) != nullptr);
    return preId;
}
//------------------------------------------------------------------------------
Tag *TSP::GetTagById(int id)
{
    foreach (Tag * thisTag, listOfTags) {
        if(thisTag->id == id){
            return thisTag;
        }
    }
    return nullptr;
}
//------------------------------------------------------------------------------
Tag *TSP::GetTagByName(QString name)
{
    foreach (Tag * thisTag, listOfTags) {
        if(thisTag->objectName() == name){
            return thisTag;
        }
    }
    return nullptr;
}
//------------------------------------------------------------------------------
Tag *TSP::GetTagByAddress(int groupId, QString addr)
{
    foreach (Tag * thisTag, listOfTags) {
        if(thisTag->address == addr && thisTag->groupId == groupId){
            return thisTag;
        }
    }
    return nullptr;
}
//------------------------------------------------------------------------------
void TSP::SaveIni(QString FileName)
{
    //if(ini != nullptr)delete ini;
    //QTextCodec *codec = QTextCodec::codecForName("Windows-1251");

    ini = new QSettings(FileName, QSettings::IniFormat);
    ini->setIniCodec(QTextCodec::codecForName("UTF-8"));
    ini->setValue("Driver", "Id, Name, Type, Options, Comment");
    ini->beginWriteArray("Drivers", listOfDrivers.count());
    for(int i = 0; i < listOfDrivers.count(); i ++){
        ini->setArrayIndex(i);
        ini->setValue("Driver", QString::number(listOfDrivers[ i ]->Id) + "~"
                       + listOfDrivers[ i ]->objectName() + "~"
                       + listOfDrivers[ i ]->type + "~"
                       + listOfDrivers[ i ]->Options + "~"
                       + listOfDrivers[ i ]->Comment);
    }
    ini->endArray();

    ini->setValue("Group", "Id, DriverId, Name, Delay, Options, Comment");
    ini->beginWriteArray("Groups", listOfGroups.count());
    for(int i = 0; i < listOfGroups.count(); i ++){
        ini->setArrayIndex(i);
        ini->setValue("Group", QString::number(listOfGroups[ i ]->Id) + "~"
                       + listOfGroups[ i ]->objectName() + "~"
                       + listOfGroups[ i ]->Options + "~"
                       + QString::number(listOfGroups[ i ]->Delay) + "~"
                       + QString::number(listOfGroups[ i ]->DriverId) + "~"
                       + listOfGroups[ i ]->Comment);
    }
    ini->endArray();

    ini->setValue("Tag", "Name, Address, GroupId, Options, Comment");
    ini->beginWriteArray("Tags", listOfTags.count());
    for(int i = 0; i < listOfTags.count(); i ++){
        ini->setArrayIndex(i);
        ini->setValue("Tag",    listOfTags[ i ]->objectName() + "~"
                       + listOfTags[ i ]->address + "~"
                       + listOfTags[ i ]->options + "~"
                       + QString::number(listOfTags[ i ]->groupId) + "~"
                       + listOfTags[ i ]->comment);
    }
    ini->endArray();
    ini->sync();
    delete ini;
}
//------------------------------------------------------------------------------
void TSP::LoadJson(QString FileName)
{
    QFile loadFile(FileName);
    if (!loadFile.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QJsonObject json = (QJsonDocument::fromJson(loadFile.readAll())).object();
    loadFile.close();
    if (json.contains("drivers") && json["drivers"].isArray()){
        QJsonArray drivers = json["drivers"].toArray();
        foreach (QJsonValue valdriver, drivers) {
            if (valdriver.isObject()){
                QJsonObject driver = valdriver.toObject();
                AddDriver(driver["id"].toInt(),
                        driver["name"].toString(),
                        driver["type"].toString(),
                        driver["options"].toString(),
                        driver["comment"].toString());
            }
        }
    }
    if (json.contains("groups") && json["groups"].isArray()){
        QJsonArray groups = json["groups"].toArray();
        foreach (QJsonValue valgroup, groups) {
            if (valgroup.isObject()){
                QJsonObject group = valgroup.toObject();
                AddGroup(group["id"].toInt(),
                        group["name"].toString(),
                        group["options"].toString(),
                        group["delay"].toInt(),
                        group["driverId"].toInt(),
                        group["comment"].toString());
            }
        }
    }
    if (json.contains("tags") && json["tags"].isArray()){
        QJsonArray tags = json["tags"].toArray();
        foreach (QJsonValue valtag, tags) {
            if (valtag.isObject()){
                QJsonObject tag = valtag.toObject();
                addTag(tag["id"].toInt(),
                        tag["name"].toString(),
                        tag["address"].toString(),
                        tag["options"].toString(),
                        tag["groupId"].toInt(),
                        tag["comment"].toString());
            }
        }
    }
}
//------------------------------------------------------------------------------
void TSP::SaveJson(QString FileName)
{
    QFile saveFile(FileName);
    if (!saveFile.open(QIODevice::WriteOnly)) return;
    QJsonObject json;
    QJsonArray drivers;
    QJsonArray groups;
    QJsonArray tags;
    for(int i = 0; i < listOfDriverConfigs.count(); i ++){
        QJsonObject driver;
        driver["id"] = listOfDriverConfigs[ i ]->id;
        driver["name"] = listOfDriverConfigs[ i ]->name;
        driver["type"] = listOfDriverConfigs[ i ]->type;
        driver["options"] = listOfDriverConfigs[ i ]->options;
        driver["comment"] = listOfDriverConfigs[ i ]->comment;
        drivers.append(driver);
    }
    for(int i = 0; i < listOfGroupConfigs.count(); i ++){
        QJsonObject group;
        group["id"] = listOfGroupConfigs[ i ]->id;
        group["name"] = listOfGroupConfigs[ i ]->name;
        group["options"] = listOfGroupConfigs[ i ]->options;
        group["delay"] = listOfGroupConfigs[ i ]->delay;
        group["driverId"] = listOfGroupConfigs[ i ]->driverId;
        group["comment"] = listOfGroupConfigs[ i ]->comment;
        groups.append(group);
    }

    for(int i = 0; i < listOfTagConfigs.count(); i ++){
        QJsonObject tag;
        tag["id"] = listOfTagConfigs[ i ]->id;
        tag["name"] = listOfTagConfigs[ i ]->name;
        tag["address"] = listOfTagConfigs[ i ]->address;
        tag["options"] = listOfTagConfigs[ i ]->options;
        tag["groupId"] = listOfTagConfigs[ i ]->groupId;
        tag["comment"] = listOfTagConfigs[ i ]->comment;
        tags.append(tag);
    }
    json["drivers"] = drivers;
    json["groups"] = groups;
    json["tags"] = tags;
    QJsonDocument saveDoc(json);
    saveFile.write(saveDoc.toJson());
}

//------------------------------------------------------------------------------

void TSP::LoadIni(QString FileName)
{
    ini = new QSettings(FileName, QSettings::IniFormat);
    ini->setIniCodec(QTextCodec::codecForName("UTF-8"));
    ini->sync();
    QString TMP;
    int size = ini->beginReadArray("Drivers");

    for(int i = 0; i < size; i++){
        ini->setArrayIndex(i);
        TMP =  ini->value("Driver").toString();

        AddDriver(TMP.section('~',0,0).toUInt(),
                   TMP.section('~',1,1),
                   TMP.section('~',2,2),
                   TMP.section('~',3,3),
                   TMP.section('~',4,4));
    }
    ini->endArray();

    size = ini->beginReadArray("Groups");

    for(int i = 0; i < size; i++){
        ini->setArrayIndex(i);
        TMP =  ini->value("Group").toString();

        AddGroup(TMP.section('~',0,0).toUInt(),
                  TMP.section('~',1,1),
                  TMP.section('~',2,2),
                  TMP.section('~',3,3).toUInt(),
                  TMP.section('~',4,4).toUInt(),
                  TMP.section('~',5,5));

    }
    ini->endArray();

    size = ini->beginReadArray("Tags");

    for(int i = 0; i < size; i++){
        ini->setArrayIndex(i);
        TMP =  ini->value("Tag").toString();

        addTag(-1,
                TMP.section('~',0,0),
                TMP.section('~',1,1),
                TMP.section('~',2,2),
                TMP.section('~',3,3).toUInt(),
                TMP.section('~',4,4));
    }

    delete ini;
}

//------------------------------------------------------------------------------
void TSP::LoadIniFromOld(QString FileName)
{

    ini = new QSettings(FileName.isEmpty() ? "tags.ini" : FileName, QSettings::IniFormat);
    int size = ini->beginReadArray("Drivers");

    for(int i = 0; i < size; i++){
        ini->setArrayIndex(i);
        AddDriver(ini->value("Id").toInt(),
                   ini->value("Name").toString(),
                   driverTypeFromInt(ini->value("Type").toInt()),
                   ini->value("Options").toString(),
                   ini->value("Comment").toString());
    }
    ini->endArray();
    //    }

    //    if(ini->contains("Groups")){
    size = ini->beginReadArray("Groups");

    for(int i = 0; i < size; i++){
        ini->setArrayIndex(i);
        AddGroup(ini->value("Id").toInt(),
                  ini->value("Name").toString(),
                  "ri=" + ini->value("OptimizerRangeInterval").toString()
                  + ", rm=" + ini->value("OptimizerRangeMax").toString(),
                  ini->value("Delay").toInt(),
                  ini->value("DriverId").toInt(),
                  ini->value("Comment").toString());
    }
    ini->endArray();
    //    }

    //    if(ini->contains("Tags")){
    size = ini->beginReadArray("Tags");

    for(int i = 0; i < size; i++){
        ini->setArrayIndex(i);

        addTag(i,
                ini->value("Name").toString(),
                ini->value("Address").toString(),
                dataTypeFromInt(ini->value("Type").toInt()),
                ini->value("GroupId").toInt(),
                ini->value("Comment").toString());

    }
    ini->endArray();
    //    }

    delete ini;
}
//------------------------------------------------------------------------------
