#include "BurnerVeles.h"
#include "InDiscretETag.h"
#include <QSettings>
#include "OutETag.h"

BurnerVeles::BurnerVeles( int ID,
                          QString Name,
                          QString TagPefix )
    :Unit( Prom::TypeBurner,
           ID,
           Name,
           TagPefix,
           false,
           Prom::UnMdStop )
{
    _setT = new OutETag( this, Prom::TpOut, Prom::PreSet, "Уставка t°C", ".setTemp", false, false, false, true, Prom::VCFloatInIntToDouble, true );

    _T = new InETag( this, Prom::TpIn, "Датчик t°C", ".temp", true, 90, 5, false, false, true, false, true, Prom::VCFloatInIntToDouble );
    _T->needBeUndetectedAlarm();
    //connect( _T, &InETag::s_valueChd, this, &BurnerVeles::SetCurrentTemp );

    _BRW = new InDiscretETag( this, "Работа горелки", ".work", true, false, true, false, false, false, true );
    _BRW->onlyChange();

    connect( _BRW, &InDiscretETag::s_qualityChd, this, &BurnerVeles::updateState );
    connect( _BRW, &InDiscretETag::s_valueChd, this, &BurnerVeles::updateState );

    _BRA = new InDiscretETag( this, "Авария горелки", ".alarmS", true, false, true, false, false, false );
    _BRA->needBeUndetectedAlarm();
}

//------------------------------------------------------------------------------
void BurnerVeles::_updateStateAndMode()
{
    if( _BRW->connected() ){
        if( _BRW->isDetected() ){
            _setCurrentState( Prom::UnStStarted );
            _setCurrentMode( Prom::UnMdStart );
            emit Started();
        }
        else {
            _setCurrentState( Prom::UnStStoped );
            _setCurrentMode( Prom::UnMdStop );
            emit Stoped();
        }
    }
    else {
        _setCurrentState( Prom::UnStNotConnected );
        _setCurrentMode( Prom::UnMdStop );
        emit NoDef();
    }
}

//-----------------------------------------------------------------------------------
void BurnerVeles::_customConnectToGUI( QObject *guiItem,  QObject *propWin )
{
    connect( _T,      SIGNAL( s_valueChd( QVariant ) ),                guiItem, SLOT( setTemp( QVariant ) ),     Qt::QueuedConnection );

    connect( guiItem, SIGNAL( overTempChanged ( QVariant ) ), _T, SLOT( setDetectLevel( QVariant ) ),       Qt::QueuedConnection );
    connect( _T,      SIGNAL( ChangeDelectLewel( QVariant ) ),      guiItem, SLOT( setOverTemp( QVariant ) ), Qt::QueuedConnection );

    connect( _setT,   SIGNAL( s_valueChd( QVariant ) ),            guiItem, SLOT( setTargetTemp( QVariant ) ), Qt::QueuedConnection );
    connect( guiItem, SIGNAL( targetTempChanged ( QVariant ) ), _setT,   SLOT( setValue( QVariant ) ),      Qt::QueuedConnection );

        connect( this, SIGNAL( Stoped() ),        guiItem, SLOT( stoped() ),     Qt::QueuedConnection );
        connect( this, SIGNAL( Started() ), guiItem, SLOT( started() ) ,Qt::QueuedConnection );
}

//-----------------------------------------------------------------------------------


















