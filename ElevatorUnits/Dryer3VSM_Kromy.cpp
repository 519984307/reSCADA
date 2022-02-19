#include <QSettings>
#include "Dryer3VSM_Kromy.h"
#include "InDiscretETag.h"
#include "OutDiscretETag.h"
#include "InETag.h"
#include "OutETag.h"
#include "UnloadTable_Kromy.h"
#include "BurnerVSM_Kromy.h"

Dryer3VSM_Kromy::Dryer3VSM_Kromy(int ID,
                                 QString Name,
                                 QString TagPefix)
    :RouteUnit(TypeSilos,
               ID,
               Name,
               TagPefix,
               false,
               UnMdStop)
{
    _startCFDelay = new QTimerExt(this);
    _startCFDelay->setSingleShot(true);
    connect(_startCFDelay, &QTimer::timeout, this, &Dryer3VSM_Kromy::_startCFbyTimer);

    _startBrDelay = new QTimerExt(this);
    _startBrDelay->setSingleShot(true);
    connect(_startBrDelay, &QTimer::timeout, this, &Dryer3VSM_Kromy::_startBrByTimer);

    _stopBrDelay = new QTimerExt(this);
    _stopBrDelay->setSingleShot(true);
    connect(_stopBrDelay, &QTimer::timeout, this, &Dryer3VSM_Kromy::_stopBrByTimer);

    _coolingTimer = new QTimerExt(this);
    _coolingTimer->setSingleShot(true);
    connect(_coolingTimer, &QTimer::timeout, this, &Dryer3VSM_Kromy::_coolingTimerEnd);

    addTimer(_startCFDelay);
    addTimer(_startBrDelay);
    addTimer(_coolingTimer);

    _vsAlarm   = new OutDiscretETag(this, PreSet, "Сирена аварии", ".vsAlarm");
    _vsWarning = new OutDiscretETag(this, PreSet, "Сирена предупреждения", ".vsWarning");
    _vsWarning->pulsOut(true);
    _as        = new InDiscretETag(this, "Кнопка стоп", ".AS", true, false, true, false);
    _as->needBeUndetectedAlarm();

    _temp1    = new InETag(this, TpIn, "Датчик t°C 1",  ".temp1.temp",  true, 30, 5, false, false, false, false, true, VCdiv10);
    _temp2    = new InETag(this, TpIn, "Датчик t°C 2",  ".temp2.temp",  true, 30, 5, false, false, false, false, true, VCdiv10);
    _temp3    = new InETag(this, TpIn, "Датчик t°C 3",  ".temp3.temp",  true, 30, 5, false, false, false, false, true, VCdiv10);
    _temp4    = new InETag(this, TpIn, "Датчик t°C 4",  ".temp4.temp",  true, 30, 5, false, false, false, false, true, VCdiv10);
    _temp5    = new InETag(this, TpIn, "Датчик t°C 5",  ".temp5.temp",  true, 30, 5, false, false, false, false, true, VCdiv10);
    _temp6    = new InETag(this, TpIn, "Датчик t°C 6",  ".temp6.temp",  true, 30, 5, false, false, false, false, true, VCdiv10);
    _temp7    = new InETag(this, TpIn, "Датчик t°C 7",  ".temp7.temp",  true, 30, 5, false, false, false, false, true, VCdiv10);
    _temp8    = new InETag(this, TpIn, "Датчик t°C 8",  ".temp8.temp",  true, 30, 5, false, false, false, false, true, VCdiv10);
    _temp9    = new InETag(this, TpIn, "Датчик t°C 9",  ".temp9.temp",  true, 30, 5, false, false, false, false, true, VCdiv10);
    _temp10   = new InETag(this, TpIn, "Датчик t°C 10", ".temp10.temp", true, 30, 5, false, false, false, false, true, VCdiv10);
    _temp11   = new InETag(this, TpIn, "Датчик t°C 11", ".temp11.temp", true, 30, 5, false, false, false, false, true, VCdiv10);
    _temp12   = new InETag(this, TpIn, "Датчик t°C 12", ".temp12.temp", true, 30, 5, false, false, false, false, true, VCdiv10);
    _temp13   = new InETag(this, TpIn, "Датчик t°C 13", ".temp13.temp", true, 30, 5, false, false, false, false, true, VCdiv10);
    _temp14   = new InETag(this, TpIn, "Датчик t°C 14", ".temp14.temp", true, 30, 5, false, false, false, false, true, VCdiv10);
    _temp15   = new InETag(this, TpIn, "Датчик t°C 15", ".temp15.temp", true, 30, 5, false, false, false, false, true, VCdiv10);


    _needLoad      = new OutDiscretETag(this, PreSet, "Нужна загрузка", ".NeedLoad");
    _needUnload    = new OutDiscretETag(this, PreSet, "Нужна разгрузка", ".NeedUnload");
    _needCircul    = new OutDiscretETag(this, PreSet, "Нужна циркуляция", ".NeedCircule");
    _load          = new InDiscretETag(this, "Загрузка", ".LoadStart",    true, false, true, false);
    _unload        = new InDiscretETag(this, "Разгрузка", ".UnloadStart", true, false, true, false,false,false,true,VCNo,10);
    //connect( _needLoad, &OutDiscretETag::s_valueChd, this, &Dryer3VSM_Kromy::_loadUnloadCireculManag, Qt::QueuedConnection );
    //connect( _needUnload, &OutDiscretETag::s_valueChd, this, &Dryer3VSM_Kromy::_loadUnloadCireculManag, Qt::QueuedConnection );
    //connect( _needCircul, &OutDiscretETag::s_valueChd, this, &Dryer3VSM_Kromy::_loadUnloadCireculManag, Qt::QueuedConnection );


    _inHumidity   = new InETag(this, TpIn, "Вх. влагомер", ".hrIn", false, 1000, 5, false, false, false, false, true, VCdiv10);
    _outHumidity  = new InETag(this, TpIn, "Вых. влагомер", ".hrOut", false, 1000, 5, false, false, false, false, true, VCdiv10, 120);

    connect( _outHumidity, &InETag::s_valueChd, this, &Dryer3VSM_Kromy::_loadUnloadCireculManag, Qt::QueuedConnection );

    _temp1->needBeUndetectedAlarm();
    _temp2->needBeUndetectedAlarm();
    _temp3->needBeUndetectedAlarm();
    _temp4->needBeUndetectedAlarm();
    _temp5->needBeUndetectedAlarm();
    _temp6->needBeUndetectedAlarm();
    _temp7->needBeUndetectedAlarm();
    _temp8->needBeUndetectedAlarm();
    _temp9->needBeUndetectedAlarm();
    _temp10->needBeUndetectedAlarm();
    _temp11->needBeUndetectedAlarm();
    _temp12->needBeUndetectedAlarm();
    _temp13->needBeUndetectedAlarm();
    _temp14->needBeUndetectedAlarm();
    _temp15->needBeUndetectedAlarm();

    _alarmTemp1  = new OutETag(this, TpOut, PreSet, "Уставка порога t°C 1",  ".temp1.setTemp",  false, false, false, true, VCdiv10, true);
    _alarmTemp2  = new OutETag(this, TpOut, PreSet, "Уставка порога t°C 2",  ".temp2.setTemp",  false, false, false, true, VCdiv10, true);
    _alarmTemp3  = new OutETag(this, TpOut, PreSet, "Уставка порога t°C 3",  ".temp3.setTemp",  false, false, false, true, VCdiv10, true);
    _alarmTemp4  = new OutETag(this, TpOut, PreSet, "Уставка порога t°C 4",  ".temp4.setTemp",  false, false, false, true, VCdiv10, true);
    _alarmTemp5  = new OutETag(this, TpOut, PreSet, "Уставка порога t°C 5",  ".temp5.setTemp",  false, false, false, true, VCdiv10, true);
    _alarmTemp6  = new OutETag(this, TpOut, PreSet, "Уставка порога t°C 6",  ".temp6.setTemp",  false, false, false, true, VCdiv10, true);
    _alarmTemp7  = new OutETag(this, TpOut, PreSet, "Уставка порога t°C 7",  ".temp7.setTemp",  false, false, false, true, VCdiv10, true);
    _alarmTemp8  = new OutETag(this, TpOut, PreSet, "Уставка порога t°C 8",  ".temp8.setTemp",  false, false, false, true, VCdiv10, true);
    _alarmTemp9  = new OutETag(this, TpOut, PreSet, "Уставка порога t°C 9",  ".temp9.setTemp",  false, false, false, true, VCdiv10, true);
    _alarmTemp10 = new OutETag(this, TpOut, PreSet, "Уставка порога t°C 10", ".temp10.setTemp", false, false, false, true, VCdiv10, true);
    _alarmTemp11 = new OutETag(this, TpOut, PreSet, "Уставка порога t°C 11", ".temp11.setTemp", false, false, false, true, VCdiv10, true);
    _alarmTemp12 = new OutETag(this, TpOut, PreSet, "Уставка порога t°C 12", ".temp12.setTemp", false, false, false, true, VCdiv10, true);
    _alarmTemp13 = new OutETag(this, TpOut, PreSet, "Уставка порога t°C 13", ".temp13.setTemp", false, false, false, true, VCdiv10, true);
    _alarmTemp14 = new OutETag(this, TpOut, PreSet, "Уставка порога t°C 14", ".temp14.setTemp", false, false, false, true, VCdiv10, true);
    _alarmTemp15 = new OutETag(this, TpOut, PreSet, "Уставка порога t°C 15", ".temp15.setTemp", false, false, false, true, VCdiv10, true);

    connect(_alarmTemp1 , &OutETag::s_valueChd, _temp1 , &InETag::setDetectLevel);
    connect(_alarmTemp2 , &OutETag::s_valueChd, _temp2 , &InETag::setDetectLevel);
    connect(_alarmTemp3 , &OutETag::s_valueChd, _temp3 , &InETag::setDetectLevel);
    connect(_alarmTemp4 , &OutETag::s_valueChd, _temp4 , &InETag::setDetectLevel);
    connect(_alarmTemp5 , &OutETag::s_valueChd, _temp5 , &InETag::setDetectLevel);
    connect(_alarmTemp6 , &OutETag::s_valueChd, _temp6 , &InETag::setDetectLevel);
    connect(_alarmTemp7 , &OutETag::s_valueChd, _temp7 , &InETag::setDetectLevel);
    connect(_alarmTemp8 , &OutETag::s_valueChd, _temp8 , &InETag::setDetectLevel);
    connect(_alarmTemp9 , &OutETag::s_valueChd, _temp9 , &InETag::setDetectLevel);
    connect(_alarmTemp10, &OutETag::s_valueChd, _temp10, &InETag::setDetectLevel);
    connect(_alarmTemp11, &OutETag::s_valueChd, _temp11, &InETag::setDetectLevel);
    connect(_alarmTemp12, &OutETag::s_valueChd, _temp12, &InETag::setDetectLevel);
    connect(_alarmTemp13, &OutETag::s_valueChd, _temp13, &InETag::setDetectLevel);
    connect(_alarmTemp14, &OutETag::s_valueChd, _temp14, &InETag::setDetectLevel);
    connect(_alarmTemp15, &OutETag::s_valueChd, _temp15, &InETag::setDetectLevel);

    //управляет температурой продукта через температуру горелки
    _brnrTempProdTempPID1 = new PID( this , 1, 0.01, 1, 30);
    _brnrTempProdTempPID2 = new PID( this , 1, 0.01, 1, 30);
    _brnrTempProdTempPID3 = new PID( this , 1, 0.01, 1, 30);

    _brnrTempProdTempPID1->setObjectName("brnrTempProdTempPID1");
    _brnrTempProdTempPID2->setObjectName("brnrTempProdTempPID2");
    _brnrTempProdTempPID3->setObjectName("brnrTempProdTempPID3");

    //    connect(_brnrTempProdTempPID1, &PID::s_logging, this, &Unit::logging, Qt::QueuedConnection);
    //    connect(_brnrTempProdTempPID2, &PID::s_logging, this, &Unit::logging, Qt::QueuedConnection);
    //    connect(_brnrTempProdTempPID3, &PID::s_logging, this, &Unit::logging, Qt::QueuedConnection);
    //принимаю value из фун. _avProdLowTemp(ср. температура) сушилки
    _brnrTempProdTempPID1->setInputValueFun(std::bind(std::mem_fn(&Dryer3VSM_Kromy::_avProdLowTemp1),this));
    _brnrTempProdTempPID2->setInputValueFun(std::bind(std::mem_fn(&Dryer3VSM_Kromy::_avProdLowTemp2),this));
    _brnrTempProdTempPID3->setInputValueFun(std::bind(std::mem_fn(&Dryer3VSM_Kromy::_avProdLowTemp3),this));
    // u передаю сигналом в горелки(ниже в addSubUnit)

    //управляет выходной влажностью через температуру продукта
    _prodTempHumPID = new PID( this , 1, 0.01, 1, 10 );
    _prodTempHumPID->setInverse(true);
    _prodTempHumPID->setObjectName("prodTempHumPID");
    connect(_prodTempHumPID, &PID::s_logging, this, &Unit::logging, Qt::QueuedConnection);
    //принимаю value из тега выходной влажности
    connect( _outHumidity, &InETag::s_valueChd, _prodTempHumPID,
             &PID::setInputValue_QVar, Qt::QueuedConnection);
    //передаю u в ПИД темп. продукта
    connect( _prodTempHumPID, &PID::s_uChdQVar, this,
             &Dryer3VSM_Kromy::setTargetLowTemp, Qt::QueuedConnection);

    //    //управляет выходной влажностью(смещённой на +5) через скорость выгрузки
    //    _UTSpeedHumPID = new PID( this, 1, 0.01, 0.5, 10, 50);
    //    _UTSpeedHumPID->setObjectName("UTSpeedHumPID");
    //    connect(_UTSpeedHumPID, &PID::s_logging, this, &Unit::logging, Qt::QueuedConnection);
    //    //принимаю value из тега выходной влажности
    //    connect( _outHumidity, &InETag::s_valueChd, _UTSpeedHumPID,
    //             &PID::setInputValue_QVar, Qt::QueuedConnection);
    //    // u передаю сигналом в выгрузной стол(ниже в addSubUnit)

}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::_conditiontHumPID()
{
    if( _autoHumidity && currentMode() == UnMdStart) {
        if(!_prodTempHumPID->isStarted()) {
            _prodTempHumPID->start();
        }
    } else _prodTempHumPID->stop();
}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::_conditionAllTempPid()
{
    if( _autoTemp ) {
        if( _burner1 != nullptr && _burner1->currentMode() == UnMdStart){
            if( ! _brnrTempProdTempPID1->isStarted() ){
                _brnrTempProdTempPID1->start();
            }
        } else _brnrTempProdTempPID1->stop();

        if( _burner2 != nullptr && _burner2->currentMode() == UnMdStart ){
            if(! _brnrTempProdTempPID2->isStarted()){
                _brnrTempProdTempPID2->start();
            }
        } else _brnrTempProdTempPID2->stop();

        if( _burner3 != nullptr && _burner3->currentMode() == UnMdStart ){
            if( ! _brnrTempProdTempPID3->isStarted()){
                _brnrTempProdTempPID3->start();
            }
        } else _brnrTempProdTempPID3->stop();
    } else {
        _brnrTempProdTempPID1->stop();
        _brnrTempProdTempPID2->stop();
        _brnrTempProdTempPID3->stop();
    }
}

