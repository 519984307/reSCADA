#include <qdebug.h>
#include "EngRouteUnit.h"
#include "InDiscretETag.h"

EngRouteUnit::EngRouteUnit(Prom::UnitType Type,
                            int ID,
                            QString Name,
                            QString TagPefix,
                            bool Mover,
                            std::vector<QString>,
                            bool BlockedStop)
    : RouteUnit(Type,
                 ID,
                 Name,
                 TagPefix,
                 Mover,
                 Prom::UnMdStop),
      _blockedStop(BlockedStop)
{

}

//------------------------------------------------------------------------------
Prom::SetModeResp EngRouteUnit::_customSetMode(Prom::UnitModes *Mode, bool)
{
    switch(*Mode) {
    case Prom::UnMdFreeze:
    case Prom::UnMdStop:{
        _stop();
        if(mover) _cleanTimer->stop();///убрать в отдельный класс
        if(currentState() == Prom::UnStStoped)
            return Prom::DoneAlready;
        else{
//            _setSetedMode(*Mode);
            return Prom::DoneWhait;
        }

        break;
    }
    case Prom::UnMdStart:
        if( _sensorsConnected && ! _alarm) {
            if(mover) {               ///убрать в отдельный класс
                _cleanTimer->stop();
            }
            if(currentState() == Prom::UnStStarted)
                return Prom::DoneAlready;
            else if(_start()){
//                _setSetedMode(*Mode);
                return Prom::DoneWhait;
            }
        }
        break;
    case Prom::UnMdCleanStop:{          /// убрать в отдельный класс
        if(mover && currentMode() == Prom::UnMdStart) {
            _cleaned = false;
            _cleanTimer->start();
            emit Cleaning();
//            _setSetedMode(*Mode);
            return Prom::DoneWhait;
        }
        else return RejNoCond;

    }
    default:;
    }
    return RejAnnown;
}

//------------------------------------------------------------------------------
void EngRouteUnit::_updateStateAndMode()
{
    switch(_getEngNowState()) {
    case Prom::EngNoDef:{
        _setCurrentState(Prom::UnStNoDef);
        _setCurrentMode(Prom::UnMdNoDef);
        emit Stoped();
        break;
    }
    case Prom::EngToStopForward: {
        _setCurrentState(Prom::UnStStopCommand);
        emit StopComand();
        break;
    }
    case Prom::EngStoped: {
        _setCurrentState(Prom::UnStStoped);
        if(_cleaned) _setCurrentMode(Prom::UnMdCleanStop);
        else _setCurrentMode(Prom::UnMdStop);
        emit Stoped();
        break;
    }
    case Prom::EngToForvard: {
        _setCurrentState(Prom::UnStStartCommand);
        emit StartComand();
        break;
    }
    case Prom::EngForvard: {
        _cleaned = false;
        emit Started();
        if(mover && _cleanTimer->isActive()){
            _setCurrentState(Prom::UnStClean);
            _setCurrentMode(Prom::UnMdStart);
            emit Cleaning();
        }
        else{
            if(_cleaned) {
                _setCurrentState(Prom::UnStCleanStarted);
                _setCurrentMode(Prom::UnMdCleanStart);
            }
            else {
                _setCurrentState(Prom::UnStStarted);
                _setCurrentMode(Prom::UnMdStart);
            }
        }
        break;
    }
    case Prom::EngManualForward: {
        _setCurrentState(Prom::UnStManualStarted);
        _setCurrentMode(Prom::UnMdNoDef);
        emit ManualStarted();
        break;
    }
    case Prom::EngAlarm: {
        _setCurrentState(Prom::UnStNoDef);
        _setCurrentMode(Prom::UnMdNoDef);
        break;
    }
    default:{
        _setCurrentState(Prom::UnStNoDef);
        detectAlarm("недопустимое состояние электродвигателя");
    }
    }
}

//------------------------------------------------------------------------------
void EngRouteUnit::_cleanTimeEnd()
{
    _cleaned = true;
    _stop();
}

//------------------------------------------------------------------------------
void EngRouteUnit::_customConnectToGUI(QObject * guiItem,  QObject *)
{
    connect(this,    SIGNAL(StartComand()),             guiItem, SLOT(startComand()),             Qt::QueuedConnection);
    connect(this,    SIGNAL(StopComand()),              guiItem, SLOT(stopComand()),              Qt::QueuedConnection);
    connect(guiItem, SIGNAL(addToCurrentRoteStarted()), this,    SLOT(AddToCurrentRoteStarted()), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(addToCurrentRoteStoped()),  this,    SLOT(AddToCurrentRoteStoped()),  Qt::QueuedConnection);
    connect(guiItem, SIGNAL(start()),                   this,    SLOT(Start()),                   Qt::QueuedConnection);
    connect(guiItem, SIGNAL(stop()),                    this,    SLOT(Stop()),               Qt::QueuedConnection);
    connect(this,    SIGNAL(Stoped()),                  guiItem, SLOT(stoped()) ,                 Qt::QueuedConnection);
    connect(this,    SIGNAL(ManualStarted()),           guiItem, SLOT(manualWork()) ,             Qt::QueuedConnection);
    connect(this,    SIGNAL(Started()),                 guiItem, SLOT(started()),                 Qt::QueuedConnection);
}

//------------------------------------------------------------------------------

