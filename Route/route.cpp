#include <QSettings>
#include <QTimer>
#include <QtDebug>

#include "route.h"
#include "RouteUnit.h"

//------------------------------------------------------------------------------
Route::Route(PromObject * Owner, int ID, QString Name)
    : QObject(), owner(Owner), ID(ID)
{
    setObjectName(Name);
    _sTimer = new QTimerExt(this);
    _sTimer->setInterval(4000);
    _sTimer->setSingleShot(true);
    connect(_sTimer, &QTimer::timeout, this, &Route::_StartTimerSlot, Qt::QueuedConnection);
    connect(this,  &Route::Created     ,    owner, &PromObject::s_routeCreated    , Qt::QueuedConnection);
    connect(this,  &Route::StartCommand,    owner, &PromObject::s_routeStart      , Qt::QueuedConnection);
    connect(this,  &Route::Started     ,    owner, &PromObject::s_routeStarted    , Qt::QueuedConnection);
    connect(this,  &Route::PartStarted ,    owner, &PromObject::s_routePartStarted, Qt::QueuedConnection);
    connect(this,  &Route::CleanCommand,    owner, &PromObject::s_routeClean      , Qt::QueuedConnection);
    connect(this,  &Route::StopCommand ,    owner, &PromObject::s_routeStop       , Qt::QueuedConnection);
    connect(this,  &Route::Stoped      ,    owner, &PromObject::s_routeStoped     , Qt::QueuedConnection);
    connect(this,  &Route::CleanCommand,    owner, &PromObject::s_routeCleaning   , Qt::QueuedConnection);

    connect(this, &Route::NameUpdate,    owner, &PromObject::s_routeNameUpdate, Qt::QueuedConnection);
    connect(this, &Route::UnitUpdate,    owner, &PromObject::s_routeUnitUpdate, Qt::QueuedConnection);

    connect(owner, &PromObject::s_globalRescanSig, this, &Route::RescanRoute, Qt::QueuedConnection);
    connect(this,  &Route::Logging, owner, &PromObject::Logging, Qt::QueuedConnection);

    connect(this, &Route::SetNameSig     , this, &Route::SetName);
    connect(this, &Route::StartSig     , this, &Route::Start   );
    connect(this, &Route::StopSig      , this, &Route::Stop    );
    connect(this, &Route::FreezeSig    , this, &Route::Freeze  );
    connect(this, &Route::CleanStopSig , this, &Route::CleanStop);
    connect(this, &Route::CleanUnitListSig    , this, &Route::CleanUnitList    );
    connect(this, &Route::DeleteLastUnitSig   , this, &Route::DeleteLastUnit   );
    connect(this, &Route::DisconnectAllUnitSig, this, &Route::DisconnectAllUnit);

    Load();

    if(objectName() == "") {
        setObjectName("Маршрут №" + QString::number(ID));
        emit NameUpdate ( ID , objectName());
    }
    emit Logging  (Prom::MessChangeState, QDateTime::currentDateTime(), true, objectName(), "создан");
}

//------------------------------------------------------------------------------
Route::~Route()
{
    DisconnectAllUnit();
    emit Logging  (Prom::MessChangeState, QDateTime::currentDateTime(), true, objectName(), "удален");
}

//------------------------------------------------------------------------------
void Route::Start()
{
    emit Logging  (Prom::MessChangeState, QDateTime::currentDateTime(), true, objectName(), "старт маршрута");
    _queueCmds.clear();
    _workCmds.clear();
    SetCommand({ Prom::RtCmToRoute, DownStepByStep, true });
}

//------------------------------------------------------------------------------
void Route::Stop()
{
    if(_genCom != nullptr && _genCom->name == Prom::RtCmToSave )
    {
        emit Logging  (Prom::MessChangeCommand, QDateTime::currentDateTime(), true, objectName(), "аварийно остановить маршрут");
        _workCmds.clear();
        _queueCmds.clear();
        SetCommand({ Prom::RtCmStop, Parallel, false });
    }
    else{
        emit Logging  (Prom::MessChangeCommand, QDateTime::currentDateTime(), true, objectName(), "остановить маршрут");
        _queueCmds.clear();
        _workCmds.clear();
        SetCommand({ Prom::RtCmToSave, UpStepByStep, false }, true);
    }
}

