#include <QSettings>
#include "Druer1Veles.h"
#include "InDiscretETag.h"
#include "OutDiscretETag.h"
#include "InETag.h"
#include "OutETag.h"
#include "static2posunit.h"


Druer1VSM_Veles::Druer1VSM_Veles( int ID,
                                  QString Name,
                                  QString TagPefix )
    :RouteUnit( Prom::TypeSilos,
                ID,
                Name,
                TagPefix,
                false,
                Prom::UnMdStop )
{

    _alarmSound = new OutDiscretETag( this, Prom::PreSet, "Сирена аварии",       ".vsAlarm");
    _startSound = new OutDiscretETag( this, Prom::PreSet, "Предпусковая сирена", ".vsStart", true, false,
                                      false, true, false, false, false, false, true );
    _start      = new OutDiscretETag( this, Prom::PreSet, "Пуск", ".start");
    _stop       = new OutDiscretETag( this, Prom::PreSet, "Стоп", ".stop");
    _fire       = new OutDiscretETag( this, Prom::PreSet, "Пожар", ".fire");
    _alarmOut   = new OutDiscretETag( this, Prom::PreSet, "Авария исх.", ".alarm" );

    _alarmIn   = new InDiscretETag( this, "Авария вх.", ".alarm", true, false, true, false );
    _alarmIn->needBeUndetectedAlarmNoTime();

    _tempHL    = new InETag( this, Prom::TpIn, "Датчик t°C верх",                    ".tempHL",  true, 30, 5, false, false, false, false, true, Prom::VCFloatInIntToDouble );
    _tempML    = new InETag( this, Prom::TpIn, "Датчик t°C середина",                ".tempML",  true, 30, 5, false, false, false, false, true, Prom::VCFloatInIntToDouble );
    _tempLL    = new InETag( this, Prom::TpIn, "Датчик t°C зона охлаждения",         ".tempLL",  true, 30, 5, false, false, false, false, true, Prom::VCFloatInIntToDouble );
    _tempEx    = new InETag( this, Prom::TpIn, "Датчик t°C теплоносителя на выходе", ".tempEx",  true, 30, 5, false, false, false, false, true, Prom::VCFloatInIntToDouble );

    _tempHL->needBeUndetectedAlarm();
    _tempML->needBeUndetectedAlarm();
    _tempLL->needBeUndetectedAlarm();
    _tempEx->needBeUndetectedAlarm();

    _overTempHL  = new OutETag( this, Prom::TpOut, Prom::PreSet, "Уставка порога t°C верх",                     ".overTempHL",  false, false, false, true, Prom::VCFloatInIntToDouble, true );
    _overTempML  = new OutETag( this, Prom::TpOut, Prom::PreSet, "Уставка порога t°C середина",                 ".overTempML",  false, false, false, true, Prom::VCFloatInIntToDouble, true );
    _overTempLL  = new OutETag( this, Prom::TpOut, Prom::PreSet, "Уставка порога t°C зона охлаждения",          ".overTempLL",  false, false, false, true, Prom::VCFloatInIntToDouble, true );
    _overTempEx  = new OutETag( this, Prom::TpOut, Prom::PreSet, "Уставка порога t°C теплоносителя на выходе",  ".overTempEx",  false, false, false, true, Prom::VCFloatInIntToDouble, true );

    connect( _overTempHL , &OutETag::s_valueChd, _tempHL , &InETag::setDetectLevel );
    connect( _overTempML , &OutETag::s_valueChd, _tempML , &InETag::setDetectLevel );
    connect( _overTempLL , &OutETag::s_valueChd, _tempLL , &InETag::setDetectLevel );
    connect( _overTempEx , &OutETag::s_valueChd, _tempEx , &InETag::setDetectLevel );

    //LoadParam();
}

