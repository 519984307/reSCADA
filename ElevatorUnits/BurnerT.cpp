#include "BurnerT.h"
#include "InDiscretETag.h"
#include <QSettings>
#include "OutETag.h"

BurnerT::BurnerT( int ID,
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
    _setT = new OutETag( this, Prom::TpOut, Prom::PreSet, "Уставка t°C", ".setT", false, false, false, true, Prom::VCFloatInIntToDouble, true );

    _T = new InETag( this, Prom::TpIn, "Датчик t°C", ".T", true, 30, 5, false, false, true, false, true, Prom::VCFloatInIntToDouble );
    _T->needBeUndetectedAlarm();
    connect( _T, &InETag::s_valueChd, this, &BurnerT::SetCurrentTemp );

    _BRW = new InDiscretETag( this, "Работа горелки", ".BRW", true, false, true, false, false, false, true );
    _BRW->onlyChange();

    if( _BRW->connected() ) _currentState = Prom::UnStManualMove;
    else _currentState = Prom::UnStNotConnected;

    connect( _BRW, &InDiscretETag::s_qualityChd, this, &BurnerT::updateState );
    connect( _BRW, &InDiscretETag::s_valueChd, this, &BurnerT::updateState );

    _BRA = new InDiscretETag( this, "Авария горелки", ".BRA", true, false, true, false, false, false );
    _BRA->needBeUndetectedAlarm();

    _QK = new InDiscretETag( this, "Защитный автомат", ".QK", true, false, false, false, false );
    _QK->needBeUndetectedAlarm();
}

//------------------------------------------------------------------------------
void BurnerT::_updateStateAndMode()
{
    if( _BRW->connected() ){
        if( _BRW->isDetected() ){
            _currentState = Prom::UnStManualStarted;
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
void BurnerT::_customConnectToGUI( QObject *guiItem,  QObject * )
{
    connect( _T,      SIGNAL( s_valueChd( QVariant ) ),        guiItem, SLOT( setTemp( QVariant ) ),             Qt::QueuedConnection );
    connect( guiItem, SIGNAL( alarmTempChanged ( QVariant ) ), _T,      SLOT( setDetectLevel( QVariant ) ),      Qt::QueuedConnection );
    connect( _T,      SIGNAL( ChangeDelectLewel( QVariant ) ), guiItem, SLOT( setBurnerLimitTemp( QVariant ) ), Qt::QueuedConnection );

    connect( _setT,   SIGNAL( s_valueChd( QVariant ) ),      guiItem, SLOT( setTargetTemp( QVariant ) ), Qt::QueuedConnection );
    connect( guiItem, SIGNAL( setTempChanged ( QVariant ) ), _setT,  SLOT( setValue( QVariant ) ),       Qt::QueuedConnection );

    connect( this,    SIGNAL( Stoped() ),        guiItem, SLOT( stoped() ),      Qt::QueuedConnection );
    connect( this,    SIGNAL( ManualStarted() ), guiItem, SLOT( manualWork() ) ,Qt::QueuedConnection );
}

//-----------------------------------------------------------------------------------


