//------------------------------------------------------------------------------
void Route::CleanStop()
{
    emit Logging  (Prom::MessChangeCommand, QDateTime::currentDateTime(), true, objectName(), "зачистить маршрут");
    _workCmds.clear();
    _queueCmds.clear();
    _queueCmds.append({ Prom::RtCmCleanStop, UpStepByStep, false, -1, -1 });
    SetCommand({ Prom::RtCmToRoutForClean, DownStepByStep, true });
}

//------------------------------------------------------------------------------
void Route::Freeze()
{
    emit Logging  (Prom::MessChangeCommand, QDateTime::currentDateTime(), true, objectName(), "аварийный стоп маршрута");
    _queueCmds.clear();
    _workCmds.clear();
    SetCommand({ Prom::RtCmStop, Parallel }, true);
}

//------------------------------------------------------------------------------
void Route::SetCommand(RouteCmd Command, bool GnrlCm)
{
    
    if(_workCmds.isEmpty())_genCom = nullptr;
    if(! _allUnitsConnected) _ConnectAllUnits();
    if(_allUnitsConnected){
        emit Active(ID);

        if(Command.beginUn == -1) Command.beginUn = 0;
        if(Command.endUn == -1) Command.endUn = _unitList.size() - 1;
        if(Command.type == DownStepByStep)
            Command.last = Command.endUn + 1;
        else
            Command.last = Command.beginUn - 1;

        _workCmds.append(Command);
        if(_genCom == nullptr || GnrlCm)
            _genCom = & _workCmds.last();

        if(_workCmds.last().type == Parallel){//запуск параллельной команды

            for (int i = _workCmds.last().beginUn ; i <= _workCmds.last().endUn ; i ++) {
                _CommandToUnit(_unitList[ i ], _workCmds.last().name);
                _workCmds.last().ascCount ++;
            }
        }
        else{//запуск пошаговой команды
            _CommandToUnit(_unitList[ _workCmds.last().next() ], _workCmds.last().name);
            _workCmds.last().ascCount ++;
        }
        RescanRoute();
    }
}

//------------------------------------------------------------------------------
void Route::_StartTimerSlot()
{
    //    if(_command.name && ! _command.done) InformFromUnit(_unitList[ _command->_lastCommand ]);
}
//------------------------------------------------------------------------------
void Route::_CommandToUnit(RouteUnit * RUnit, Prom::RouteCommand Command)
{
    emit Logging  (Prom::MessInfo, QDateTime::currentDateTime(), true, objectName(), "команда от маршрута - " + objectName() +
                    " юниту - " + RUnit->objectName() + QString::number(Command));
    emit RUnit->CommandFromRouteSig(Command);
}