//------------------------------------------------------------------------------
bool Dryer3VSM_Kromy::addSubUnit(Unit *unit)
{
    if( Unit::addSubUnit(unit) ){
        if( unit->unitType == TypeDryerFan){
            if( CFs.indexOf(unit) < 0 ) {
                CFs.append(unit);
            }
        }
        if(unit->unitType == TypeDryer3_Bunker){
            _bunker = unit;
            connect(_bunker, SIGNAL(s_modeChange(Unit*)), this, SLOT(_loadUnloadCireculManag()), Qt::QueuedConnection);
            connect(_bunker, SIGNAL(s_stateChange(Unit*)), this, SLOT(_loadUnloadCireculManag()), Qt::QueuedConnection);
            connect(_bunker, SIGNAL( s_belowLL() ), SLOT( _bunkerBelowLL() ), Qt::QueuedConnection);
        }
        if(unit->unitType == TypeDryerUnloadTable){
            _unloadTable = unit;
            connect(_unload, SIGNAL(Undetected()), _unloadTable, SLOT(notMayStart()), Qt::QueuedConnection);
            connect(_unload, SIGNAL(Detected()),   _unloadTable, SLOT(mayStart()),    Qt::QueuedConnection);
            //        connect( _UTSpeedHumPID, SIGNAL(s_uChdQVar(QVariant)),
            //                 _unloadTable, SLOT(setSpeed(QVariant)), Qt::QueuedConnection);
        }
        if(unit->unitType == TypeBurner){
            if( Brs.indexOf(unit) < 0){
                Brs.append(unit);
            }
        }
        return true;
    }
    return false;

}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::setBurner( Unit * Burner, uint Number )
{
    if( Burner == nullptr ) return;
    if( Brs.indexOf( Burner ) >= 0){
        Brs.append( Burner );
    }
    PID * pid{ nullptr };
    Unit * burnerOld{ nullptr };
    switch (Number) {
    case 1:
        burnerOld = _burner1;
        pid = _brnrTempProdTempPID1;
        _burner1 = Burner;
        break;
    case 2:
        burnerOld = _burner2;
        pid = _brnrTempProdTempPID2;
        _burner2 = Burner;
        break;
    case 3:
        burnerOld = _burner3;
        pid = _brnrTempProdTempPID3;

        _burner3 = Burner;
        break;
    default:
        return;
    }

    disconnect( pid, SIGNAL(s_uChdQVar(QVariant)), burnerOld, SLOT(setTargetTemp(QVariant)) );
    connect( pid, SIGNAL(s_uChdQVar(QVariant)), Burner, SLOT(setTargetTemp(QVariant)), Qt::QueuedConnection );
    addSubUnit( Burner );
    //    BurnerVSM_Kromy::s_modeChange() * sd;
}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::setShHum(  Unit *ShHum, bool InOrOut )
{
    if( ShHum == nullptr ) return;
    Unit * shnekNew = ShHum;
    Unit * shnekOld;
    if( InOrOut ){
        shnekOld = _shHumIn;
        _shHumIn = ShHum;
        shnekNew = ShHum;
        disconnect( _load, SIGNAL( Detected() ),   shnekOld, SLOT( Start() ) );
        disconnect( _load, SIGNAL( Undetected() ), shnekOld, SLOT( Stop() ) );
        if( _shHumIn != nullptr ){
            connect( _load, SIGNAL( Detected() ),   shnekNew, SLOT( Start() ), Qt::QueuedConnection );
            connect( _load, SIGNAL( Undetected() ), shnekNew, SLOT( Stop() ) , Qt::QueuedConnection );
        }
    } else {
        shnekOld = _shHumOut;
        _shHumOut = ShHum;
        shnekNew = ShHum;
        disconnect( _unload, SIGNAL( Detected() ),   shnekOld, SLOT( Start() ) );
        disconnect( _unload, SIGNAL( Undetected() ), shnekOld, SLOT( Stop() ) );
        if( _shHumOut != nullptr ){
            connect( _unload, SIGNAL( Detected() ),   shnekNew, SLOT( Start() ), Qt::QueuedConnection );
            connect( _unload, SIGNAL( Undetected() ), shnekNew, SLOT( Stop() ) , Qt::QueuedConnection );
        }
    }
    addSubUnit( ShHum );
}
//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::setCircule(bool circule)
{
    //_circuleOn = circule;
    if(circule){
        _needUnload->off();
        _needCircul->on();
    }
    else {
         _needCircul->off();
    }
    if(_bunker){
        _updateSubUnitMode(_bunker);
    }
}
//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::setUnload(bool unload)
{
    //_unloadOn = unload;
    if(unload){
        _needCircul->off();
        _needUnload->on();
    }
    else
        _needUnload->off();
}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::setAutoLoad(bool load)
{
    _autoLoad = load;
    _loadUnloadCireculManag();

}
//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::_loadUnloadCireculManag()
{
    if( _bunker->currentState() == UnStHiLvl ){
        setUnload( true );
    }
    else{
        if( _autoHumidity && currentMode() == UnMdStart ){
            if( _outHumidity->value().toDouble() == 0 ){
                setCircule( true );
            }
            else if( _outHumidity->value().toDouble() <= _targetHumidity.toDouble() ){
                setUnload( true );
            }
            else {
                if( _outHumidity->value().toDouble() > _targetHumidity.toDouble() + 0.5){
                    setCircule( true );
                }
            }
        }
    }
    if( _autoLoad && _bunker->currentMode() == UnMdNeedLoad){
        _needLoad->on();
    }
    else _needLoad->off();
}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::_bunkerBelowLL()
{
    _vsWarning->singleImpulse( 10000, 0);
}
//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::setAutoHumidity(bool AutoHumidity)
{
    _autoHumidity = AutoHumidity;
    if( _autoHumidity){
        _conditiontHumPID();
        setAutoTemp( true );
        _loadUnloadCireculManag();
    }
    else{

    }
    emit s_autoHumidityChd( _autoHumidity );
}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::setAutoTemp(bool AutoTemp)
{
    _autoTemp = AutoTemp;
    if( _autoTemp ){
        _conditionAllTempPid();
    }
    else{
        setAutoHumidity( false );
    }
    emit s_autoTempChd( _autoTemp );
}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::setTargetLowTemp(QVariant Temp)
{
    _targetLowTemp = Temp.toDouble();
    if( ! _prodTempHumPID->isStarted() ){
        _brnrTempProdTempPID1->setSetpoint( Temp.toDouble() );
        _brnrTempProdTempPID2->setSetpoint( Temp.toDouble() );
        _brnrTempProdTempPID3->setSetpoint( Temp.toDouble() );
    }
    emit s_targetLowTempChd( _targetLowTemp );
}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::setTargetHumidity(QVariant Hymidity)
{
    _targetHumidity = Hymidity.toDouble();
    _prodTempHumPID->setSetpoint( _targetHumidity.toDouble() );
    //_UTSpeedHumPID->setSetpoint( Hymidity.toDouble() + 5 );
    _loadUnloadCireculManag();
}
//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::setMaxProductTemp(QVariant Temp)
{
    _maxProductTemp = Temp.toDouble();
    _prodTempHumPID->setMaxU( _maxProductTemp.toDouble() );

}
//------------------------------------------------------------------------------
//void Dryer3VSM_Kromy::setMaxUTSpeed(QVariant Speed)
//{
//    _maxUTSpeed = Speed.toDouble();
//    _UTSpeedHumPID->setMaxU( _maxUTSpeed.toDouble() );
//}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::_startCFbyTimer()
{
    foreach(Unit * cfU, CFs) {
        if(cfU->currentMode() != UnMdStart)
            if(cfU->setedMode() != UnMdStart){
                cfU->setMode(UnMdStart, false);
                break;
            }
    }
}
//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::_alarmSubUnitDo(Unit * unit)
{
    if(unit->unitType == TypeShnek
            || unit->unitType == TypeDryerUnloadTable ){
        if(_vsAlarm)_vsAlarm->on();
        return;
    }
    else {
        _setSetedMode( UnMdNoDef );
        _commandToSubUnitByType(TypeBurner, UnMdColling, false, { UnMdStart } );
    }
    if(_vsAlarm && currentMode()== UnMdStart )_vsAlarm->on();
}
//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::_startBrByTimer()
{
    if( setedMode() == UnMdStart && _BrMode != UnMdStart && ! _alarm){
        foreach(Unit * brU, Brs) {
            if( !brU->isCurrOrSetedMode( UnMdStart )){
                brU->setMode(UnMdStart, false);
                break;
            }
        }
    }
}
//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::_stopBrByTimer()
{
    if( setedModeIn({ UnMdColling, UnMdCollingStop })){
        foreach( Unit * brU, Brs ){

            if( setedMode() == UnMdCollingStop
                    && ! brU->isCurrOrSetedMode(UnMdCollingStop) ){
                if(brU->setMode( UnMdCollingStop, false) == DoneWhait){
                    break;
                }
            }
            else if(setedMode() == UnMdColling
                    && ! brU->isCurrOrSetedMode( UnMdColling )
                    && ( brU->setedMode() == UnMdCollingStop
                         || brU->isCurrOrSetedModeIn({ UnMdStop, UnMdStart }))){
                if(brU->setMode( UnMdColling, false)== DoneWhait){
                    break;
                }
            }
        }
    }
}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::_setCurrentState(UnitStates CurrentState)
{
    _currentState = CurrentState;
    static QString Status;
    if(isAlarm())Status = "Не готова. ";
    else{
        if(_bunker && _bunker->currentState() != UnStEmpty)
            Status = "Готова. ";
        else
            Status = "Нужна загрузка. ";
    }

    switch(CurrentState) {
    case UnStStartCommand       : Status =  "Запуск."; break;
    case UnStStarted            : Status =  "Сушка.";  break;
    case UnStStopCommand        : Status += "СТОП!";   break;
    case UnStStoped             : Status += "Не продута.";  break;
    case UnStColling            : Status += "Продувка.";    break;
    case UnStCollingStoped      : Status += "Продута.";     break;
    case UnStCollingStopCommand : Status += "Продувка и стоп.";  break;
    case UnStNoDef              : ;
    default: ;
    }
    emit SetStatus(Status);
}

