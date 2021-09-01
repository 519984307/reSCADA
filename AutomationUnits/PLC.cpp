#include <QSettings>
#include "PLC.h"
#include "OutDiscretETag.h"
#include "promobject.h"

PLC::PLC(int ID,
          QString Name,
          QString TagPefix,
          Prom::UnitModes SaveMode
         )
    :Unit(Prom::TypePLC,
           ID,
           Name,
           TagPefix,
           false,
           SaveMode)
{
    _setCurrentMode(Prom::UnMdStop);

    //connect(this, &PLC::Stoped, owner, &PromObject::SysGlobalStop, Qt::QueuedConnection);
    _heartBeat = new OutDiscretETag(this, Prom::PreSet, "Пульс АРМ для ПЛК", ".HB",
                                    true, false, false, true, false, false, false,
                                    true, true, true, VCNo, true);

    if(_heartBeat->connected()) _setCurrentState(Prom::UnStStoped);
    else _setCurrentState(Prom::UnStNotConnected);

    connect(_heartBeat, &OutDiscretETag::s_qualityChd, this, &PLC::updateState);
    connect(_heartBeat, &OutDiscretETag::s_on, this, &PLC::updateState);
    connect(_heartBeat, &OutDiscretETag::s_off, this, &PLC::updateState);
    _heartBeat->pulsOut(true);
}
//------------------------------------------------------------------------------
void PLC::_updateStateAndMode()
{
    if(_heartBeat->connected()){

        if(_heartBeat->isOn()){
            _setCurrentState(Prom::UnStStarted);
            _setCurrentMode(Prom::UnMdStart);
            emit s_started();
        }
        else {
            if(currentState() == Prom::UnStNotConnected){
                if(resetAlarm()){
                    _setCurrentState(Prom::UnStStoped);
                    setMode(Prom::UnMdStart, true);
                    return;
                }
            }
            _setCurrentState(Prom::UnStStoped);
            _setCurrentMode(Prom::UnMdStop);
            emit s_stoped();
        }
    }
    else {
        if(_heartBeat->isOn()) _heartBeat->off();
        _setCurrentState(Prom::UnStNotConnected);
        _setCurrentMode(Prom::UnMdStop);
        //if( _mode != Prom::UnMdStop)
        emit s_stoped();
        emit s_noDef();
    }
}

//------------------------------------------------------------------------------
Prom::SetModeResp PLC::_customSetMode(Prom::UnitModes *mode, bool)
{
    switch(*mode) {
    case Prom::UnMdStop:{
        _heartBeat->off();
        _setSetedMode(*mode);
        return Prom::DoneAlready;
        break;
    }
    case Prom::UnMdStart:{
        if(_alarm)return Prom::RejAlarm;
        if(currentState() == Prom::UnStStoped) {
            if(_heartBeat->on())
                _setSetedMode(*mode);
            return Prom::DoneAlready;
        }
        else
            return Prom::RejNoCond;
        break;
    }
    default: return Prom::RejAnnown;
    }
}

//------------------------------------------------------------------------------
void PLC::_customConnectToGUI(QObject *guiItem, QObject *)
{
    connect(guiItem, SIGNAL(start()),         this,      SLOT(Start()),    Qt::QueuedConnection);
    connect(guiItem, SIGNAL(stop()),          this,      SLOT(Stop()),     Qt::QueuedConnection);
    connect(this,    SIGNAL(s_stoped()),        guiItem, SLOT(stoped()) ,    Qt::QueuedConnection);
    //connect(this,    SIGNAL(ManualStarted()), guiItem, SLOT(manualWork()), Qt::QueuedConnection);
    connect(this,    SIGNAL(s_started()),       guiItem, SLOT(started()),    Qt::QueuedConnection);
}

//------------------------------------------------------------------------------

