//------------------------------------------------------------------------------
void Route::InformFromUnit(RouteUnit * RUnit, Prom::RouteCommand ResponseCommand, Prom::RouteUnitInfo UnitInfo)
{
    emit Logging  (Prom::MessInfo, QDateTime::currentDateTime(), true, objectName(), "обработка маршрутом изменения состояния " + RUnit->objectName() +
                    Prom::modeToString(RUnit->currentMode()));
    static int unitN;
    static int i;
    static RouteCmd * command;
    unitN = _unitList.indexOf(RUnit);
    if(unitN == -1){
        emit Logging  (Prom::MessAlarm, QDateTime::currentDateTime(), true, objectName(), "к маршруту подсоединен не учавствующий в нем юнит " + RUnit->objectName());
        return;
    }
    //command = nullptr;
    for (i = 0; i < _workCmds.size(); i ++) {//+перебор всех команд для обработки сообщений от юнита

        if(_workCmds[ i ].beginUn <= unitN && unitN <= _workCmds[ i ].endUn){///------------------------------+ юнит из области действия команды

            command = &(_workCmds[ i ]);
            if( command->type == Parallel){                        ///++--------------------------++ параллельная команда
                command->respCount++;
                command->last = unitN;

                if(ResponseCommand !=  command->name){
                    switch ( command->name) {
                    case Prom::RtCmNo:;break;
                    case Prom::RtCmToRoutForClean:{
                        if(unitN !=  command->last){
                            command->good = false;
                            if(unitN > 0)
                                SetCommand({ Prom::RtCmToSave, Parallel, false,  command->last , unitN - 1 });
                            command->last = unitN + 1;
                        }
                        else{
                            command->last ++;
                        }

                    };
                        break;
                    case Prom::RtCmCleanStop:{
                    };
                        break;
                    case Prom::RtCmToRoute:{
                        command->good = false;
                        command->workAftFin = true;
                        command->finCmd = true;
                        if(unitN > 0)
                            SetCommand({ Prom::RtCmToSave, Parallel, false,  command->beginUn , unitN - 1 });
                        command->last = unitN + 1;
                    };break;
                    default:;
                    }
                    ///Обработка отказа
                    command->good = false;
                }
                if( command->respCount ==  command->count()){
                    command->finCmd = true;
                }
            }
            else {                                                                             ///++--------------------------++ пошаговая команда
                if( command->type == UpStepByStep ? unitN <=  command->last : unitN >=  command->last){//----------------+от охваченного командой
                    if(ResponseCommand !=  command->name){//выход из команды

                        ///Обработка выхода из команды охваченного
                        switch ( command->name) {
                        case Prom::RtCmNo:
                            ;break;
                        case Prom::RtCmToRoutForClean:{
                            if(unitN !=  command->last){
                                command->good = false;
                                if(unitN > 0)
                                    SetCommand({ Prom::RtCmStopOnRouteAlarm, Parallel, false,  command->last , unitN - 1 });
                                command->last = unitN + 1;
                            }
                            else{
                                command->last ++;
                            }

                        };break;
                        case Prom::RtCmCleanStop:
                            command->workAftFin = true;
                            command->finCmd = true;
                            _CommandToUnit(_unitList[ unitN ], Prom::RtCmStop);
                            break;
                        case Prom::RtCmToRoute:{ //+++++++ обработка выходов из RtCmToRoute
                            switch (RUnit->unitType) {
                            case Prom::TypeSilos:
                                command->good = true;
                                command->workAftFin = false;
                                command->finCmd = true;
                                if((unitN - command->last) > 0){
                                    SetCommand({ Prom::RtCmCleanStop, UpStepByStep, false,  command->beginUn , unitN - 1 });
                                }
                                command->last = unitN + 1;
                                break;
                            case Prom::TypeDryer1_Bunker:
                            case Prom::TypeDryer3_Bunker:
                                if(UnitInfo == Prom::RtUnOutCycle)
                                    goto def;
                                command->good = true;
                                command->workAftFin = false;
                                command->finCmd = true;
                                _queueCmds.clear();
                                SetCommand({ Prom::RtCmToRoutForClean, DownStepByStep, true, command->beginUn, unitN - 1 }, true);
                                _queueCmds.append({ Prom::RtCmCleanStop, UpStepByStep, false, command->beginUn, unitN - 1 });
                                _queueCmds.append({ Prom::RtCmToRoute, DownStepByStep, true, command->beginUn, unitN });
                                break;
                            default:{
                            def:
                                command->good = false;
                                command->workAftFin = true;
                                command->finCmd = true;
                                if((unitN - command->last) > 0){
                                    SetCommand({ Prom::RtCmStopOnRouteAlarm, Parallel, true,  command->beginUn , unitN - 1 });
                                }
                                command->last = unitN + 1;
                            }
                                break;
                            }
                        }//---- обработка выходов из RtCmToRoute
                            break;
                        case Prom::RtCmStop:;
                        case Prom::RtCmToSave:{
                            _CommandToUnit(_unitList[ unitN ], Prom::RtCmStop);
                        }
                        default:;///!!!
                        }
                        if(command->count() < 2){
                            command->finCmd = true;
                            command->workAftFin = false;
                        }
                    }
                    if(UnitInfo != Prom::RtUnNo){//есть инфо от юнита
                        /// Обработка сообщения от юнита охваченного командой
                    }

                }//-от охваченного командой
                else if( command->type == UpStepByStep ? unitN >  command->last : unitN <  command->last){ ///----------------+от НЕ охваченного командой                                                               //-----------от еще НЕохваченного командой
                    command->respCount ++;
                    if(unitN ==  command->next()){                                                //----от следующего
                        if( ResponseCommand ==  command->name){                                   //+--- команда исполнена
                            command->good = true;
                            command->last = command->next();
                            //command->finCmd = false;
                            if( command->type == UpStepByStep ?
                                 command->last >=  command->endUn
                                 :  command->last <=  command->beginUn){ //последний в маршруте
                                ///Завершение выполненной команды
                                command->finCmd = true;
                            }
                            else {                                                                   //НЕ последний в маршруте и команда движется
                                //if(! _command->finCmd){
                                _CommandToUnit(_unitList[  command->next() ],  command->name);
                                command->ascCount ++;
                                //}
                            }
                            if(UnitInfo != Prom::RtUnNo){                                         //есть инфо от юнита
                                /// Обработка сообщения от следующего исполнявшего
                            }
                        }                                                                           //--- команда исполнена
                        else{                                                                      //+--- команда НЕ исполнена
                            switch (command->name) {
                            case Prom::RtCmToRoutForClean :;break;
                            case Prom::RtCmStop :{
                                command->good = false;
                                command->last = command->next();
                                if( command->type == UpStepByStep ?
                                     command->last >=  command->endUn
                                     :  command->last <=  command->beginUn){ //последний в маршруте
                                    ///Завершение выполненной команды
                                    command->finCmd = true;
                                }
                                else {                                                                   //НЕ последний в маршруте и команда движется
                                    //if(! _command->finCmd){
                                    _CommandToUnit(_unitList[  command->next() ],  command->name);
                                    command->ascCount ++;
                                    //}
                                }
                            }
                            default:{
                                command->good = false;
                            }
                            }
                            command->finCmd = true;
                            command->ascCount --;

                            if(command->type == UpStepByStep)
                                command->endUn = command->last;
                            else
                                command->beginUn = command->last;

                            if(UnitInfo != Prom::RtUnNo){
                                ///Обработка сообщения при невыполнении команды
                            }
                        }                                                                           //--- команда НЕ исполнена
                    }
                    else{                                                                           //+---не от следующего
                        if(UnitInfo != Prom::RtUnNo){
                            /// Обработка сообщения для НЕохваченных командой
                        }
                    }                                                                               //----не от следующего
                }
            }
        }
    }//-перебор всех команда для обработки сообщений от юнита

    //Удаление выполненных команд
    for (i-- ; i >= 0 ; i--) {
        if( & _workCmds[i] != _genCom && command->finCmd){
            if(! _workCmds[i].workAftFin){
                _workCmds.removeAt(i);
            }
        }
    }

    if(_genCom != nullptr && _genCom->finCmd){
        if(_queueCmds.isEmpty()){
            if(! _genCom->workAftFin){
                _workCmds.clear();
                _queueCmds.clear();
                _genCom = nullptr;
                DisconnectAllUnit();
                emit NotActive(ID);
            }
        }
        else if(_genCom->good){
            do {
                if(_queueCmds.first().name == Prom::RtCmClearCmList){
                    _workCmds.clear();
                    _queueCmds.removeAt(0);
                }
                else {
//                    if(_queueCmds.first().beginUn == -1)
//                        _queueCmds.first().beginUn = _genCom->beginUn;
//                    if(_queueCmds.first().endUn == -1)
//                        _queueCmds.first().endUn = _genCom->endUn;
                    _workCmds.clear();
                    _genCom = nullptr;
                    SetCommand(_queueCmds.takeFirst(), true);
                    break;
                }
            }
            while(! _queueCmds.isEmpty());
        }
    }
    RescanRoute();
}

