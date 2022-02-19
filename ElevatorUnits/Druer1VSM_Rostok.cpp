#include <QSettings>
#include "Druer1VSM_Rostok.h"
#include "InDiscretETag.h"
#include "OutDiscretETag.h"
#include "InETag.h"
#include "OutETag.h"
#include "static2posunit.h"


Druer1VSM_Rostok::Druer1VSM_Rostok(int ID,
                                    QString Name,
                                    QString TagPefix)
    :RouteUnit(Prom::TypeDruer,
                ID,
                Name,
                TagPefix,
                false,
                Prom::UnMdCollingStop)
{

    _sound      = new OutDiscretETag(this, Prom::PreSet, "Сирена", ".sound");
    _alarmLamp  = new OutDiscretETag(this, Prom::PreSet, "Аварийная лампа", ".alarmLamp");
    _start      = new OutDiscretETag(this, Prom::PreSet, "Пуск", ".autoStart");
    _cooling    = new OutDiscretETag(this, Prom::PreSet, "Продувка", ".cooling");
    _coolingStop= new OutDiscretETag(this, Prom::PreSet, "Стоп с продувкой",".coolingStop");
    _alarmStop  =  new OutDiscretETag(this, Prom::PreSet, "Аварийный стоп",  ".alarmStop");
    _smoke      = new OutDiscretETag(this, Prom::PreSet, "Дым", ".smokeCmd");
    _fire       = new OutDiscretETag(this, Prom::PreSet, "Пожар",".fireCmd");
    _resetAppAlarm = new OutDiscretETag(this, Prom::PreSet, "Сброс апаратных аварий",
                 ".globalResetAlarm",
                 true, false, false, false, false, false, false, false, true);

    _AS     = new InDiscretETag(this, "Стоп кнопка", ".AS", true, false, false, false, false, false);
    _smokeS = new InDiscretETag(this, "Сигнал 'ДЫМ'", ".smokeS", true, false, true, false, false, false);
    _smokeS->onlyChange();
    _fireS  = new InDiscretETag(this, "Сигнал 'ПОЖАР'", ".fireS", true, false, true, false, false, false);
    _fireS->onlyChange();

    _coolingTemp  = new OutETag(this, Prom::TpOut, Prom::PreSet, "t°C продувки", ".coolingTemp", false, false, false, true, Prom::VCdiv10, false, true);
    _coolingDelay = new OutETag(this, Prom::TpOut, Prom::PreSet, "Задержка продувки сек.", ".coolingDelay", false, false, false, true, Prom::VCNo, false, true);
    _brsStartDelay= new OutETag(this, Prom::TpOut, Prom::PreSet, "Задержка старта горелки сек.", ".brsStartDelay", false, false, false, true, Prom::VCNo, false, true);
    _CFstartDelay = new OutETag(this, Prom::TpOut, Prom::PreSet, "Задержка старта циклофенов сек.", ".CFstartDelay", false, false, false, true, Prom::VCNo, false, true);

    _tempHL    = new InETag(this, Prom::TpIn, "t°C верх",                    ".temp3",  true, 30, 1, false, false, false, false, true, Prom::VCdiv10);
    _tempML    = new InETag(this, Prom::TpIn, "t°C середина",                ".temp2",  true, 30, 1, false, false, false, false, true, Prom::VCdiv10);
    _tempLL    = new InETag(this, Prom::TpIn, "t°C зона охлаждения",         ".temp1",  true, 30, 1, false, false, false, false, true, Prom::VCdiv10);
    _tempOut    = new InETag(this, Prom::TpIn, "t°C теплоносителя на выходе", ".tempOut",  true, 30, 1, false, false, false, false, true, Prom::VCdiv10);
    //    _tempIn    = new InETag(this, Prom::TpIn, "t°C теплоносителя на входе",  ".tempIn",  true, 30, 1, false, false, false, false, true, Prom::VCFloatInIntToDouble);

    _tempHL->needBeUndetectedAlarm();
    _tempML->needBeUndetectedAlarm();
    _tempLL->needBeUndetectedAlarm();
    _tempOut->needBeUndetectedAlarm();

    _tempHLAlarmLvl  = new OutETag(this, Prom::TpOut, Prom::PreSet, "Уставка порога t°C верх",                     ".temp3AlarmLvl",  false, false, false, true, Prom::VCdiv10, true);
    _tempMLAlarmLvl  = new OutETag(this, Prom::TpOut, Prom::PreSet, "Уставка порога t°C середина",                 ".temp2AlarmLvl",  false, false, false, true, Prom::VCdiv10, true);
    _tempLLAlarmLvl  = new OutETag(this, Prom::TpOut, Prom::PreSet, "Уставка порога t°C зона охлаждения",          ".temp1AlarmLvl",  false, false, false, true, Prom::VCdiv10, true);
    _tempOutAlarmLvl = new OutETag(this, Prom::TpOut, Prom::PreSet, "Уставка порога t°C теплоносителя на выходе",  ".tempOutAlarmLvl",  false, false, false, true, Prom::VCdiv10, true);
    //    _tempInAlarmLvl  = new OutETag(this, Prom::TpOut, Prom::PreSet, "Уставка порога t°C теплоносителя на выходе",  ".tempInAlarmLvl",  false, false, false, true, Prom::VCFloatInIntToDouble, true);

    connect(_tempHLAlarmLvl , &OutETag::s_valueChd, _tempHL , &InETag::setDetectLevel);
    connect(_tempMLAlarmLvl , &OutETag::s_valueChd, _tempML , &InETag::setDetectLevel);
    connect(_tempLLAlarmLvl , &OutETag::s_valueChd, _tempLL , &InETag::setDetectLevel);
    connect(_tempOutAlarmLvl, &OutETag::s_valueChd, _tempOut, &InETag::setDetectLevel);
    //    connect(_tempInAlarmLvl, &OutETag::s_valueChd,  _tempIn, &InETag::setDetectLevel);

    //LoadParam();
}

