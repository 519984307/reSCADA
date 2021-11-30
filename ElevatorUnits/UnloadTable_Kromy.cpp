#include "UnloadTable_Kromy.h"
#include "InDiscretETag.h"
#include "OutETag.h"
#include "OutDiscretETag.h"
#include "electroengine.h"

UnloadTable_Kromy::UnloadTable_Kromy(int ID,
                                      QString Name,
                                      QString TagPefix,
                                      bool Mover,
                                      std::vector<QString> EngSensorNames )

    : OneEngRouteUnit( TypeDryerUnloadTable,
                        ID,
                        Name,
                        TagPefix,
                        Mover,
                        EngSensorNames)
{
    //_pos = new InDiscretETag(this, "Положение стола", ".sensor", true, false, true, true);
    //_pos->onlyChange();
    _pos = new InDiscretETag(this,
                              "Положение стола",
                              ".sensor", true, false,
                              true, true, true);

    if(_engine->startForward->isOn())
    {
        _pos->pulsSensor(true);
    }
    else{
        _pos->pulsSensor(false);
    }
    _pos->needBeDetectedAlarm();

    connect(_engine->startForward,  &OutDiscretETag::s_on,  _pos, &InDiscretETag::needBeDetectedAlarm     , Qt::QueuedConnection);
    connect(_engine->startForward,  &OutDiscretETag::s_off, _pos, &InDiscretETag::needBeDetectedAlarm , Qt::QueuedConnection);

    connect(_engine->startForward,  &OutDiscretETag::s_on,  _pos, &InDiscretETag::pulsSensorON     , Qt::QueuedConnection);
    connect(_engine->startForward,  &OutDiscretETag::s_off, _pos, &InDiscretETag::pulsSensorOFF , Qt::QueuedConnection);

    _SetFR = new OutETag(this, Prom::TpOut, Prom::PreSet,
                          "Установка частоты ЧП",
                          ".speed", false, false, false, true, Prom::VCNo, true);
    _forseSop = new OutDiscretETag(this, Prom::PreSet, "Аварийный стоп", ".forseStop");
}
//------------------------------------------------------------------------------
//void UnloadTable_Kromy::GetSpeed(QVariant Value)
//{
//    _SetFR->setValue(Value);
//}

//------------------------------------------------------------------------------
void UnloadTable_Kromy::_customConnectToGUI(QObject *guiItem,  QObject *propWin)
{
    EngRouteUnit::_customConnectToGUI(guiItem, propWin);
    connect(guiItem, SIGNAL(speedChanged(QVariant)), _SetFR,  SLOT(setValue(QVariant)), Qt::QueuedConnection);
    connect(_SetFR,  SIGNAL(s_valueChd(QVariant)),      guiItem, SLOT(setSpeed(QVariant)), Qt::QueuedConnection);
}

//------------------------------------------------------------------------------
void UnloadTable_Kromy::setMayStart(bool mayStart)
{
    _mayStart = mayStart;
    if(_mayStart){
        if(_waitForMayStart){
            setMode(Prom::UnMdStart, false);
        }
    }
    else if(isCurrOrSetedMode(Prom::UnMdStart)){
        setMode(Prom::UnMdStop, false);
        //SetMode(Prom::UnMdStopAndWait, false);
    }
}
//------------------------------------------------------------------------------
void UnloadTable_Kromy::mayStart()
{
    setMayStart(true);
}
//------------------------------------------------------------------------------
void UnloadTable_Kromy::notMayStart()
{
    setMayStart(false);
}

//------------------------------------------------------------------------------
Prom::SetModeResp UnloadTable_Kromy::_customSetMode(UnitModes *Mode, bool UserOrSys)
{

    if(*Mode == Prom::UnMdFreeze){
        if(_forseSop)_forseSop->on();
    }
    else if(_forseSop)
        _forseSop->off();

    switch (*Mode) {
    case Prom::UnMdStart:
        if(! _mayStart){
//            _setCurrentMode(Prom::UnMdStopAndWait, false);
//            _waitForMayStart = true;
//            emit WaitForStart(_waitForMayStart);
//            return Prom::DoneWhait;
            return Prom::RejNoCond;
        }
        break;
    case Prom::UnMdStopAndWait:{
        _waitForMayStart = true;
        emit WaitForStart(_waitForMayStart);
        Prom::UnitModes M = Prom::UnMdStop;
        return OneEngRouteUnit::_customSetMode(&M, UserOrSys);
    }
        break;
    case Prom::UnMdStop:
    case Prom::UnMdCleanStop:
    case Prom::UnMdFreeze:
        _waitForMayStart = false;
        emit WaitForStart(_waitForMayStart);
        break;
    default:
        break;
    }
    return OneEngRouteUnit::_customSetMode(Mode, UserOrSys);

}

//------------------------------------------------------------------------------
void UnloadTable_Kromy::_updateStateAndMode()
{

    switch(_getEngNowState()) {
    case Prom::EngStoped: {
        _setCurrentState(Prom::UnStStoped);
        if(_waitForMayStart)
            _setCurrentMode(Prom::UnMdStopAndWait);
        else{
            if(_cleaned) _setCurrentMode(Prom::UnMdCleanStop);
            else _setCurrentMode(Prom::UnMdStop);
        }
        emit Stoped();
        break;
    }
    default:
        OneEngRouteUnit::_updateStateAndMode();
    }
}
//------------------------------------------------------------------------------
void UnloadTable_Kromy::setSpeed(QVariant Speed)
{
    _SetFR->setValue( Speed );
}

//------------------------------------------------------------------------------
