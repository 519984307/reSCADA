#include "BurnerSimp.h"
#include "InDiscretETag.h"
#include "OutDiscretETag.h"
#include <QSettings>
#include "OutETag.h"

BurnerSimp::BurnerSimp( int ID,
                QString Name,
                QString TagPefix,
                Prom::UnitModes SaveMode
                )
    :Unit( Prom::TypeBurner,
           ID,
           Name,
           TagPefix,
           false,
           SaveMode )
{

    _startLG = new OutDiscretETag( this, Prom::PreSet, "Пуск горелки", ".startLG" );
    _startedLG= new InDiscretETag( this, "Работа горелки", ".startedLG", true, false, true, false, false, false, true );
    _startedLG->onlyChange();

    if( _startedLG->connected() ) _currentState = Prom::UnStManualMove;
    else _currentState = Prom::UnStNotConnected;

    connect( _startedLG, &InDiscretETag::s_qualityChd, this, &BurnerSimp::updateState );
    connect( _startedLG, &InDiscretETag::s_valueChd, this, &BurnerSimp::updateState );

    _setT = new OutETag( this, Prom::TpOut, Prom::PreSet, "Необходимая t°C теплоносителя после горелки", ".setTemp", false, false, false, true, Prom::VCFloatInIntToDouble, true );

    _T = new InETag( this, Prom::TpIn, "t°C теплоносителя после горелки", ".temp", true, 90, 1, false, false, false, false, true, Prom::VCFloatInIntToDouble );
    _T->needBeUndetectedAlarm();

    _TAlarmLvl  = new OutETag( this, Prom::TpOut, Prom::PreSet, "t°C перегрева теплоносителя после горелки",  ".tempAlarmLvl",  false, false, false, true, Prom::VCFloatInIntToDouble, true );
    connect( _TAlarmLvl, &OutETag::s_valueChd,  _T, &InETag::setDetectLevel );
    _alarmLG = new InDiscretETag( this, "Авария горелки", ".alarmLG", true, false, true, false, false, false );
    _alarmLG->needBeUndetectedAlarm();

    _QF = new InDiscretETag( this, "Защитный автомат", ".QF", true, false, false, false, false,
                             false );
    _QF->needBeUndetectedAlarm();

    _step = new InDiscretETag( this, "Работа 2-й ступени", ".step", true, false, true, false, false, false );
    _step->onlyChange();

    _P = new OutETag( this, Prom::TpOut, Prom::PreSet, "ПИД - пропорциональная часть", ".PID_P", false, false, false, true, Prom::VCFloatInIntToDouble, false, true );
    _I = new OutETag( this, Prom::TpOut, Prom::PreSet, "ПИД - интегральная часть"    , ".PID_I", false, false, false, true, Prom::VCNo, false, true );
    _D = new OutETag( this, Prom::TpOut, Prom::PreSet, "ПИД - дифференциальная часть", ".PID_D", false, false, false, true, Prom::VCFloatInIntToDouble, false, true );
}

//------------------------------------------------------------------------------
void BurnerSimp::_updateStateAndMode()
{
    if( _startedLG->connected() ){
        if( _startedLG->isDetected() ){
            _currentState = Prom::UnStStarted;
            _currentMode = Prom::UnMdStart;
            emit ManualStarted();
        }
        else {
            _currentState = Prom::UnStStoped;
            _currentMode = Prom::UnMdStop;
            emit Stoped();
        }
    }
    else {
        _currentState = Prom::UnStNotConnected;
        _currentMode = Prom::UnMdStop;
        emit NoDef();
    }
}

//-----------------------------------------------------------------------------------
void BurnerSimp::_customConnectToGUI( QObject *guiItem,  QObject * )
{
    connect( _T,         SIGNAL( s_valueChd( QVariant ) ),          guiItem, SLOT( setTemp( QVariant ) ),            Qt::QueuedConnection );

    connect( guiItem,    SIGNAL( overTempChanged ( QVariant ) ), _TAlarmLvl, SLOT( setValue( QVariant ) ),    Qt::QueuedConnection );
    connect( _TAlarmLvl, SIGNAL( s_valueChd( QVariant ) ),          guiItem,    SLOT( setOverTemp( QVariant ) ), Qt::QueuedConnection );

    connect( _setT,   SIGNAL( s_valueChd( QVariant ) ),             guiItem, SLOT( setTargetTemp( QVariant ) ), Qt::QueuedConnection );
    connect( guiItem, SIGNAL( targetTempChanged ( QVariant ) ),  _setT,   SLOT( setValue( QVariant ) ),      Qt::QueuedConnection );

    connect( this, SIGNAL( Stoped() ),        guiItem, SLOT( stoped() ),     Qt::QueuedConnection );
    connect( this, SIGNAL( ManualStarted() ), guiItem, SLOT( manualWork() ) ,Qt::QueuedConnection );

    connect( _T, SIGNAL( Alarm( QString ) ), guiItem, SLOT( setTempAlarm  () ), Qt::QueuedConnection );
    connect( _T, SIGNAL( AlarmReseted() ),   guiItem, SLOT( resetTempAlarm() ), Qt::QueuedConnection );

    connect( _step, SIGNAL( s_valueChd( QVariant ) ), guiItem, SLOT( setStep( QVariant ) ), Qt::QueuedConnection );
}

//-----------------------------------------------------------------------------------


















