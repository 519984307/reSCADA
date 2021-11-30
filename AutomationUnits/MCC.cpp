#include "MCC.h"
#include "InDiscretETag.h"
#include <QSettings>
#include "promobject.h"

MCC::MCC( int ID,
          QString Name,
          QString TagPefix,
          Prom::UnitModes SaveMode
          )
    :Unit( Prom::TypeMMC,
           ID,
           Name,
           TagPefix,
           false,
           SaveMode )
{
    _setCurrentMode( Prom::UnMdStop );


    _MMCmode = new InDiscretETag( this, "Режим ЩСУ", ".MD", true, false, false, false, false, false );
    _MMCmode->onlyChange();

    if( _MMCmode->connected() ) _setCurrentState( Prom::UnStManualMove );
    else _setCurrentState( Prom::UnStNotConnected );

    connect( _MMCmode, &InDiscretETag::s_qualityChd, this, &MCC::updateState );
    connect( _MMCmode, &InDiscretETag::Detected, this, &MCC::updateState );
    connect( _MMCmode, &InDiscretETag::Undetected, this, &MCC::updateState );
    //connect( this, &MCC::ManualStarted, owner, &PromObject::SysGlobalStop, Qt::QueuedConnection );
}
//------------------------------------------------------------------------------
void MCC::_updateStateAndMode()
{
    if( _MMCmode->connected() ){

        if( _MMCmode->isDetected() ){
            _setCurrentState( Prom::UnStStarted );
            _setCurrentMode( Prom::UnMdStart );
            emit Started();
        }
        else {
            if( currentState() == Prom::UnStNotConnected ){
                if( _resetAlarm() ){
                    _setCurrentState( Prom::UnStStoped );
                    setMode( Prom::UnMdStart, true );
                    return;
                }
            }
            _setCurrentState( Prom::UnStStoped );
            _setCurrentMode( Prom::UnMdStop );
            emit ManualStarted();
        }
    }
    else {
        _setCurrentState( Prom::UnStNotConnected );
        _setCurrentMode( Prom::UnMdStop );
        emit NoDef();
    }
}

//-----------------------------------------------------------------------------------
void MCC::_customConnectToGUI( QObject *guiItem, QObject * )
{
    connect( this,      SIGNAL( ManualStarted() ),           guiItem, SLOT( manualWork() ) ,             Qt::QueuedConnection );
    connect( this,      SIGNAL( Started() ),                 guiItem, SLOT( started() ),                 Qt::QueuedConnection );
}

//-----------------------------------------------------------------------------------


















