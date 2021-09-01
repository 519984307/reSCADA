#include "RouteUnit.h"
#include "route.h"
#include "ETag.h"
#include "SCADAenums.h"

extern Route * g_currentRoute;

RouteUnit::RouteUnit(Prom::UnitType Type,
                      int ID,
                      QString Name,
                      QString TagPefix,
                      bool Mover,
                      Prom::UnitModes SaveMode)
    :Unit(Type,
           ID,
           Name,
           TagPefix,
           Mover,
           SaveMode)
    //      _routeAlarmMode(AlarmRouteMode)
{
    pthread_mutex_init(&mutex, NULL);
    connect(this, &RouteUnit::CommandFromRouteSig, this, &RouteUnit::CommandFromRoute, Qt::QueuedConnection);
}
//------------------------------------------------------------------------------
RouteUnit::~RouteUnit()
{
    pthread_mutex_destroy(&mutex);
}

//----------------------------------------------------------------------------------------.
bool RouteUnit::InRouteCommand(Route * route, Prom::RouteCommand Command) const
{   
    if(_myRoute == route){
        return _routeCommand == Command;
    }
    else return false;
}

//------------------------------------------------------------------------------
void RouteUnit::_AddToCurrentRoute(Prom::UnitModes mode)
{
    AddInRoute(g_currentRoute, mode);
}

//------------------------------------------------------------------------------
bool RouteUnit::SetMyRoute(Route * route, Prom::UnitModes mode)
{
    pthread_mutex_lock(&mutex);
    if(_myRoute != nullptr && route != nullptr) return false;
    if(route != nullptr){
        logging(Prom::MessChangeCommand, QDateTime::currentDateTime(), true, objectName(), "подключение к маршруту " + route->objectName());
        _blocked = true;
    }
    else {
        logging(Prom::MessChangeCommand,  QDateTime::currentDateTime(), true, objectName(), "отключение от маршрута " + (_myRoute != nullptr ? _myRoute->objectName() : "нулевой указаткль!"));
        _blocked = false;

    }
    _myRoute = route;
    _routeMode = mode;
    _routeCommand = Prom::RtCmNo;
    emit SetInRoute(_myRoute == nullptr ? 0 : _myRoute->ID);
    pthread_mutex_unlock(&mutex);
    return true;
}

//------------------------------------------------------------------------------
void RouteUnit::AddInRoute(Route *route, Prom::UnitModes mode)
{
    if(route == nullptr){
        logging(Prom::MessChangeCommand,  QDateTime::currentDateTime(), true, objectName(), "подключение к маршруту не удалось: маршрут не существует");
        return;
    }
    _routeCommand = Prom::RtCmNo;
    connect   (this, SIGNAL(AddInRouteSig(RouteUnit *, Prom::UnitModes)), route, SLOT(AddUnit(RouteUnit * , Prom::UnitModes)), Qt::QueuedConnection);
    emit AddInRouteSig(this, mode);
    disconnect(this, SIGNAL(AddInRouteSig(RouteUnit *, Prom::UnitModes)), route, SLOT(AddUnit(RouteUnit * , Prom::UnitModes)));
}

//------------------------------------------------------------------------------
Prom::UnitModes RouteUnit::RouteMode(Route * rout)
{
    return rout->UnitRouteMode(this);
}

//------------------------------------------------------------------------------
//bool RouteUnit::SetRouteMode(Route * rout, Prom::UnitModes mode)
//{
//    return rout->SetUnitRouteMode(this, mode);
//}

//------------------------------------------------------------------------------
void RouteUnit::CommandFromRoute(Prom::RouteCommand Command)
{
    static Prom::UnitModes newMode = Prom::UnMdNoDef;
    emit logging  (Prom::MessInfo, QDateTime::currentDateTime(), true, objectName(), "обработка юнитом " + objectName() +
                     "команды - " + QString::number(Command));
    if(_myRoute != nullptr) {
        if(_routeCommand == Command){
            if(currentMode() == _ModeOfCommand(&_routeCommand)){
                emit InformToRoute(this, _routeCommand, Prom::RtUnNo);
            }
        }
        else{
            newMode = _ModeOfCommand(&Command);

            switch (setMode(newMode, false)){
            case RejNoCond   :
            case RejTransPr  :
                emit InformToRoute(this, Prom::RtCmNo, Prom::RtUnReject);
                break;
            case RejAlarm    :
                emit InformToRoute(this, Prom::RtCmNo, Prom::RtUnAlarm);
                break;
            case RejAnnown   :
                emit InformToRoute(this, Prom::RtCmNo, Prom::RtUnDtKnowComm);
                break;
            case DoneAlready :
                _routeCommand = Command;
                emit InformToRoute(this, _routeCommand, Prom::RtUnNo);
                break;
            case DoneWhait   :
                _routeCommand = Command;
                break;
               ;
            }
        }
    }
    else {
        emit InformToRoute(this, Prom::RtCmNo, Prom::RtUnNotInRoute);
        logging(Prom::MessInfo,  QDateTime::currentDateTime(), false, objectName(), "попытка маршрутного изменения режима оборудования не состоящего в маршруте");
    }
}
//------------------------------------------------------------------------------
void RouteUnit::_doOnModeChange()
{
    if(_myRoute != nullptr){
        if(Prom::icvalModes(_ModeOfCommand(& _routeCommand), currentMode())){
            emit InformToRoute(this, _routeCommand, Prom::RtUnNo);
        }
        else if(! _alarm){
            if(currentMode() == _ModeOfCommand(&_routeCommand)){
                _routeCommand =  Prom::RtCmNo;
                emit InformToRoute(this, Prom::RtCmNo, Prom::RtUnReject);
            }
            else if(! _midleMode()){
                _routeCommand =  Prom::RtCmNo;
                emit InformToRoute(this, Prom::RtCmNo, Prom::RtUnReject);
            }
        }
    }
}

//------------------------------------------------------------------------------
Prom::UnitModes RouteUnit::_ModeOfCommand(Prom::RouteCommand *Command)
{
    using namespace Prom;
    switch (*Command) {
    case RtCmNo               : return currentMode();
    case RtCmCleanStop        : return UnMdCleanStop;
    case RtCmStop             : return UnMdStop;
    case RtCmToRoute          :
    case RtCmToRoutForClean   :
    case RtCmCleanStart       : return _routeMode;
    case RtCmToSave           :
    case RtCmStopOnRouteAlarm : return saveMode;
    default: return UnMdNoDef;
    }
}

//------------------------------------------------------------------------------
void RouteUnit::_alarmDo()
{
    setMode(saveMode, false);
    _routeCommand =  Prom::RtCmNo;
    emit InformToRoute(this, Prom::RtCmNo, Prom::RtUnAlarm);
}

//------------------------------------------------------------------------------