//------------------------------------------------------------------------------
void Route::RescanRoute()
{
    if(_workCmds.count() > 0)
        emit Started(ID);
    else
        emit Stoped (ID);
//    if(_genCom != nullptr)
//        switch (_genCom->name) {
//        case Prom::RtCmToRoute : {
//            if(_genCom->done())
//
//            else
//                emit StartCommand(ID);
//        };break;
//        case Prom::RtCmStop :{
//            if(_genCom->finCmd)
//                emit StopCommand(ID);
//            else
//                emit Stoped (ID);
//        };break;
//        case Prom::RtCmToSave :{
//            if(_genCom->done())
//                emit StopCommand(ID);
//            else
//                emit Stoped (ID);
//        };break;
//        case Prom::RtCmCleanStop :{
//            if(_genCom->done())
//                emit CleanCommand(ID);
//            else
//                emit Stoped (ID);
//        };break;
//        case Prom::RtCmNo : emit NotActive(ID);
//        default:;
//        }
}
//------------------------------------------------------------------------------
QString Route::_UnitsString()
{
    QString tmpS = "";
    for(int i = 0 ; i < _unitList.count(); i ++) {
        tmpS += "->" +_unitList [ i ]->objectName();
        switch (_unitList [ i ]->unitType) {
        case Prom::TypeGate:
        case Prom::TypeUnderGate:
            tmpS += _unitMode[ i ] == Prom::UnMd1pos ? "(от)" : "(зк)";
            break;
        case Prom::TypeValve:
            tmpS += _unitMode[ i ] == Prom::UnMd1pos ? "(лв)" : "(пр)";
            break;
        default:
            if(_unitMode[ i ] == Prom::UnMdStop) tmpS += "(стоп)";
            break;
        }
    }
    tmpS.remove(0, 2);
    return tmpS;
}