//------------------------------------------------------------------------------
bool Dryer3VSM_Kromy::_resetAlarm( bool upClassAlarm)
{
    if(_vsAlarm) _vsAlarm->off();
    return Unit::_resetAlarm(upClassAlarm);
}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::_resetAlarmDo()
{
    _setCurrentState(currentState());
}
//------------------------------------------------------------------------------
QVariant Dryer3VSM_Kromy::_avProdLowTemp1()
{
    return( _temp4->value().toDouble()
            + _temp7->value().toDouble()
            + _temp10->value().toDouble() )
            /3;
}
//------------------------------------------------------------------------------
QVariant Dryer3VSM_Kromy::_avProdLowTemp2()
{
    return( _temp5->value().toDouble()
            + _temp8->value().toDouble()
            + _temp11->value().toDouble() )
            /3;
}
//------------------------------------------------------------------------------
QVariant Dryer3VSM_Kromy::_avProdLowTemp3()
{
    return( _temp6->value().toDouble()
            + _temp9->value().toDouble()
            + _temp12->value().toDouble() )
            /3;
}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::setMaxBernerTemp( QVariant maxBernerTemp )
{
    _maxBernerTemp = maxBernerTemp;
    _brnrTempProdTempPID1->setMaxU(_maxBernerTemp.toDouble());
    _brnrTempProdTempPID2->setMaxU(_maxBernerTemp.toDouble());
    _brnrTempProdTempPID3->setMaxU(_maxBernerTemp.toDouble());
    emit s_maxBernerTempChd(_maxBernerTemp);
}

//------------------------------------------------------------------------------
//void Dryer3VSM_Kromy::_allPIDstart()
//{

//    if( _autoHumidity )_prodTempHumPID->start();
//    //_UTSpeedHumPID->start();
//}
//------------------------------------------------------------------------------
//void Dryer3VSM_Kromy::_allPIDstop()
//{
//    _brnrTempProdTempPID1->stop();
//    _brnrTempProdTempPID2->stop();
//    _brnrTempProdTempPID3->stop();
//    _prodTempHumPID->stop();
//    //_UTSpeedHumPID->stop();
//}
//------------------------------------------------------------------------------
SetModeResp Dryer3VSM_Kromy::_customSetMode(UnitModes *mode, bool UserOrSys)
{
    static SetModeResp respTMP;
    allTimerStop();
    if(_vsWarning)_vsWarning->off();

    switch(*mode) {
    case UnMdStart :
        if(_alarm){
            return RejAlarm;
        }
        if( ! _bunker || _bunker->stateNotIn({ UnStHiLvl, UnStMidLvl, UnStLowLvl }) ) {
            return  RejNoCond;
        }
        if(_CFmode != UnMdStart){
            respTMP = DoneWhait;
            _startCFDelay->singleShot(_vsWarning->impulseDuration(), this, &Dryer3VSM_Kromy::_startCFbyTimer);
            if(_vsWarning)_vsWarning->on();
        }
        else if(_BrMode != UnMdStart){
            respTMP = DoneWhait;
            _startBrDelay->singleShot(_vsWarning->impulseDuration(), this, &Dryer3VSM_Kromy::_startBrByTimer);
            if(_vsWarning)_vsWarning->on();
        }
        else{
            respTMP = DoneAlready;
            _setCurrentState(UnStStarted);
        }
        if(respTMP == DoneWhait) _setCurrentState(UnStStartCommand);
        break;

    case UnMdStop :
        respTMP = _commandToSubUnitByType(TypeBurner, UnMdStop, UserOrSys );
        if(_unloadTable) _unloadTable->setMode(UnMdFreeze, UserOrSys);

        respTMP = _commandToSubUnitByType(TypeDryerFan, UnMdStop, UserOrSys);

        if(respTMP == DoneWhait) _setCurrentState(UnStStopCommand);
        break;

    case UnMdCollingStop :
    case UnMdColling :

        if(_CFmode != UnMdStart){
            respTMP = DoneWhait;
            _startCFDelay->singleShot(_vsWarning->impulseDuration(), this, &Dryer3VSM_Kromy::_startCFbyTimer);
            if(_vsWarning)_vsWarning->on();
        } else if( _BrMode != UnMdCollingStop ){
            respTMP = DoneWhait;
            _stopBrByTimer();
        }
        else {
            respTMP = DoneAlready;//_startSubUnitByType(TypeDryerFan, UserOrSys);
            if( *mode == UnMdCollingStop )
                _coolingTimer->start();
        }
        if(respTMP == DoneWhait){
            if( *mode == UnMdColling )
                _setCurrentState(UnStCollingCommand);
            else if( *mode == UnMdCollingStop ){
                _setCurrentState(UnStCollingStopCommand );
            }
        }
        break;
    default : return RejAnnown;
    }
    return respTMP;
}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::_alarmDo()
{
    if(_as && _as->isDetected()){
        if(_unloadTable)_unloadTable->setMode(UnMdStop, true);
        setMode(UnMdStop, false);
    }
    if(_vsAlarm)_vsAlarm->on();
    if( isCurrOrSetedMode(UnMdStart) )
        setMode(UnMdColling, false);
}