//------------------------------------------------------------------------------
bool Druer1VSM_Veles::addSubUnit(Unit *unit)
{
    if(  unit->unitType == TypeDryerFan ){
        CFs.append( unit );
    }
    if( unit->unitType == TypeDryer3_Bunker ){
        _bunker = unit;
        //if( _UnloadTable ) break;
    }
    if( unit->unitType == TypeDryerUnloadTable ){
        _unloadTable = unit;
        //if( _bunker )break;
    }
    if( unit->unitType == TypeBurner ){
        _burner = unit;
    }
    return Unit::addSubUnit( unit );
}

//------------------------------------------------------------------------------
void Druer1VSM_Veles::_updateStateAndMode()
{

}

//------------------------------------------------------------------------------
void Druer1VSM_Veles::CoolingStop()
{
    setMode( Prom::UnMdCollingStop, true );
}

//------------------------------------------------------------------------------
Prom::SetModeResp Druer1VSM_Veles::_customSetMode( Prom::UnitModes *mode, bool UserOrSys )
{
    static Prom::SetModeResp respTMP;

    switch ( *mode ) {
    case Prom::UnMdStart :
        if( _alarm ){
            return Prom::RejAlarm;
        }
        else {
            _stop->off();
            if( _start->on() ){
                _startSound->on();
                //QTimer::singleShot( 3000, [ this ](  ) { this->_startSound->Off(); } );
                if( _burner ){
                    if( _burner->currentMode() == Prom::UnMdStart ){
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
    case Prom::UnMdStop :
        _start->off();
        _stop->off();
        _fire->on();
        //QTimer::singleShot( 5000, [ this ](  ) { this->_fire->Off(); } );
        respTMP = Prom::DoneAlready;
        break;
    case Prom::UnMdCollingStop :
        _start->off();
        _stop->on();
        //QTimer::singleShot( 5000, [ this ](  ) { this->_stop->Off(); } );
        break;
    default : return RejAnnown;
    }
    return respTMP;
}
//------------------------------------------------------------------------------
void Druer1VSM_Veles::_alarmSubUnitDo( Unit * unit )
{

    if( unit->unitType == Prom::TypeBurner ){
        if( currentMode() == Prom::UnMdStart || setedMode() == Prom::UnMdStart )
            setMode( Prom::UnMdStop, false );
    }
    else if( unit->unitType == Prom::TypeDryerFan && ! _burner->isAlarm()  ){
        if( currentMode() == Prom::UnMdStart || setedMode() == Prom::UnMdStart )
            setMode( Prom::UnMdCollingStop, false );
    }
    else if( _unloadTable && ! _alarm ) {
        if( currentMode() == Prom::UnMdStart || setedMode() == Prom::UnMdStart )
           _unloadTable->setMode( Prom::UnMdCollingStop, true);
   }
}

//------------------------------------------------------------------------------
void Druer1VSM_Veles::_alarmDo()
{
    if( _alarmOut )_alarmOut->on();
    if( _alarmSound ) _alarmSound->on();
    if( currentMode() == Prom::UnMdStart ){
        setMode( Prom::UnMdCollingStop, false );
    }
}
//------------------------------------------------------------------------------
void Druer1VSM_Veles::_resetAlarmDo()
{
    if( _alarmOut ) _alarmOut->off();
    if( _alarmSound ) _alarmSound->off();
    if( _stop ) _stop->off();
}

//------------------------------------------------------------------------------
void Druer1VSM_Veles::_setblockSubUnitByType( UnitType type, bool block )
{
    foreach( Unit * unit, _subUnits  ){
        if( unit->unitType == type ){
            unit->setBlocked( block );
        }
    }
}

//------------------------------------------------------------------------------
UnitModes Druer1VSM_Veles::_chackModByType( Prom::UnitType type )
{
    static Prom::UnitModes modeTMP;
    modeTMP = Prom::UnMdNoDef;
    bool katch = false;
    for( int i = 0; i < _subUnits.count(); i ++  ){
        if( _subUnits[i]->unitType == type ){
            if( katch ){
                if( _subUnits[i]->currentMode() != modeTMP )
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
void Druer1VSM_Veles::_updateSubUnitMode( Unit * unit )
{
    if( unit->unitType == Prom::TypeBurner ){
        if( unit->currentMode() == Prom::UnMdStart ){
            _setCurrentMode( Prom::UnMdStart );
            emit Started();
        }
        else {
            _setCurrentMode( Prom::UnMdStop );
            emit Stoped();
        }
    }
}

//------------------------------------------------------------------------------
void Druer1VSM_Veles::_updateSubUnitState( Unit *unit )
{

}

//------------------------------------------------------------------------------
void Druer1VSM_Veles::_customConnectToGUI( QObject *guiItem,  QObject *propWin )
{
    //    connect( guiItem, SIGNAL( addToCurrentRoteStarted() ), this, SLOT( AddToCurrentRoteStarted() ), Qt::QueuedConnection );
    //    connect( guiItem, SIGNAL( addToCurrentRoteStoped()  ), this, SLOT( AddToCurrentRoteStoped()  ),   Qt::QueuedConnection );

    connect( _tempHL,      SIGNAL( s_valueChd( QVariant ) ), guiItem, SLOT( setTempHL( QVariant ) ), Qt::QueuedConnection );
    connect( _tempML,      SIGNAL( s_valueChd( QVariant ) ), guiItem, SLOT( setTempML( QVariant ) ), Qt::QueuedConnection );
    connect( _tempLL,      SIGNAL( s_valueChd( QVariant ) ), guiItem, SLOT( setTempLL( QVariant ) ), Qt::QueuedConnection );
    connect( _tempEx,      SIGNAL( s_valueChd( QVariant ) ), guiItem, SLOT( setTempEX( QVariant ) ), Qt::QueuedConnection );

    connect( _overTempHL, SIGNAL( s_valueChd( QVariant ) ), guiItem, SLOT( setOverTempHL( QVariant ) ), Qt::QueuedConnection );
    connect( _overTempML, SIGNAL( s_valueChd( QVariant ) ), guiItem, SLOT( setOverTempML( QVariant ) ), Qt::QueuedConnection );
    connect( _overTempLL, SIGNAL( s_valueChd( QVariant ) ), guiItem, SLOT( setOverTempLL( QVariant ) ), Qt::QueuedConnection );
    connect( _overTempEx, SIGNAL( s_valueChd( QVariant ) ), guiItem, SLOT( setOverTempEx( QVariant ) ), Qt::QueuedConnection );

    connect( guiItem,      SIGNAL( overTempHLChanged( QVariant ) ), _overTempHL, SLOT( setValue( QVariant ) ), Qt::QueuedConnection );
    connect( guiItem,      SIGNAL( overTempMLChanged( QVariant ) ), _overTempML, SLOT( setValue( QVariant ) ), Qt::QueuedConnection );
    connect( guiItem,      SIGNAL( overTempLLChanged( QVariant ) ), _overTempLL, SLOT( setValue( QVariant ) ), Qt::QueuedConnection );
    connect( guiItem,      SIGNAL( overTempExChanged( QVariant ) ), _overTempEx, SLOT( setValue( QVariant ) ), Qt::QueuedConnection );

    connect( guiItem, SIGNAL( start() ),   this, SLOT( Start() ),       Qt::QueuedConnection );
    connect( guiItem, SIGNAL( stop() ),    this, SLOT( Stop() ),        Qt::QueuedConnection );
    connect( guiItem, SIGNAL( cooling() ), this, SLOT( CoolingStop() ), Qt::QueuedConnection );

    connect( this,    SIGNAL( Stoped() ),  guiItem, SLOT( stoped() ),  Qt::QueuedConnection );
    connect( this,    SIGNAL( Started() ), guiItem, SLOT( started() ), Qt::QueuedConnection );
}

//------------------------------------------------------------------------------