//------------------------------------------------------------------------------
void Route::CleanUnitList()
{
    //qDebug()<<"CleanUnitList() in Rout";
    DisconnectAllUnit();
    _unitList.clear();
    _unitMode.clear();
    emit UnitUpdate(ID ,  _UnitsString());
}

//------------------------------------------------------------------------------
void Route::DeleteLastUnit()
{
    RouteUnit * tmp = _unitList.takeAt(_unitList.count() - 1);
    DisconnectUnit(tmp);
    _unitMode.removeLast();
    Save();
    emit Logging  (Prom::MessChangeCommand, QDateTime::currentDateTime(), true, objectName(), "удаление юнита " + tmp->objectName());
    //ConnectUnit(RUnit, false); // мягкое подключение
    emit UnitUpdate(ID ,  _UnitsString());
}

//------------------------------------------------------------------------------
void Route::SetAsCurrent()
{
    emit owner->s_setCurrentRouteSig(this);
}

//------------------------------------------------------------------------------
bool Route::CheckConnectAllUnits()
{
    bool check = true;
    for(int i = 0; i < _unitList.count(); i ++) {
        check = check &&(_unitList[i]->MyRoute() == this);
    }
    return check;
}

//------------------------------------------------------------------------------
Prom::UnitModes Route::UnitRouteMode(RouteUnit *RUnit)
{
    if(_unitList.contains(RUnit))
        return _unitMode[ _unitList.indexOf(RUnit) ];
    else return Prom::UnMdNoDef;
}

//------------------------------------------------------------------------------
bool Route::SetUnitRouteMode(RouteUnit *RUnit, Prom::UnitModes mode)
{
    if(_unitList.contains(RUnit)){
        _unitMode[ _unitList.indexOf(RUnit) ] = mode;
        return true;
    }
    else return false;
}
//------------------------------------------------------------------------------
void Route::SetName(QString name)
{
    setObjectName(name);
    Save();
    emit NameUpdate ( ID , name);
}