//------------------------------------------------------------------------------
SetModeResp Dryer3VSM_Kromy::_startSubUnitByType(UnitType type, bool UserOrSys)
{
    static SetModeResp respTMP;
    //static bool alarmCF;
    foreach(Unit * unit, _subUnits ){
        if(unit->unitType == type){
            if(unit->isAlarm())
                return RejAlarm;
        }
    }
    respTMP = DoneAlready;
    foreach(Unit * unit, _subUnits ){
        if(unit->unitType == type){
            switch(unit->setMode(UnMdStart, UserOrSys)) {
            case DoneWhait : respTMP = DoneWhait;
                break;
            case DoneAlready :
                if(respTMP != DoneWhait) respTMP = DoneAlready;
                break;
            default: //В остальных случаях только авария или нет условий
                _commandToSubUnitByType(type, UnMdStop, false);
                break;
            }
        }
    }
    return respTMP;
}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::_updateSubUnitState(Unit *unit)
{
    switch(unit->unitType) {
    case TypeDryerFan :
        break;
    case TypeBurner :
        if( unit->stateIn( {UnStStartCommand, UnStStarted})
                && _CFmode != UnMdStart){
            unit->setMode( UnMdStop, false );
        }
        break;
    case TypeDryer3_Bunker :
        if(isCurrOrSetedMode(UnMdStart)){
            if(unit->stateNotIn({ UnStLowLvl, UnStMidLvl, UnStHiLvl })){
                setMode(UnMdColling, false);
                //                if(_unloadTable && _unloadTable->isCurrOrSetedMode(UnMdStart))
                //                    _unloadTable->setMode(UnMdStop, false);
            }
        }
        break;
    default:
        break;
    }
}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::_updateSubUnitMode(Unit * unit)
{
    static UnitModes _CFmodeOld;
    static UnitModes _BrModeOld;
    _CFmodeOld = _CFmode;
    _BrModeOld = _BrMode;

    if(unit != nullptr){
        switch(unit->unitType) {
        case TypeDryerFan :
        {
            if(unit->currentMode() == _CFmode)
                return;
            if(unit->currentMode() != UnMdStart){

                if(setedModeIn({UnMdStart,
                               UnMdCollingStop,
                               UnMdColling})){
                    if(_coolingTimer->isActive())
                        _rejectSetedMode();
                }
            }
            _CFmode = _commonModeByType(CFs);
            if( _CFmode != UnMdStart && _CFmode != UnMdStop
                    && setedModeIn({
                                   UnMdStart,
                                   UnMdCollingStop,
                                   UnMdColling })){
                if( ! _startCFDelay->isActive()
                        //чтобы остановившийся циклофен не мог запустить следующий
                        && unit->currentMode() == UnMdStart )
                    _startCFDelay->start();
            }
            else _startCFDelay->stop();
            break;
        }
        case TypeBurner :
        {
            _conditionAllTempPid();
            if(unit->currentMode() == _BrMode) return;

            _BrMode = _commonModeByType(Brs);

            if( _BrMode != UnMdStart && _BrMode != UnMdColling
                    && _CFmode == UnMdStart
                    && setedMode() == UnMdStart ){
                if(! _startBrDelay->isActive()
                        && unit->currentModeIn({ UnMdColling, UnMdStart}))
                    _startBrDelay->start();
            }
            else _startBrDelay->stop();

            if( _BrMode != UnMdCollingStop
                    && setedModeIn( { UnMdCollingStop, UnMdColling } ) ){
                if( !_stopBrDelay->isActive() )
                    _stopBrDelay->start();
            }
            else _stopBrDelay->stop();
            break;
        }
        default:;
        }
    }
    if( _CFmodeOld != _CFmode || _BrModeOld != _BrMode ){
        _updateStateAndMode();
    }
}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::_updateStateAndMode()
{

    //++++++++++++Блокировки+++++++++++

    _setblockSubUnitByType(TypeDryerFan, ( _BrMode == UnMdStart
                                           || _BrMode == UnMdPartStarted) );

    _setblockSubUnitByType(TypeBurner, _CFmode != UnMdStart );

    //--------------Блокировки-----------

    if( _CFmode == UnMdStart){
        if( setedMode() ==  UnMdStart
                && !_startBrDelay->isActive() && ! _alarm ){
            _startBrByTimer();
        }
        else if( setedModeIn( { UnMdColling, UnMdCollingStop })
                 && !_stopBrDelay->isActive() ){
            _stopBrByTimer();
        }
        switch(_BrMode) {
        //        case  UnMdNoDef:
        //        case  UnMdStop:
        //            break;
        case  UnMdCollingStop:
            if(setedMode() == UnMdCollingStop){
                _coolingTimer->start();
                _setCurrentState(UnStCollingStopCommand);
                break;
            }
        case  UnMdColling:
            if(setedMode() == UnMdStop){
                if(_commandToSubUnitByType(TypeDryerFan, UnMdStop, false) == DoneAlready)
                    _setCurrentMode(UnMdStop);
            }
            else if(setedMode() == UnMdColling) {
                _setCurrentMode( UnMdColling );
                _setCurrentState(UnStColling);
            }
            else {
                _setCurrentState(UnStColling);
            }
            break;
        case  UnMdPartStarted:
            _coolingTimer->stop();
            if( setedMode() ==  UnMdStart){
                _setCurrentState( UnStStartCommand );
            }
            else if( setedMode() ==  UnMdNoDef){
                _setCurrentMode( UnMdStart );
                _setCurrentState( UnStStarted );
                _loadUnloadCireculManag();
            }
            break;
        case  UnMdStart:
            _coolingTimer->stop();
            if( setedModeNotIn( {UnMdStop , UnMdCollingStop} )
                    && setedMode() == UnMdStart){
                _setCurrentMode( UnMdStart );
                _setCurrentState( UnStStarted );
                _loadUnloadCireculManag();
            }
            break;
        default:;
        }
    }
    else{
        _startBrDelay->stop();
        if( _BrMode == UnMdPartStarted
                || _BrMode == UnMdStart ){
            _commandToSubUnitByType(TypeBurner, UnMdColling, false, {UnMdStart});
        }

        if( _CFmode == UnMdPartStarted && setedModeIn({UnMdStart})) {
            _setCurrentState(UnStStartCommand);
        }
        else if( _CFmode == UnMdNoDef ) {
            _setCurrentState( UnStNoDef );
            _setCurrentMode( UnMdNoDef );
        }
        else if( _CFmode == UnMdStop ) {
            if(_BrMode == UnMdStop ){
                _setCurrentMode( UnMdStop );
                _setCurrentState( UnStStoped );
            }
            else if( _BrMode == UnMdCollingStop
                     && ! _coolingTimer->isActive()) {
                _setCurrentMode( UnMdCollingStop );
                _setCurrentState( UnStCollingStoped );
            }
        }
    }
    _conditiontHumPID();
}
//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::Start()
{
    if(! _blocked)setMode(UnMdStart, true);
}
//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::Stop()
{
    setMode(UnMdStop, true);
}
//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::Cooling()
{
    setMode(UnMdColling, true);
}
//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::CoolingStop()
{
    setMode(UnMdCollingStop, true);
}
//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::GetCoolDelay(QVariant Val)
{
    _coolingTimer->setInterval(Val.toUInt() * 1000);
}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::GetStartCFDelay(QVariant Val)
{
    _startCFDelay->setInterval(Val.toUInt() * 1000);
}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::GetStartBrDelay(QVariant Val)
{
    _startBrDelay->setInterval(Val.toUInt() * 1000);
}
//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::setStopBrDelay(QVariant Val)
{
    _stopBrDelay->setInterval(Val.toUInt() * 1000);
}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::reInitialise()
{
    emit s_coolDelayChd( _coolingTimer->interval() / 1000 );
    emit s_startCFDelayChd( _startCFDelay->interval() / 1000 );
    emit s_startBrDelayChd( _startBrDelay->interval() / 1000 );
    emit s_stopBrDelayChd( _stopBrDelay->interval() / 1000 );

    emit s_targetLowTempChd( _targetLowTemp );
    emit s_targetHumidityChd( _targetHumidity );
    emit s_maxBernerTempChd( _maxBernerTemp );
    emit s_maxProductTempChd( _maxProductTemp );
    emit s_maxUTSpeedChd( _maxUTSpeed  );

    emit s_brnrTempProdTempPID_KpChd(_brnrTempProdTempPID1->getKp()  );
    emit s_brnrTempProdTempPID_KiChd(_brnrTempProdTempPID1->getKi()  );
    emit s_brnrTempProdTempPID_KdChd(_brnrTempProdTempPID1->getKd()  );
    emit s_brnrTempProdTempPID_TdChd(_brnrTempProdTempPID1->getTd()  );
    emit s_brnrTempProdTempPID_MaxUChd(_brnrTempProdTempPID1->getMaxU());
    emit s_brnrTempProdTempPID_MinUChd(_brnrTempProdTempPID1->getMinU());

    emit s_prodTempHumPID_KpChd(_prodTempHumPID->getKp()  );
    emit s_prodTempHumPID_KiChd(_prodTempHumPID->getKi()  );
    emit s_prodTempHumPID_KdChd(_prodTempHumPID->getKd()  );
    emit s_prodTempHumPID_TdChd(_prodTempHumPID->getTd()  );
    emit s_prodTempHumPID_MaxUChd(_prodTempHumPID->getMaxU());
    emit s_prodTempHumPID_MinUChd(_prodTempHumPID->getMinU());

    //    emit s_UTSpeedHumPID_KpChd(_UTSpeedHumPID->getKp()  );
    //    emit s_UTSpeedHumPID_KiChd(_UTSpeedHumPID->getKi()  );
    //    emit s_UTSpeedHumPID_KdChd(_UTSpeedHumPID->getKd()  );
    //    emit s_UTSpeedHumPID_TdChd(_UTSpeedHumPID->getTd()  );
    //    emit s_UTSpeedHumPID_MaxUChd(_UTSpeedHumPID->getMaxU());
    //    emit s_UTSpeedHumPID_MinUChd(_UTSpeedHumPID->getMinU());

    emit s_autoHumidityChd( _autoHumidity );
    emit s_autoTempChd( _autoTemp );

    Unit::reInitialise();
}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::saveParam()
{
    ini->setValue(tagPrefix + "/coolTimeSec", _coolingTimer->interval() / 1000);
    ini->setValue(tagPrefix + "/startCFDelay", _startCFDelay->interval() / 1000);
    ini->setValue(tagPrefix + "/startBrDelay", _startBrDelay->interval() / 1000);
    ini->setValue(tagPrefix + "/stopBrDelay", _stopBrDelay->interval() / 1000);

    ini->setValue(tagPrefix + "/targetLowTemp",  _targetLowTemp.toDouble());
    ini->setValue(tagPrefix + "/targetHumidity", _targetHumidity.toDouble());
    ini->setValue(tagPrefix + "/maxBernerTemp",  _maxBernerTemp.toDouble());
    ini->setValue(tagPrefix + "/maxProductTemp", _maxProductTemp.toDouble());
    ini->setValue(tagPrefix + "/maxUTSpeed",     _maxUTSpeed.toDouble());

    ini->setValue(tagPrefix + "/brnrTempProdTempPID->Kp",   _brnrTempProdTempPID1->getKp()  );
    ini->setValue(tagPrefix + "/brnrTempProdTempPID->Ki",   _brnrTempProdTempPID1->getKi()  );
    ini->setValue(tagPrefix + "/brnrTempProdTempPID->Kd",   _brnrTempProdTempPID1->getKd()  );
    ini->setValue(tagPrefix + "/brnrTempProdTempPID->Td",   _brnrTempProdTempPID1->getTd()  );
    //    ini->setValue(tagPrefix + "/brnrTempProdTempPID->MaxU", _brnrTempProdTempPID1->getMaxU());
    ini->setValue(tagPrefix + "/brnrTempProdTempPID->MinU", _brnrTempProdTempPID1->getMinU());

    ini->setValue(tagPrefix + "/brnrTempProdTempPID->Kp",   _brnrTempProdTempPID2->getKp()  );
    ini->setValue(tagPrefix + "/brnrTempProdTempPID->Ki",   _brnrTempProdTempPID2->getKi()  );
    ini->setValue(tagPrefix + "/brnrTempProdTempPID->Kd",   _brnrTempProdTempPID2->getKd()  );
    ini->setValue(tagPrefix + "/brnrTempProdTempPID->Td",   _brnrTempProdTempPID2->getTd()  );
    //    ini->setValue(tagPrefix + "/brnrTempProdTempPID->MaxU", _brnrTempProdTempPID2->getMaxU());
    ini->setValue(tagPrefix + "/brnrTempProdTempPID->MinU", _brnrTempProdTempPID2->getMinU());

    ini->setValue(tagPrefix + "/brnrTempProdTempPID->Kp",   _brnrTempProdTempPID3->getKp()  );
    ini->setValue(tagPrefix + "/brnrTempProdTempPID->Ki",   _brnrTempProdTempPID3->getKi()  );
    ini->setValue(tagPrefix + "/brnrTempProdTempPID->Kd",   _brnrTempProdTempPID3->getKd()  );
    ini->setValue(tagPrefix + "/brnrTempProdTempPID->Td",   _brnrTempProdTempPID3->getTd()  );
    //    ini->setValue(tagPrefix + "/brnrTempProdTempPID->MaxU", _brnrTempProdTempPID3->getMaxU());
    ini->setValue(tagPrefix + "/brnrTempProdTempPID->MinU", _brnrTempProdTempPID3->getMinU());

    ini->setValue(tagPrefix + "/prodHumidPID.Kp",   _prodTempHumPID->getKp()  );
    ini->setValue(tagPrefix + "/prodHumidPID.Ki",   _prodTempHumPID->getKi()  );
    ini->setValue(tagPrefix + "/prodHumidPID.Kd",   _prodTempHumPID->getKd()  );
    ini->setValue(tagPrefix + "/prodHumidPID.Td",   _prodTempHumPID->getTd()  );
    //ini->setValue(tagPrefix + "/prodHumidPID.MaxU", _prodTempHumPID->getMaxU());
    ini->setValue(tagPrefix + "/prodHumidPID.MinU", _prodTempHumPID->getMinU());

    //    ini->setValue(tagPrefix + "/UTSpeedHumPID->Kp",   _UTSpeedHumPID->getKp()  );
    //    ini->setValue(tagPrefix + "/UTSpeedHumPID->Ki",   _UTSpeedHumPID->getKi()  );
    //    ini->setValue(tagPrefix + "/UTSpeedHumPID->Kd",   _UTSpeedHumPID->getKd()  );
    //    ini->setValue(tagPrefix + "/UTSpeedHumPID->Td",   _UTSpeedHumPID->getTd()  );
    //    ini->setValue(tagPrefix + "/UTSpeedHumPID->MaxU", _UTSpeedHumPID->getMaxU());
    //    ini->setValue(tagPrefix + "/UTSpeedHumPID->MinU", _UTSpeedHumPID->getMinU());

    ini->setValue(tagPrefix + "/autoHumidity", _autoHumidity);
    ini->setValue(tagPrefix + "/autoTemp", _autoTemp);

    Unit::saveParam();
}
//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::loadParam()
{
    _coolingTimer->setInterval(ini->value(tagPrefix + "/coolTimeSec", 60000).toInt() * 1000);
    _startCFDelay->setInterval(ini->value(tagPrefix + "/startCFDelay", 5000).toInt() * 1000);
    _startBrDelay->setInterval(ini->value(tagPrefix + "/startBrDelay", 60000).toInt() * 1000);
    _stopBrDelay->setInterval(ini->value(tagPrefix + "/stopBrDelay", 30).toInt() * 1000);

    setTargetLowTemp(  ini->value(tagPrefix + "/targetLowTemp",     0).toDouble());
    setTargetHumidity( ini->value(tagPrefix + "/targetHumidity",    0).toDouble());
    setMaxBernerTemp( ini->value(tagPrefix + "/maxBernerTemp", 0).toDouble());
    setMaxProductTemp( ini->value(tagPrefix + "/maxProductTemp", 0).toDouble());
    //setMaxUTSpeed(   ini->value(tagPrefix + "/maxUTSpeed", 0).toDouble());

    _brnrTempProdTempPID1->setKp(  ini->value(tagPrefix + "/brnrTempProdTempPID->Kp",   0).toDouble());
    _brnrTempProdTempPID1->setKi(  ini->value(tagPrefix + "/brnrTempProdTempPID->Ki",   0).toDouble());
    _brnrTempProdTempPID1->setKd(  ini->value(tagPrefix + "/brnrTempProdTempPID->Kd",   0).toDouble());
    _brnrTempProdTempPID1->setTd(  ini->value(tagPrefix + "/brnrTempProdTempPID->Td",   0).toDouble());
    //_brnrTempProdTempPID1->setMaxU(ini->value(tagPrefix + "/prodTempPID.MaxU", 100).toDouble());
    _brnrTempProdTempPID1->setMinU(ini->value(tagPrefix + "/prodTempPID.MinU", 0).toDouble());

    _brnrTempProdTempPID2->setKp(  ini->value(tagPrefix + "/brnrTempProdTempPID->Kp",   0).toDouble());
    _brnrTempProdTempPID2->setKi(  ini->value(tagPrefix + "/brnrTempProdTempPID->Ki",   0).toDouble());
    _brnrTempProdTempPID2->setKd(  ini->value(tagPrefix + "/brnrTempProdTempPID->Kd",   0).toDouble());
    _brnrTempProdTempPID2->setTd(  ini->value(tagPrefix + "/brnrTempProdTempPID->Td",   0).toDouble());
    //_brnrTempProdTempPID2->setMaxU(ini->value(tagPrefix + "/prodTempPID.MaxU", 100).toDouble());
    _brnrTempProdTempPID2->setMinU(ini->value(tagPrefix + "/prodTempPID.MinU", 0).toDouble());

    _brnrTempProdTempPID3->setKp(  ini->value(tagPrefix + "/brnrTempProdTempPID->Kp",   0).toDouble());
    _brnrTempProdTempPID3->setKi(  ini->value(tagPrefix + "/brnrTempProdTempPID->Ki",   0).toDouble());
    _brnrTempProdTempPID3->setKd(  ini->value(tagPrefix + "/brnrTempProdTempPID->Kd",   0).toDouble());
    _brnrTempProdTempPID3->setTd(  ini->value(tagPrefix + "/brnrTempProdTempPID->Td",   0).toDouble());
    //_brnrTempProdTempPID3->setMaxU(ini->value(tagPrefix + "/prodTempPID.MaxU", 100).toDouble());
    _brnrTempProdTempPID3->setMinU(ini->value(tagPrefix + "/prodTempPID.MinU", 0).toDouble());

    _prodTempHumPID->setKp(  ini->value(tagPrefix + "/prodHumidPID.Kp",   0).toDouble());
    _prodTempHumPID->setKi(  ini->value(tagPrefix + "/prodHumidPID.Ki",   0).toDouble());
    _prodTempHumPID->setKd(  ini->value(tagPrefix + "/prodHumidPID.Kd",   0).toDouble());
    _prodTempHumPID->setTd(  ini->value(tagPrefix + "/prodHumidPID.Td",   0).toDouble());
    //_prodTempHumPID->setMaxU(ini->value(tagPrefix + "/prodHumidPID.MaxU", 80).toDouble());
    _prodTempHumPID->setMinU(ini->value(tagPrefix + "/prodHumidPID.MinU", 0).toDouble());

    //    _UTSpeedHumPID->setKp(  ini->value(tagPrefix + "/UTSpeedHumPID->Kp",   0).toDouble());
    //    _UTSpeedHumPID->setKi(  ini->value(tagPrefix + "/UTSpeedHumPID->Ki",   0).toDouble());
    //    _UTSpeedHumPID->setKd(  ini->value(tagPrefix + "/UTSpeedHumPID->Kd",   0).toDouble());
    //    _UTSpeedHumPID->setTd(  ini->value(tagPrefix + "/UTSpeedHumPID->Td",   0).toDouble());
    //_UTSpeedHumPID->setMaxU(ini->value(tagPrefix + "/UTSpeedHumPID->MaxU", 50).toDouble());
    //_UTSpeedHumPID->setMinU(ini->value(tagPrefix + "/UTSpeedHumPID->MinU", 10).toDouble());

    setAutoTemp    ( ini->value(tagPrefix + "/autoTemp",     false).toBool());
    setAutoHumidity( ini->value(tagPrefix + "/autoHumidity", false).toBool());


    Unit::loadParam();
}

