#include <QSettings>
#include "BurnerVSM_Kromy.h"
#include "InDiscretETag.h"
#include "OutETag.h"
#include "OutDiscretETag.h"
#include "QDebug"

BurnerVSM_Kromy::BurnerVSM_Kromy(int ID,
                                 QString Name,
                                 QString TagPefix)
    :RouteUnit(Prom::TypeBurner,
               ID,
               Name,
               TagPefix,
               false,
               Prom::UnMdStop)
{
    _start = new OutDiscretETag(this, Prom::PreSet, "Пуск", ".start");

    _targetTemp = new OutETag(this, Prom::TpOut, Prom::PreSet,"Уставка t°C", ".setTemp", false, false, false, true, Prom::VCdiv10);
    connect( _targetTemp, &OutETag::s_valueChd, this, &BurnerVSM_Kromy::s_targetTempChd, Qt::QueuedConnection );
    _alarmTemp  = new OutETag(this, Prom::TpOut, Prom::PreSet, "Уставка аврийного порога t°C", ".setOverheat", false, false, false, true, Prom::VCdiv10);
    connect( _alarmTemp, &OutETag::s_valueChd, this, &BurnerVSM_Kromy::s_alarmTempChd, Qt::QueuedConnection );

    _temp = new InETag(this, Prom::TpIn, "Датчик t°C", ".temp", true, _alarmTemp->value(), 5, false, false, false, false, true, Prom::VCdiv10);
    _temp->setDetectLevel(_alarmTemp->value());
    connect(_alarmTemp, & OutETag::s_valueChd, _temp, &InETag::setDetectLevel, Qt::QueuedConnection);
    _temp->needBeUndetectedAlarm();

    //    _ignorTRM = new OutDiscretETag(this, Prom::PreSet, "Игнор аварии ТРМ", ".ignoreTRM");
    _startSig = new InDiscretETag (this, "Пуск от ПЛК",    ".startSig", true, false, true);
    _relayKM  = new InDiscretETag (this, "Реле включения", ".relayKM", true, false, true);


    //    //_startedPID = new InDiscretETag(this, "Запуск ПИД регулятора", ".startedTRM", 1, 0, true, false);
    //    //_startedPID->onlyChange();
    _lessPID = new InDiscretETag(this, "ПИД регулятор меньше", ".less", true, false, true, false, false, false);
    _lessPID->onlyChange();
    _morePID = new InDiscretETag(this, "ПИД регулятор больше", ".more", true, false, true, false, false, false);
    _morePID->onlyChange();

    connect(_start, &OutDiscretETag::s_qualityChd, this, &BurnerVSM_Kromy::updateState, Qt::QueuedConnection);
    connect(_start, &OutDiscretETag::s_on,             this, &BurnerVSM_Kromy::updateState, Qt::QueuedConnection);
    connect(_start, &OutDiscretETag::s_off,            this, &BurnerVSM_Kromy::updateState, Qt::QueuedConnection);

    //connect(_startSig, &InDiscretETag::QualityChanged,  this, &BurnerVSM_Kromy::UpdateState);
    //connect(_startSig, &InDiscretETag::s_valueChd,         this, &BurnerVSM_Kromy::UpdateState);

    connect(_relayKM,  &InDiscretETag::s_qualityChd,  this, &BurnerVSM_Kromy::updateState, Qt::QueuedConnection);
    connect(_relayKM,  &InDiscretETag::s_valueChd,         this, &BurnerVSM_Kromy::updateState, Qt::QueuedConnection);

    if(_start->isOn()) {
        _relayKM->needBeDetectedAlarm();
        _startSig->needBeDetectedAlarm();
    }
    else {
        _relayKM->needBeUndetectedAlarm();
        _startSig->needBeUndetectedAlarm();
    }

    connect(_start, &OutDiscretETag::s_on,  _startSig, &InDiscretETag::needBeDetectedAlarm, Qt::QueuedConnection);
    connect(_start, &OutDiscretETag::s_off, _startSig, &InDiscretETag::needBeUndetectedAlarm, Qt::QueuedConnection);

    connect(_start, &OutDiscretETag::s_on,  _relayKM, &InDiscretETag::needBeDetectedAlarm, Qt::QueuedConnection);
    connect(_start, &OutDiscretETag::s_off, _relayKM, &InDiscretETag::needBeUndetectedAlarm, Qt::QueuedConnection);
}

//------------------------------------------------------------------------------
void BurnerVSM_Kromy::_updateStateAndMode()
{
    if(_start->isOn()){
        if(_relayKM->isDetected()){
            _setCurrentState(Prom::UnStStarted);
            _setCurrentMode(Prom::UnMdStart);
        }
        else{
            _setCurrentState(Prom::UnStStartCommand);
            /*TEST*/if(_relayKM->isImit() && _start->isImit())
                /*TEST*/     QTimer::singleShot(1000, [ this ]() {
                /*TEST*/     this->_relayKM->writeImitVal(true);
                /*TEST*/     this->_startSig->writeImitVal(true);
                /*TEST*/  });
        }
    }
    else{
        if(_relayKM->isDetected()){
            _setCurrentState(Prom::UnStStopCommand);
            /*TEST*/if(_relayKM->isImit() && _start->isImit())
                /*TEST*/     QTimer::singleShot(1000, [ this ]() {
                /*TEST*/     this->_relayKM->writeImitVal(false);
                /*TEST*/     this->_startSig->writeImitVal(false);
                /*TEST*/  });
        }
        else{
            _setCurrentState(Prom::UnStStoped);
            _setCurrentMode(Prom::UnMdStop);
        }
    }
}
//------------------------------------------------------------------------------
void BurnerVSM_Kromy::_hiLimTargetTempCorrect(QVariant AlarmTemp)
{
    _targetTemp->setHiLimit( AlarmTemp.toDouble() - 10 );
}