//------------------------------------------------------------------------------
//bool Route::_ChekComm( unsigned int Ind )
//{
//    if(_pullOfCmd[ Ind ].count() < 0) return false;
//    _pullOfCmd[ Ind ].finCmd = false;
//    _pullOfCmd[ Ind ].good = true;
//    if(_pullOfCmd[ Ind ].type == UpStepByStep){
//        if(_pullOfCmd[ Ind ].last < _pullOfCmd[ Ind ].beginUn){
//            _pullOfCmd[ Ind ].last = _pullOfCmd[ Ind ].beginUn - 1;
//            _CommandToUnit(_unitList[ _pullOfCmd[ Ind ].next() ], _pullOfCmd[ Ind ].name);
//            _pullOfCmd[ Ind ].ascCount = 1;
//            _pullOfCmd[ Ind ].respCount = 0;
//        }
//        else {
//            _pullOfCmd[ Ind ].respCount -= _pullOfCmd[ Ind ].beginUn;
//            _pullOfCmd[ Ind ].ascCount  -= _pullOfCmd[ Ind ].beginUn;
//        }
//    }
//    else if (_pullOfCmd[ Ind ].type == DownStepByStep) {
//        if(_pullOfCmd[ Ind ].last > _pullOfCmd[ Ind ].endUn){
//            _pullOfCmd[ Ind ].last = _pullOfCmd[ Ind ].endUn + 1;
//            _CommandToUnit(_unitList[ _pullOfCmd[ Ind ].next() ], _pullOfCmd[ Ind ].name);
//            _pullOfCmd[ Ind ].ascCount = 1;
//            _pullOfCmd[ Ind ].respCount = 0;
//        }
//        else{
//            _pullOfCmd[ Ind ].respCount -= _pullOfCmd[ Ind ].beginUn;
//            _pullOfCmd[ Ind ].ascCount  -= _pullOfCmd[ Ind ].beginUn;
//        }
//    return true;
//}

//------------------------------------------------------------------------------
void Route::DisconnectAllUnit()
{
    for(int i = 0; i < _unitList.count(); i ++) {
        DisconnectUnit(_unitList[i]);
    }
    RescanRoute();
}

//------------------------------------------------------------------------------
bool Route::_ConnectAllUnits()
{
    bool res = true;
    for(int i = 0; i < _unitList.count(); i ++) {
        res &= _ConnectUnit(_unitList[i]);
    }
    if(! res) {
        DisconnectAllUnit();
    }
    _allUnitsConnected = res;
    return res;
}

//------------------------------------------------------------------------------
bool Route::DisconnectUnit(RouteUnit * RUnit)
{
    if(RUnit->MyRoute() == this){
        disconnect(RUnit, &RouteUnit::InformToRoute, this, &Route::InformFromUnit);
        RUnit->SetMyRoute(nullptr,  Prom::UnMdNoDef);
        _CheckConnection();
        return true;
    }
    else {
        emit Logging  (Prom::MessAlarm, QDateTime::currentDateTime(), true, objectName(), "попытка отсоединить не входящий в маршрут " + RUnit->objectName());
        return true;

    }
}