//SetModeResp Dryer3VSM_Kromy::_commandToSubUnitByType(UnitType type, bool UserOrSys)
//{
//    static SetModeResp respTMP;
//    respTMP = DoneAlready;
//    foreach(Unit * unit, _subUnits ){
//        if(unit->unitType == type){
//            switch(unit->setMode(UnMdStop, UserOrSys)) {
//            case DoneWhait : respTMP = DoneWhait;
//                break;
//            case DoneAlready :
//                if(respTMP != DoneWhait) respTMP = DoneAlready;
//                break;
//            default: //В остальных случаях только авария или нет условий
//                break;
//            }
//        }
//    }
//    return respTMP;
//}

//------------------------------------------------------------------------------
SetModeResp
Dryer3VSM_Kromy::_commandToSubUnitByType(UnitType type,
                                         UnitModes Command,
                                         bool UserOrSys,
                                         QVector<UnitModes>TargetUnitModes)
{
    static SetModeResp respTMP;
    respTMP = DoneAlready;
    foreach(Unit * unit, _subUnits ){
        if(unit->unitType == type){
            if( ! TargetUnitModes.empty() ){
                if(! unit->isCurrOrSetedModeIn(TargetUnitModes))
                    continue;
            }
            switch(unit->setMode(Command, UserOrSys)) {
            case DoneWhait : respTMP = DoneWhait;
                break;
            case DoneAlready :
                if(respTMP != DoneWhait) respTMP = DoneAlready;
                break;
            default: //В остальных случаях только авария или нет условий
                break;
            }
        }
    }
    return respTMP;
}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::_setblockSubUnitByType(UnitType type, bool block)
{
    foreach(Unit * unit, _subUnits ){
        if(unit->unitType == type){
            unit->setBlocked(block);
        }
    }
}