//------------------------------------------------------------------------------
bool Druer1VSM_Rostok::addSubUnit(Unit *unit)
{
    if(unit->unitType == TypeDryerFan){
        CFs.append(unit);
    }
    if(unit->unitType == TypeDryer1_Bunker){
        _bunker = unit;
        //if(_UnloadTable) break;
    }
    if(unit->unitType == TypeDryerUnloadTable){
        _unloadTable = unit;
        //if(_bunker)break;
    }
    if(unit->unitType == TypeBurner){
        _burner = unit;
    }
    return Unit::addSubUnit(unit);
}

//------------------------------------------------------------------------------
void Druer1VSM_Rostok::_updateStateAndMode()
{

}

//------------------------------------------------------------------------------
Prom::SetModeResp Druer1VSM_Rostok::_customSetMode(Prom::UnitModes *mode, bool)
{
    static Prom::SetModeResp respTMP;

    switch (*mode) {
    case Prom::UnMdStart :
        if(_alarm){
            return Prom::RejAlarm;
        }
        else {
            _cooling->off();
            _coolingStop->off();
            if(_start->on()){
                //_sound->On();
                //QTimer::singleShot(3000, [ this ]() { this->_startSound->Off(); });
                if(_burner){
                    if(_burner->currentMode() == Prom::UnMdStart){
                        respTMP = Prom::DoneWhait;
                    }
                    else {
                        respTMP = Prom::DoneAlready;
                    }
                }
            }
            else respTMP = Prom::RejNoCond;
        }
        break;
    case Prom::UnMdCollingStop :
        _start->off();
        _cooling->off();
        _coolingStop->on();//!
        _smoke->off();
        _fire->off();
        _alarmStop->off();
        respTMP = Prom::DoneAlready;
        break;
    case Prom::UnMdColling :
        _start->off();
        _cooling->on();//!
        _coolingStop->off();
        _smoke->off();
        _fire->off();
        _alarmStop->off();
        respTMP = Prom::DoneAlready;
        break;
    case Prom::UnMdFire :
        _start->off();
        _cooling->off();
        _coolingStop->off();
        _smoke->off();
        _fire->on();//!
        _alarmStop->off();
        respTMP = Prom::DoneAlready;
        break;
    case Prom::UnMdSmoke :
        _start->off();
        _cooling->off();
        _coolingStop->off();
        _smoke->on();//!
        _fire->off();
        _alarmStop->off();
        respTMP = Prom::DoneAlready;
        break;
    case Prom::UnMdFreeze :
        _start->off();
        _cooling->off();
        _coolingStop->off();
        _smoke->off();
        _fire->off();
        _alarmStop->on();//!
        respTMP = Prom::DoneAlready;
        break;

    default : return RejAnnown;
    }
    return respTMP;
}
//------------------------------------------------------------------------------
void Druer1VSM_Rostok::_alarmSubUnitDo(Unit * unit)
{

    if(unit->unitType == Prom::TypeBurner){
        if(currentMode() == Prom::UnMdStart || setedMode() == Prom::UnMdStart)
            setMode(Prom::UnMdStop, false);
    }
    else if(unit->unitType == Prom::TypeDryerFan && ! _burner->isAlarm()){
        if(currentMode() == Prom::UnMdStart || setedMode() == Prom::UnMdStart)
            setMode(Prom::UnMdCollingStop, false);
    }
    else if(_unloadTable && ! _alarm) {
        if(currentMode() == Prom::UnMdStart || setedMode() == Prom::UnMdStart)
            _unloadTable->setMode(Prom::UnMdCollingStop, true);
    }
}