//------------------------------------------------------------------------------
bool Route::_ConnectUnit(RouteUnit * RUnit)
{
    if(RUnit->MyRoute() == this) return true;
    if(RUnit->MyRoute() == nullptr) {
        if(RUnit->SetMyRoute(this,  UnitRouteMode(RUnit))){
            //            bool b = disconnect(RUnit, &RouteUnit::InformToRoute, this, &Route::InformFromUnit);
            //            qDebug() << this->objectName() << RUnit->objectName() << b;
            connect(RUnit, &RouteUnit::InformToRoute, this, &Route::InformFromUnit, Qt::QueuedConnection);
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
void Route::_CheckConnection()
{
    _allUnitsConnected = true;
    for(int i = 0 ; i < _unitList.count(); i ++) {
        _allUnitsConnected &= _unitList[i]->MyRoute() == this;
    }
}

//------------------------------------------------------------------------------
void Route::AddUnit(RouteUnit * RUnit, Prom::UnitModes mode)
{
    if(! _unitList.contains(RUnit)){
        _unitList.append(RUnit);
        _unitMode.append(mode);
        emit Logging  (Prom::MessInfo, QDateTime::currentDateTime(), true, objectName(), "добавление юнита " + RUnit->objectName());
        Save();
        emit UnitUpdate(ID ,  _UnitsString());
    }
    //qDebug()<<"Add RUnit in Route" << RUnit->objectName();
}

//------------------------------------------------------------------------------
bool Route::DelUnit(RouteUnit *RUnit)
{
    if(! _unitList.contains(RUnit)){

        if(DisconnectUnit(RUnit)) {
            for(int i = 0; i < _unitList.count(); i ++) {
                if(_unitList[i] == RUnit) {
                    _unitList.removeAt(i);
                    _unitMode.removeAt(i);
                }
            }
            emit UnitUpdate(ID , _UnitsString());
            return true;
        }
        else return false;
    }
    else return true;
}

//------------------------------------------------------------------------------
void Route::Save()
{
    owner->iniRoute->beginGroup(QString::number(ID));
    owner->iniRoute->setValue("Name", objectName());
    owner->iniRoute->beginWriteArray("Units", _unitList.count());
    for(int i = 0; i < _unitList.count(); i ++){
        owner->iniRoute->setArrayIndex(i);
        owner->iniRoute->setValue("Name", _unitList[ i ]->objectName());
        owner->iniRoute->setValue("Mode", _unitMode[ i ]);
    }
    owner->iniRoute->endArray();
    owner->iniRoute->endGroup();
    //owner->iniRoute->sync();
}

//------------------------------------------------------------------------------
void Route::Load()
{
    if(owner->iniRoute->contains(QString::number(ID) + "/Name")){
        owner->iniRoute->beginGroup(QString::number(ID));
        setObjectName(owner->iniRoute->value("Name").toString());
        emit NameUpdate ( ID , objectName());
        int size = owner->iniRoute->beginReadArray("Units");

        for(int i = 0; i < size; i++){
            owner->iniRoute->setArrayIndex(i);
//                        QString S = owner->iniRoute->value("Name").toString();
//                        Unit * un = owner->UnitByName(owner->iniRoute->value("Name").toString());
//                        int I  = owner->iniRoute->value("Mode").toInt();
//                        Prom::UnitModes UnMd = Prom::unitModeFromInt(owner->iniRoute->value("Mode").toInt());

            if(! owner->UnitByName(owner->iniRoute->value("Name").toString())
                    || Prom::unitModeFromInt(owner->iniRoute->value("Mode").toInt()) == Prom::UnMdNoDef
                    || _unitList.contains(dynamic_cast<RouteUnit*>(owner->UnitByName(owner->iniRoute->value("Name").toString())))){
                emit Logging  (Prom::MessAlarm, QDateTime::currentDateTime(), false, objectName(), "ошибка загрузки из файла настроек");
                break;
            }
            _unitMode.append(Prom::unitModeFromInt(owner->iniRoute->value("Mode").toInt()));
            _unitList.append(dynamic_cast<RouteUnit*>(owner->UnitByName(owner->iniRoute->value("Name").toString())));
        }
        owner->iniRoute->endArray();
        owner->iniRoute->endGroup();
        emit UnitUpdate(ID ,  _UnitsString());
    }
}

//------------------------------------------------------------------------------
bool Route::RouteCmd::done()
{
    return ascCount == respCount && respCount == count() && good;
}

//------------------------------------------------------------------------------
unsigned int Route::RouteCmd::count()
{
    //qDebug()<< "count" << endUn - beginUn + 1;
    return endUn - beginUn + 1;
}

//------------------------------------------------------------------------------
int Route::RouteCmd::next()
{
    int TMP;
    if(type == UpStepByStep)
        TMP = last + 1;
    else if(type == DownStepByStep)
        TMP = last - 1;
    else
        TMP = -1;
    //qDebug()<<"next " << TMP;
    return TMP;
}

//------------------------------------------------------------------------------