//------------------------------------------------------------------------------
UnitModes Dryer3VSM_Kromy::_commonModeByType(QVector<Unit*> unitsGroup )
{
    static UnitModes modeTMP;
    if( _subUnits.count() == 0 ) return UnMdNoDef;
    modeTMP = unitsGroup[0]->currentMode();
    //UnitModes modeUnitTMP;

    for(int i = 1; i < unitsGroup.count(); i ++ ){
        //modeUnitTMP = subUnits[i]->currentMode();
        if( unitsGroup[i]->currentMode() != modeTMP){
            if( unitsGroup[i]->currentMode() == UnMdStart
                    || modeTMP == UnMdStart  ){
                return UnMdPartStarted;
            }
            else return UnMdNoDef;
        }
    }
    return  modeTMP;
}

//----------------------------------------------------------------------------------------
bool Dryer3VSM_Kromy::_chackSubUnitModeByType(UnitType Type, UnitModes Mode)
{
    for(int i = 0; i < _subUnits.count(); i ++ ){
        if(_subUnits[i]->unitType == Type){
            if(_subUnits[i]->currentMode() == Mode)
                return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::setBrnrTempProdTempPID_Kp(QVariant Kp  )
{
    _brnrTempProdTempPID1->setKp(Kp  .toDouble());
    _brnrTempProdTempPID2->setKp(Kp  .toDouble());
    _brnrTempProdTempPID3->setKp(Kp  .toDouble());
}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::setBrnrTempProdTempPID_Ki(QVariant Ki  )
{
    _brnrTempProdTempPID1->setKi(Ki  .toDouble());
    _brnrTempProdTempPID2->setKi(Ki  .toDouble());
    _brnrTempProdTempPID3->setKi(Ki  .toDouble());
}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::setBrnrTempProdTempPID_Kd(QVariant Kd  )
{
    _brnrTempProdTempPID1->setKd(Kd  .toDouble());
    _brnrTempProdTempPID2->setKd(Kd  .toDouble());
    _brnrTempProdTempPID3->setKd(Kd  .toDouble());
}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::setBrnrTempProdTempPID_Td(QVariant Td  )
{
    _brnrTempProdTempPID1->setTd(Td  .toDouble());
    _brnrTempProdTempPID2->setTd(Td  .toDouble());
    _brnrTempProdTempPID3->setTd(Td  .toDouble());
}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::setBrnrTempProdTempPID_MaxU(QVariant MaxU)
{
    _brnrTempProdTempPID1->setMaxU(MaxU.toDouble());
    _brnrTempProdTempPID2->setMaxU(MaxU.toDouble());
    _brnrTempProdTempPID3->setMaxU(MaxU.toDouble());
}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::setBrnrTempProdTempPID_MinU(QVariant MinU)
{
    _brnrTempProdTempPID1->setMinU(MinU.toDouble());
    _brnrTempProdTempPID2->setMinU(MinU.toDouble());
    _brnrTempProdTempPID3->setMinU(MinU.toDouble());
}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::setProdTempHumPID_Kp(QVariant Kp  ){ _prodTempHumPID->setKp(Kp  .toDouble()); }
void Dryer3VSM_Kromy::setProdTempHumPID_Ki(QVariant Ki  ){ _prodTempHumPID->setKi(Ki  .toDouble()); }
void Dryer3VSM_Kromy::setProdTempHumPID_Kd(QVariant Kd  ){ _prodTempHumPID->setKd(Kd  .toDouble()); }
void Dryer3VSM_Kromy::setProdTempHumPID_Td(QVariant Td  ){ _prodTempHumPID->setTd(Td  .toDouble()); }
void Dryer3VSM_Kromy::setProdTempHumPID_MaxU(QVariant MaxU){ _prodTempHumPID->setMaxU(MaxU.toDouble()); }
void Dryer3VSM_Kromy::setProdTempHumPID_MinU(QVariant MinU){ _prodTempHumPID->setMinU(MinU.toDouble()); }
//void Dryer3VSM_Kromy::setUTSpeedHumPID_Kp(QVariant Kp  ){ _UTSpeedHumPID->setKp(Kp  .toDouble()); };
//void Dryer3VSM_Kromy::setUTSpeedHumPID_Ki(QVariant Ki  ){ _UTSpeedHumPID->setKi(Ki  .toDouble()); };
//void Dryer3VSM_Kromy::setUTSpeedHumPID_Kd(QVariant Kd  ){ _UTSpeedHumPID->setKd(Kd  .toDouble()); };
//void Dryer3VSM_Kromy::setUTSpeedHumPID_Td(QVariant Td  ){ _UTSpeedHumPID->setTd(Td  .toDouble()); };
//void Dryer3VSM_Kromy::setUTSpeedHumPID_MaxU(QVariant MaxU){ _UTSpeedHumPID->setMaxU(MaxU.toDouble()); };
//void Dryer3VSM_Kromy::setUTSpeedHumPID_MinU(QVariant MinU){ _UTSpeedHumPID->setMinU(MinU.toDouble()); }

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::_coolingTimerEnd()
{
    if(_BrMode == UnMdStop || _BrMode == UnMdCollingStop){
        _commandToSubUnitByType(TypeDryerFan, UnMdStop, false);
        _setblockSubUnitByType(TypeDryerFan, false);
    }
}

//------------------------------------------------------------------------------
void Dryer3VSM_Kromy::_customConnectToGUI(QObject *guiItem,  QObject *propWin)
{
    //---- Время зачистки ------
    QVariant ret;
    QObject * tmpSgSt, * engRow;

    connect(this, SIGNAL(Cleaning()         ), guiItem, SLOT(cleaningWork()), Qt::QueuedConnection);
    connect(this, SIGNAL(s_quitAlarm(QString)), this, SLOT(MoverAlarm()), Qt::QueuedConnection);

    QMetaObject::invokeMethod(propWin, "addEngRow", Qt::DirectConnection, Q_RETURN_ARG(QVariant, ret), Q_ARG(QVariant, this->objectName())); //создал строку меню задержки
    engRow = qvariant_cast< QObject* >(ret);
    QMetaObject::invokeMethod(engRow, "addPropertySetting", Qt::DirectConnection,
                              Q_RETURN_ARG(QVariant, ret),
                              Q_ARG(QVariant, this->objectName() + "_сool_delay" ),
                              Q_ARG(QVariant, "сек. - время продувки"),
                              Q_ARG(QVariant, 1));

    tmpSgSt = qvariant_cast< QObject* >(ret);//получаю указатель на зачистку
    //подключаю сигналы к зачистке
    connect(tmpSgSt, SIGNAL(changedVal(QVariant)), this,    SLOT(GetCoolDelay(QVariant)), Qt::QueuedConnection);
    connect(this,    SIGNAL(s_coolDelayChd(QVariant)),     tmpSgSt, SLOT(changeVal(QVariant)), Qt::QueuedConnection);
    connect(this->_coolingTimer,  SIGNAL(StartSig(QVariant)),            tmpSgSt, SLOT(startCountdown(QVariant)),       Qt::QueuedConnection);
    connect(this->_coolingTimer,  SIGNAL(StopSig() ), tmpSgSt, SLOT(stopCountdown()),       Qt::QueuedConnection);
    connect(this->_coolingTimer,  SIGNAL(timeout() ), tmpSgSt, SLOT(stopCountdown()),       Qt::QueuedConnection);
    //---- Время зачистки ------

    //создал строку меню задержки циклофенов
    QMetaObject::invokeMethod(propWin, "addEngRow", Qt::DirectConnection, Q_RETURN_ARG(QVariant, ret), Q_ARG(QVariant, this->objectName()));
    engRow = qvariant_cast< QObject* >(ret);
    QMetaObject::invokeMethod(engRow, "addPropertySetting", Qt::DirectConnection,
                              Q_RETURN_ARG(QVariant, ret),
                              Q_ARG(QVariant, this->objectName() + "_startCFDelay" ),
                              Q_ARG(QVariant, "сек. - задержка старта циклофенов"),
                              Q_ARG(QVariant, 1));

    tmpSgSt = qvariant_cast< QObject* >(ret);//получаю указатель
    //подключаю сигналы
    connect(tmpSgSt, SIGNAL(changedVal(QVariant)), this,    SLOT(GetStartCFDelay(QVariant)), Qt::QueuedConnection);
    connect(this,    SIGNAL(s_startCFDelayChd(QVariant)), tmpSgSt, SLOT(changeVal(QVariant)), Qt::QueuedConnection);
    connect(this->_startCFDelay,  SIGNAL(StartSig(QVariant)),            tmpSgSt, SLOT(startCountdown(QVariant)), Qt::QueuedConnection);
    connect(this->_startCFDelay,  SIGNAL(StopSig() ), tmpSgSt, SLOT(stopCountdown()),       Qt::QueuedConnection);
    connect(this->_startCFDelay,  SIGNAL(timeout() ), tmpSgSt, SLOT(stopCountdown()),  Qt::QueuedConnection);
    //---- Время циклофенов ------

    //создал строку меню задержки старта горелок
    QMetaObject::invokeMethod(propWin, "addEngRow", Qt::DirectConnection, Q_RETURN_ARG(QVariant, ret), Q_ARG(QVariant, this->objectName()));
    engRow = qvariant_cast< QObject* >(ret);
    QMetaObject::invokeMethod(engRow, "addPropertySetting", Qt::DirectConnection,
                              Q_RETURN_ARG(QVariant, ret),
                              Q_ARG(QVariant, this->objectName() + "_startBrDelay" ),
                              Q_ARG(QVariant, "сек. - задержка старта горелок"),
                              Q_ARG(QVariant, 1));

    tmpSgSt = qvariant_cast< QObject* >(ret);//получаю указатель
    //подключаю сигналы
    connect(tmpSgSt, SIGNAL(changedVal(QVariant)),            this,    SLOT(GetStartBrDelay(QVariant)), Qt::QueuedConnection);
    connect(this,    SIGNAL(s_startBrDelayChd(QVariant) ),      tmpSgSt, SLOT(changeVal(QVariant)), Qt::QueuedConnection);
    connect(this->_startBrDelay,  SIGNAL(StartSig(QVariant)), tmpSgSt, SLOT(startCountdown(QVariant)), Qt::QueuedConnection);
    connect(this->_startBrDelay,  SIGNAL(StopSig() ),         tmpSgSt, SLOT(stopCountdown()),       Qt::QueuedConnection);
    connect(this->_startBrDelay,  SIGNAL(timeout() ),         tmpSgSt, SLOT(stopCountdown()),  Qt::QueuedConnection);
    //---- Время  старта горелок ------

    //создал строку меню задержки стопа горелок
    QMetaObject::invokeMethod(propWin, "addEngRow", Qt::DirectConnection, Q_RETURN_ARG(QVariant, ret), Q_ARG(QVariant, this->objectName()));
    engRow = qvariant_cast< QObject* >(ret);
    QMetaObject::invokeMethod(engRow, "addPropertySetting", Qt::DirectConnection,
                              Q_RETURN_ARG(QVariant, ret),
                              Q_ARG(QVariant, this->objectName() + "_startBrDelay" ),
                              Q_ARG(QVariant, "сек. - задержка остановки горелок"),
                              Q_ARG(QVariant, 1));

    tmpSgSt = qvariant_cast< QObject* >(ret);//получаю указатель
    //подключаю сигналы
    connect(tmpSgSt, SIGNAL(changedVal(QVariant)),           this,    SLOT(setStopBrDelay(QVariant)), Qt::QueuedConnection);
    connect(this,    SIGNAL(s_stopBrDelayChd(QVariant) ),    tmpSgSt, SLOT(changeVal(QVariant)), Qt::QueuedConnection);
    connect(this->_stopBrDelay,  SIGNAL(StartSig(QVariant)), tmpSgSt, SLOT(startCountdown(QVariant)), Qt::QueuedConnection);
    connect(this->_stopBrDelay,  SIGNAL(StopSig() ),         tmpSgSt, SLOT(stopCountdown()),       Qt::QueuedConnection);
    connect(this->_stopBrDelay,  SIGNAL(timeout() ),         tmpSgSt, SLOT(stopCountdown()),  Qt::QueuedConnection);
    //---- Время стопа горелок ------

    //    connect(guiItem, SIGNAL(addToCurrentRoteStarted()   ), this, SLOT(AddToCurrentRoteStarted()),   Qt::QueuedConnection);
    //    connect(guiItem, SIGNAL(addToCurrentRoteStoped()  ), this, SLOT(AddToCurrentRoteStoped() ),   Qt::QueuedConnection);
    connect(this, SIGNAL(SetStatus(QVariant) ), guiItem, SLOT(setStatus(QVariant)), Qt::QueuedConnection);


    connect(_temp1 ,    SIGNAL(s_valueChd(QVariant)),  guiItem,  SLOT(setTemp1(QVariant)),   Qt::QueuedConnection);
    connect(_temp2 ,    SIGNAL(s_valueChd(QVariant)),  guiItem,  SLOT(setTemp2(QVariant)),   Qt::QueuedConnection);
    connect(_temp3 ,    SIGNAL(s_valueChd(QVariant)),  guiItem,  SLOT(setTemp3(QVariant)),   Qt::QueuedConnection);
    connect(_temp4 ,    SIGNAL(s_valueChd(QVariant)),  guiItem,  SLOT(setTemp4(QVariant)),   Qt::QueuedConnection);
    connect(_temp5 ,    SIGNAL(s_valueChd(QVariant)),  guiItem,  SLOT(setTemp5(QVariant)),   Qt::QueuedConnection);
    connect(_temp6 ,    SIGNAL(s_valueChd(QVariant)),  guiItem,  SLOT(setTemp6(QVariant)),   Qt::QueuedConnection);
    connect(_temp7 ,    SIGNAL(s_valueChd(QVariant)),  guiItem,  SLOT(setTemp7(QVariant)),   Qt::QueuedConnection);
    connect(_temp8 ,    SIGNAL(s_valueChd(QVariant)),  guiItem,  SLOT(setTemp8(QVariant)),   Qt::QueuedConnection);
    connect(_temp9 ,    SIGNAL(s_valueChd(QVariant)),  guiItem,  SLOT(setTemp9(QVariant)),   Qt::QueuedConnection);
    connect(_temp10,    SIGNAL(s_valueChd(QVariant)),  guiItem,  SLOT(setTemp10(QVariant)),  Qt::QueuedConnection);
    connect(_temp11,    SIGNAL(s_valueChd(QVariant)),  guiItem,  SLOT(setTemp11(QVariant)),  Qt::QueuedConnection);
    connect(_temp12,    SIGNAL(s_valueChd(QVariant)),  guiItem,  SLOT(setTemp12(QVariant)),  Qt::QueuedConnection);
    connect(_temp13,    SIGNAL(s_valueChd(QVariant)),  guiItem,  SLOT(setTemp13(QVariant)),  Qt::QueuedConnection);
    connect(_temp14,    SIGNAL(s_valueChd(QVariant)),  guiItem,  SLOT(setTemp14(QVariant)),  Qt::QueuedConnection);
    connect(_temp15,    SIGNAL(s_valueChd(QVariant)),  guiItem,  SLOT(setTemp15(QVariant)),  Qt::QueuedConnection);

    connect(_alarmTemp1 , SIGNAL(s_valueChd(QVariant)), guiItem,  SLOT(setAlarmTemp1(QVariant)),   Qt::QueuedConnection);
    connect(_alarmTemp2 , SIGNAL(s_valueChd(QVariant)), guiItem,  SLOT(setAlarmTemp2(QVariant)),   Qt::QueuedConnection);
    connect(_alarmTemp3 , SIGNAL(s_valueChd(QVariant)), guiItem,  SLOT(setAlarmTemp3(QVariant)),   Qt::QueuedConnection);
    connect(_alarmTemp4 , SIGNAL(s_valueChd(QVariant)), guiItem,  SLOT(setAlarmTemp4(QVariant)),   Qt::QueuedConnection);
    connect(_alarmTemp5 , SIGNAL(s_valueChd(QVariant)), guiItem,  SLOT(setAlarmTemp5(QVariant)),   Qt::QueuedConnection);
    connect(_alarmTemp6 , SIGNAL(s_valueChd(QVariant)), guiItem,  SLOT(setAlarmTemp6(QVariant)),   Qt::QueuedConnection);
    connect(_alarmTemp7 , SIGNAL(s_valueChd(QVariant)), guiItem,  SLOT(setAlarmTemp7(QVariant)),   Qt::QueuedConnection);
    connect(_alarmTemp8 , SIGNAL(s_valueChd(QVariant)), guiItem,  SLOT(setAlarmTemp8(QVariant)),   Qt::QueuedConnection);
    connect(_alarmTemp9 , SIGNAL(s_valueChd(QVariant)), guiItem,  SLOT(setAlarmTemp9(QVariant)),   Qt::QueuedConnection);
    connect(_alarmTemp10, SIGNAL(s_valueChd(QVariant)), guiItem,  SLOT(setAlarmTemp10(QVariant)),  Qt::QueuedConnection);
    connect(_alarmTemp11, SIGNAL(s_valueChd(QVariant)), guiItem,  SLOT(setAlarmTemp11(QVariant)),  Qt::QueuedConnection);
    connect(_alarmTemp12, SIGNAL(s_valueChd(QVariant)), guiItem,  SLOT(setAlarmTemp12(QVariant)),  Qt::QueuedConnection);
    connect(_alarmTemp13, SIGNAL(s_valueChd(QVariant)), guiItem,  SLOT(setAlarmTemp13(QVariant)),  Qt::QueuedConnection);
    connect(_alarmTemp14, SIGNAL(s_valueChd(QVariant)), guiItem,  SLOT(setAlarmTemp14(QVariant)),  Qt::QueuedConnection);
    connect(_alarmTemp15, SIGNAL(s_valueChd(QVariant)), guiItem,  SLOT(setAlarmTemp15(QVariant)),  Qt::QueuedConnection);

    connect(guiItem, SIGNAL(alarmTemp1Changed(QVariant)), _alarmTemp1 ,  SLOT(setValue(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(alarmTemp2Changed(QVariant)), _alarmTemp2 ,  SLOT(setValue(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(alarmTemp3Changed(QVariant)), _alarmTemp3 ,  SLOT(setValue(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(alarmTemp4Changed(QVariant)), _alarmTemp4 ,  SLOT(setValue(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(alarmTemp5Changed(QVariant)), _alarmTemp5 ,  SLOT(setValue(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(alarmTemp6Changed(QVariant)), _alarmTemp6 ,  SLOT(setValue(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(alarmTemp7Changed(QVariant)), _alarmTemp7 ,  SLOT(setValue(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(alarmTemp8Changed(QVariant)), _alarmTemp8 ,  SLOT(setValue(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(alarmTemp9Changed(QVariant)), _alarmTemp9 ,  SLOT(setValue(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(alarmTemp10Changed(QVariant)), _alarmTemp10,  SLOT(setValue(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(alarmTemp11Changed(QVariant)), _alarmTemp11,  SLOT(setValue(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(alarmTemp12Changed(QVariant)), _alarmTemp12,  SLOT(setValue(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(alarmTemp13Changed(QVariant)), _alarmTemp13,  SLOT(setValue(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(alarmTemp14Changed(QVariant)), _alarmTemp14,  SLOT(setValue(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(alarmTemp15Changed(QVariant)), _alarmTemp15,  SLOT(setValue(QVariant)), Qt::QueuedConnection);

    connect(guiItem, SIGNAL(start()),       this, SLOT(Start()),    Qt::QueuedConnection);
    connect(guiItem, SIGNAL(stop()),        this, SLOT(Stop()),     Qt::QueuedConnection);
    connect(guiItem, SIGNAL(cooling()),     this, SLOT(Cooling()),  Qt::QueuedConnection);
    connect(guiItem, SIGNAL(coolingStop()), this, SLOT(CoolingStop()),  Qt::QueuedConnection);

    //    connect(this,    SIGNAL(Stoped()),  guiItem, SLOT(stoped()),  Qt::QueuedConnection);
    //    connect(this,    SIGNAL(Started()), guiItem, SLOT(started()), Qt::QueuedConnection);

    connect(_needUnload, SIGNAL(s_valueChd(QVariant)), guiItem,  SLOT(setUnload(QVariant) ), Qt::QueuedConnection);
    connect(_needCircul, SIGNAL(s_valueChd(QVariant)), guiItem,  SLOT(setCircule(QVariant)), Qt::QueuedConnection);
    connect(this,        SIGNAL(s_passLoad(QVariant)), guiItem,  SLOT(setAutoLoad(QVariant)   ), Qt::QueuedConnection);

    connect(guiItem, SIGNAL(passUnload(bool)  ), this,     SLOT(setUnload(bool)  ), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(passCircule(bool) ), this,     SLOT(setCircule(bool) ), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(passLoad(bool)    ), this,     SLOT(setAutoLoad(bool)    ), Qt::QueuedConnection);


    connect(_inHumidity,   SIGNAL(s_valueChd(QVariant)),    guiItem, SLOT(setHrIn(QVariant) ),          Qt::QueuedConnection);
    connect(_outHumidity,  SIGNAL(s_valueChd(QVariant)),    guiItem, SLOT(setHrOut(QVariant)),          Qt::QueuedConnection);

    connect(guiItem, SIGNAL(s_brnrTempProdTempPID_KpChd(QVariant)), this, SLOT(setBrnrTempProdTempPID_Kp(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(s_brnrTempProdTempPID_KiChd(QVariant)), this, SLOT(setBrnrTempProdTempPID_Ki(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(s_brnrTempProdTempPID_KdChd(QVariant)), this, SLOT(setBrnrTempProdTempPID_Kd(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(s_brnrTempProdTempPID_TdChd(QVariant)), this, SLOT(setBrnrTempProdTempPID_Td(QVariant)), Qt::QueuedConnection);
    //connect(guiItem, SIGNAL(s_brnrTempProdTempPID_MaxUChd(QVariant)), this, SLOT(setBrnrTempProdTempPID_MaxU(QVariant)), Qt::QueuedConnection);
    //connect(guiItem, SIGNAL(s_brnrTempProdTempPID_MinUChd(QVariant)), this, SLOT(setBrnrTempProdTempPID_MinU(QVariant)), Qt::QueuedConnection);

    connect(guiItem, SIGNAL(s_prodTempHumPID_KpChd(QVariant)), this, SLOT(setProdTempHumPID_Kp(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(s_prodTempHumPID_KiChd(QVariant)), this, SLOT(setProdTempHumPID_Ki(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(s_prodTempHumPID_KdChd(QVariant)), this, SLOT(setProdTempHumPID_Kd(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(s_prodTempHumPID_TdChd(QVariant)), this, SLOT(setProdTempHumPID_Td(QVariant)), Qt::QueuedConnection);
    //connect(guiItem, SIGNAL(s_prodTempHumPID_MaxUChd(QVariant)), this, SLOT(setProdTempHumPID_MaxU(QVariant)), Qt::QueuedConnection);
    //connect(guiItem, SIGNAL(s_prodTempHumPID_MinUChd(QVariant)), this, SLOT(setProdTempHumPID_MinU(QVariant)), Qt::QueuedConnection);

    //    connect(guiItem, SIGNAL(s_UTSpeedHumPID_KpChd(QVariant)), this, SLOT(setUTSpeedHumPID_Kp(QVariant)), Qt::QueuedConnection);
    //    connect(guiItem, SIGNAL(s_UTSpeedHumPID_KiChd(QVariant)), this, SLOT(setUTSpeedHumPID_Ki(QVariant)), Qt::QueuedConnection);
    //    connect(guiItem, SIGNAL(s_UTSpeedHumPID_KdChd(QVariant)), this, SLOT(setUTSpeedHumPID_Kd(QVariant)), Qt::QueuedConnection);
    //    connect(guiItem, SIGNAL(s_UTSpeedHumPID_TdChd(QVariant)), this, SLOT(setUTSpeedHumPID_Td(QVariant)), Qt::QueuedConnection);
    //connect(guiItem, SIGNAL(s_UTSpeedHumPID_MaxUChd(QVariant)), this, SLOT(setUTSpeedHumPID_MaxU(QVariant)), Qt::QueuedConnection);
    //connect(guiItem, SIGNAL(s_UTSpeedHumPID_MinUChd(QVariant)), this, SLOT(setUTSpeedHumPID_MinU(QVariant)), Qt::QueuedConnection);

    connect(this, SIGNAL(s_brnrTempProdTempPID_KpChd(QVariant)), guiItem, SLOT(setBrnrTempProdTempPID_Kp(QVariant)), Qt::QueuedConnection);
    connect(this, SIGNAL(s_brnrTempProdTempPID_KiChd(QVariant)), guiItem, SLOT(setBrnrTempProdTempPID_Ki(QVariant)), Qt::QueuedConnection);
    connect(this, SIGNAL(s_brnrTempProdTempPID_KdChd(QVariant)), guiItem, SLOT(setBrnrTempProdTempPID_Kd(QVariant)), Qt::QueuedConnection);
    connect(this, SIGNAL(s_brnrTempProdTempPID_TdChd(QVariant)), guiItem, SLOT(setBrnrTempProdTempPID_Td(QVariant)), Qt::QueuedConnection);
    //connect(this, SIGNAL(s_brnrTempProdTempPID_MaxUChd(QVariant)), guiItem, SLOT(setBrnrTempProdTempPID_MaxU(QVariant)), Qt::QueuedConnection);
    //connect(this, SIGNAL(s_brnrTempProdTempPID_MinUChd(QVariant)), guiItem, SLOT(setBrnrTempProdTempPID_MinU(QVariant)), Qt::QueuedConnection);

    connect(this, SIGNAL(s_prodTempHumPID_KpChd(QVariant)), guiItem, SLOT(setProdTempHumPID_Kp(QVariant)), Qt::QueuedConnection);
    connect(this, SIGNAL(s_prodTempHumPID_KiChd(QVariant)), guiItem, SLOT(setProdTempHumPID_Ki(QVariant)), Qt::QueuedConnection);
    connect(this, SIGNAL(s_prodTempHumPID_KdChd(QVariant)), guiItem, SLOT(setProdTempHumPID_Kd(QVariant)), Qt::QueuedConnection);
    connect(this, SIGNAL(s_prodTempHumPID_TdChd(QVariant)), guiItem, SLOT(setProdTempHumPID_Td(QVariant)), Qt::QueuedConnection);
    //connect(this, SIGNAL(s_prodTempHumPID_MaxUChd(QVariant)), guiItem, SLOT(setProdTempHumPID_MaxU(QVariant)), Qt::QueuedConnection);
    //connect(this, SIGNAL(s_prodTempHumPID_MinUChd(QVariant)), guiItem, SLOT(setProdTempHumPID_MinU(QVariant)), Qt::QueuedConnection);

    //    connect(this, SIGNAL(s_UTSpeedHumPID_KpChd(QVariant)), guiItem, SLOT(setUTSpeedHumPID_Kp(QVariant)), Qt::QueuedConnection);
    //    connect(this, SIGNAL(s_UTSpeedHumPID_KiChd(QVariant)), guiItem, SLOT(setUTSpeedHumPID_Ki(QVariant)), Qt::QueuedConnection);
    //    connect(this, SIGNAL(s_UTSpeedHumPID_KdChd(QVariant)), guiItem, SLOT(setUTSpeedHumPID_Kd(QVariant)), Qt::QueuedConnection);
    //    connect(this, SIGNAL(s_UTSpeedHumPID_TdChd(QVariant)), guiItem, SLOT(setUTSpeedHumPID_Td(QVariant)), Qt::QueuedConnection);
    //connect(this, SIGNAL(s_UTSpeedHumPID_MaxUChd(QVariant)), guiItem, SLOT(setUTSpeedHumPID_MaxU(QVariant)), Qt::QueuedConnection);
    //connect(this, SIGNAL(s_UTSpeedHumPID_MinUChd(QVariant)), guiItem, SLOT(setUTSpeedHumPID_MinU(QVariant)), Qt::QueuedConnection);

    connect(this, SIGNAL(s_maxBernerTempChd(QVariant)), guiItem, SLOT(setMaxBernerTemp(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(s_maxBernerTempChd(QVariant)), this, SLOT(setMaxBernerTemp(QVariant)), Qt::QueuedConnection);

    connect(this, SIGNAL(s_maxProductTempChd(QVariant)), guiItem, SLOT(setMaxProductTemp(QVariant)), Qt::QueuedConnection);
    connect(this, SIGNAL(s_maxUTSpeedChd(QVariant)), guiItem, SLOT(setMaxUTSpeed(QVariant)), Qt::QueuedConnection);

    connect(guiItem, SIGNAL(s_maxProductTempChd(QVariant)), this, SLOT(setMaxProductTemp(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(s_maxUTSpeedChd(QVariant)), this, SLOT(setMaxUTSpeed(QVariant)), Qt::QueuedConnection);

    connect(this,    SIGNAL(s_targetHumidityChd(QVariant)), guiItem, SLOT(setTargetHumidity(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(s_targetHumidityChd(QVariant)), this,    SLOT(setTargetHumidity(QVariant)), Qt::QueuedConnection);

    connect(this,    SIGNAL(s_targetLowTempChd(QVariant)), guiItem, SLOT(setTargetTempLow(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(s_targetTempLowChd(QVariant)), this,    SLOT(setTargetLowTemp(QVariant)), Qt::QueuedConnection);

    connect(this,    SIGNAL(s_autoHumidityChd(QVariant)), guiItem, SLOT(setAutoHumidity(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(s_autoHumidityChd(bool)), this,    SLOT(setAutoHumidity(bool)), Qt::QueuedConnection);

    connect(this,    SIGNAL(s_autoTempChd(QVariant)),     guiItem, SLOT(setAutoTemp(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(s_autoTempChd(bool)),     this,    SLOT(setAutoTemp(bool)), Qt::QueuedConnection);
}

//------------------------------------------------------------------------------



