//------------------------------------------------------------------------------
void Druer1VSM_Rostok::_alarmDo()
{
//    if(_sound) _sound->On();
//    if(currentMode() == UnMdStart || setedMode() == UnMdStart){
//        SetMode(Prom::UnMdColling, false);
//    }
}
//------------------------------------------------------------------------------
void Druer1VSM_Rostok::_resetAlarmDo()
{
    if(_sound) _sound->off();
}

//------------------------------------------------------------------------------
void Druer1VSM_Rostok::_setblockSubUnitByType(UnitType type, bool block)
{
    foreach(Unit * unit, _subUnits){
        if(unit->unitType == type){
            unit->setBlocked(block);
        }
    }
}

//------------------------------------------------------------------------------
UnitModes Druer1VSM_Rostok::_chackModByType(Prom::UnitType type)
{
    static Prom::UnitModes modeTMP;
    modeTMP = Prom::UnMdNoDef;
    bool katch = false;
    for(int i = 0; i < _subUnits.count(); i ++){
        if(_subUnits[i]->unitType == type){
            if(katch){
                if(_subUnits[i]->currentMode() != modeTMP)
                    return Prom::UnMdNoDef;
            }
            else{
                modeTMP = _subUnits[i]->currentMode();
                katch = true;
            }
        }
    }
    return  modeTMP;
}

//------------------------------------------------------------------------------
void Druer1VSM_Rostok::_updateSubUnitMode(Unit * unit)
{
    if(unit->unitType == Prom::TypeBurner){
        if(unit->currentMode() == Prom::UnMdStart){
            _setCurrentMode(Prom::UnMdStart);
            emit Started();
        }
        else {
            _setCurrentMode(Prom::UnMdStop);
            emit Stoped();
        }
    }
}

//------------------------------------------------------------------------------
void Druer1VSM_Rostok::_updateSubUnitState(Unit *)
{

}
//------------------------------------------------------------------------------
bool Druer1VSM_Rostok::_resetAlarm(bool upClassAlarm)
{
    if(_resetAppAlarm)
        _resetAppAlarm->on();
    return Unit::_resetAlarm(upClassAlarm);
}