//------------------------------------------------------------------------------
SetModeResp BurnerVSM_Kromy::_customSetMode(UnitModes * mode, bool)
{
    if(! _sensorsConnected) return  Prom::RejNoCond;

    switch (*mode) {
    case Prom::UnMdStart :
        if(_alarm)
            return Prom::RejAlarm;

        _setSetedMode(Prom::UnMdStart);
        if(currentState() == Prom::UnStStarted)
            return DoneAlready;
        if(currentState() == Prom::UnStStartCommand)
            return DoneWhait;

        if(_start->on()){
            return Prom::DoneWhait;
        }
        else {
            _setSetedMode(Prom::UnMdNoDef);
            return Prom::DoneWhait;
        }
        break;
    case Prom::UnMdColling :
    case Prom::UnMdCollingStop :
    case Prom::UnMdStop :
        _start->off();
        _setSetedMode(Prom::UnMdStop);
        if(currentState() == Prom::UnStStoped)
            return DoneAlready;
        if(currentState() == Prom::UnStStopCommand)
            return DoneWhait;
        return DoneWhait;
        break;

    default : return RejAnnown;

    }
}

//------------------------------------------------------------------------------
void BurnerVSM_Kromy::stop()
{
    setMode(Prom::UnMdCollingStop, true);
}

//------------------------------------------------------------------------------
void BurnerVSM_Kromy::start()
{
    if(! _blocked)
        setMode(Prom::UnMdStart, true);
}

//------------------------------------------------------------------------------
void BurnerVSM_Kromy::setTargetTemp(QVariant TargetTemp)
{
    if( TargetTemp != _targetTemp->value() ){
        _targetTemp->setValue( TargetTemp.toDouble());
    }
}
//------------------------------------------------------------------------------
void BurnerVSM_Kromy::setAlarmTemp(QVariant TargetTemp)
{
    if( TargetTemp != _targetTemp->value() ){
        _temp->setDetectLevel( TargetTemp.toDouble() );
        _alarmTemp->setValue( TargetTemp.toDouble() );
        _targetTemp->setHiLimit( TargetTemp.toDouble() - 5 );
        emit s_maxTargTempChd(TargetTemp);
    }
}

//------------------------------------------------------------------------------
void BurnerVSM_Kromy::_customConnectToGUI(QObject *guiItem,  QObject *)
{
    connect(_temp,       SIGNAL(s_valueChd(QVariant)), guiItem, SLOT(setTemp(QVariant)),            Qt::QueuedConnection);
    connect(_targetTemp, SIGNAL(s_valueChd(QVariant)), guiItem, SLOT(setTargetTemp(QVariant)),      Qt::QueuedConnection);
    connect(_alarmTemp,  SIGNAL(s_valueChd(QVariant)), guiItem, SLOT(setAlarmTemp(QVariant)),  Qt::QueuedConnection);

    connect(guiItem, SIGNAL(targetTempChanged (QVariant)), this, SLOT(setTargetTemp(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(alarmTempChanged (QVariant)), this,   SLOT(setAlarmTemp(QVariant)), Qt::QueuedConnection);

    connect(this, SIGNAL(s_stoped()),      guiItem, SLOT(stoped()),      Qt::QueuedConnection);
    connect(this, SIGNAL(s_started()),     guiItem, SLOT(started()),     Qt::QueuedConnection);
    connect(this, SIGNAL(s_startComand()), guiItem, SLOT(startComand()), Qt::QueuedConnection);
    connect(this, SIGNAL(s_stopComand()),  guiItem, SLOT(stopComand()),  Qt::QueuedConnection);
    // connect(this, SIGNAL(ManualStarted()), guiItem, SLOT(manualWork()), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(start()), this, SLOT(start()), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(stop()),  this, SLOT(stop()),  Qt::QueuedConnection);
}
//------------------------------------------------------------------------------
void BurnerVSM_Kromy::_setCurrentState(UnitStates CurrentState)
{
    _currentState = CurrentState;
    switch (CurrentState) {
    case UnStStartCommand : emit s_startComand();break;
    case UnStStarted :      emit s_started();    break;
    case UnStStopCommand :  emit s_stopComand(); break;
    case UnStColling :
    case UnStStoped :       emit s_stoped();     break;
    case UnStNoDef :        emit s_noDef();      break;
    default: emit s_noDef();
    }
    RouteUnit::_setCurrentState(CurrentState);
}

//------------------------------------------------------------------------------


