//------------------------------------------------------------------------------
void Druer1VSM_Rostok::_customConnectToGUI(QObject *guiItem,  QObject *)
{
    //    connect(guiItem, SIGNAL(addToCurrentRoteStarted()), this, SLOT(AddToCurrentRoteStarted()), Qt::QueuedConnection);
    //    connect(guiItem, SIGNAL(addToCurrentRoteStoped()), this, SLOT(AddToCurrentRoteStoped()),   Qt::QueuedConnection);

    connect(_tempHL,      SIGNAL(s_valueChd(QVariant)), guiItem, SLOT(setTempHL(QVariant)), Qt::QueuedConnection);
    connect(_tempML,      SIGNAL(s_valueChd(QVariant)), guiItem, SLOT(setTempML(QVariant)), Qt::QueuedConnection);
    connect(_tempLL,      SIGNAL(s_valueChd(QVariant)), guiItem, SLOT(setTempLL(QVariant)), Qt::QueuedConnection);
    connect(_tempOut,     SIGNAL(s_valueChd(QVariant)), guiItem, SLOT(setTempEx(QVariant)), Qt::QueuedConnection);

    connect(_tempHL, SIGNAL(s_alarm(QString)), guiItem, SLOT(setTempAlarmHL()), Qt::QueuedConnection);
    connect(_tempHL, SIGNAL(s_alarmReseted()),   guiItem, SLOT(resetTempAlarmHL()), Qt::QueuedConnection);

    connect(_tempML, SIGNAL(s_alarm(QString)), guiItem, SLOT(setTempAlarmML()), Qt::QueuedConnection);
    connect(_tempML, SIGNAL(s_alarmReseted()),   guiItem, SLOT(resetTempAlarmML()), Qt::QueuedConnection);

    connect(_tempLL, SIGNAL(s_alarm(QString)), guiItem, SLOT(setTempAlarmLL()), Qt::QueuedConnection);
    connect(_tempLL, SIGNAL(s_alarmReseted()),   guiItem, SLOT(resetTempAlarmLL()), Qt::QueuedConnection);

    connect(_tempOut, SIGNAL(s_alarm(QString)), guiItem, SLOT(setTempAlarmEx()), Qt::QueuedConnection);
    connect(_tempOut, SIGNAL(s_alarmReseted()),   guiItem, SLOT(resetTempAlarmEx()), Qt::QueuedConnection);

    connect(_tempHLAlarmLvl, SIGNAL(s_valueChd(QVariant)), guiItem, SLOT(setOverTempHL(QVariant)), Qt::QueuedConnection);
    connect(_tempMLAlarmLvl, SIGNAL(s_valueChd(QVariant)), guiItem, SLOT(setOverTempML(QVariant)), Qt::QueuedConnection);
    connect(_tempLLAlarmLvl, SIGNAL(s_valueChd(QVariant)), guiItem, SLOT(setOverTempLL(QVariant)), Qt::QueuedConnection);
    connect(_tempOutAlarmLvl,SIGNAL(s_valueChd(QVariant)), guiItem, SLOT(setOverTempEx(QVariant)), Qt::QueuedConnection);

    connect(guiItem,      SIGNAL(overTempHLChanged(QVariant)), _tempHLAlarmLvl, SLOT(setValue(QVariant)), Qt::QueuedConnection);
    connect(guiItem,      SIGNAL(overTempMLChanged(QVariant)), _tempMLAlarmLvl, SLOT(setValue(QVariant)), Qt::QueuedConnection);
    connect(guiItem,      SIGNAL(overTempLLChanged(QVariant)), _tempLLAlarmLvl, SLOT(setValue(QVariant)), Qt::QueuedConnection);
    connect(guiItem,      SIGNAL(overTempExChanged(QVariant)), _tempOutAlarmLvl, SLOT(setValue(QVariant)), Qt::QueuedConnection);

    connect(guiItem, SIGNAL(start()),       this, SLOT(Start()),       Qt::QueuedConnection);
    connect(guiItem, SIGNAL(coolingStop()), this, SLOT(CoolingStop()),        Qt::QueuedConnection);
    connect(guiItem, SIGNAL(cooling()),     this, SLOT(Cooling()), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(alarmStop()),   this, SLOT(AlarmStop()),   Qt::QueuedConnection);
    connect(guiItem, SIGNAL(smoke()),       this, SLOT(Smoke()),   Qt::QueuedConnection);
    connect(guiItem, SIGNAL(fire()),        this, SLOT(Fire()),   Qt::QueuedConnection);

    connect(this,    SIGNAL(Stoped()),  guiItem, SLOT(stoped()),  Qt::QueuedConnection);
    connect(this,    SIGNAL(Started()), guiItem, SLOT(started()), Qt::QueuedConnection);


    connect(_coolingTemp  , SIGNAL(s_valueChd(QVariant)), guiItem, SLOT(setCoolingTemp(QVariant)), Qt::QueuedConnection);
    connect(_coolingDelay , SIGNAL(s_valueChd(QVariant)), guiItem, SLOT(setCoolingDelay(QVariant)), Qt::QueuedConnection);
    connect(_brsStartDelay, SIGNAL(s_valueChd(QVariant)), guiItem, SLOT(setBrDelay(QVariant)), Qt::QueuedConnection);
    connect(_CFstartDelay , SIGNAL(s_valueChd(QVariant)), guiItem, SLOT(setCfDelay(QVariant)), Qt::QueuedConnection);

    connect(guiItem, SIGNAL(coolingTempChanged(QVariant)), _coolingTemp  ,SLOT(setValue(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(coolingDelayChanged(QVariant)), _coolingDelay ,SLOT(setValue(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(brDelayChanged(QVariant)), _brsStartDelay,SLOT(setValue(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(cfDelayChanged(QVariant)), _CFstartDelay ,SLOT(setValue(QVariant)), Qt::QueuedConnection);


}

//------------------------------------------------------------------------------


















