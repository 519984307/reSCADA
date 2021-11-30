#include <qdebug.h>
#include "OWSinhroTransporter.h"
#include "SCADAenums.h"
#include "InDiscretETag.h"

OWSinhroTransporter::OWSinhroTransporter( int ID,
                                          QString Name,
                                          QString TagPefix)

    : RouteUnit(  TypeTransporter,
                  ID,
                  Name,
                  TagPefix,
                  true,
                  Prom::UnMdStop )
{
    _SS = new InDiscretETag( this,
                              "Датчик движения",
                              ".SS", true, false,
                              true, true, true );

    _SS->pulsSensor( true );
    _SS->onlyChange();
    connect( _SS, &InETag::Detected, this, &Unit::updateState, Qt::QueuedConnection );
    connect( _SS, &InETag::Undetected, this, &Unit::updateState, Qt::QueuedConnection );
}

//------------------------------------------------------------------------------
void OWSinhroTransporter::_customConnectToGUI( QObject *guiItem, QObject *propWin )
{
    connect( guiItem, SIGNAL( addToCurrentRoteStarted() ), this,    SLOT( AddToCurrentRoteStarted() ), Qt::QueuedConnection );
    connect( guiItem, SIGNAL( addToCurrentRoteStoped() ),  this,    SLOT( AddToCurrentRoteStoped() ),  Qt::QueuedConnection );
    connect( this,    SIGNAL( Stoped() ),                  guiItem, SLOT( stoped() ) ,                 Qt::QueuedConnection );
    connect( this,    SIGNAL( ManualStarted() ),           guiItem, SLOT( manualWork() ) ,             Qt::QueuedConnection );
    connect( this,    SIGNAL( Started() ),                 guiItem, SLOT( started() ),                 Qt::QueuedConnection );
}

//------------------------------------------------------------------------------
Prom::SetModeResp OWSinhroTransporter::_customSetMode( UnitModes *mode, bool UserOrSys )
{
    switch ( *mode ) {
    case Prom::UnMdStart :
    case Prom::UnMdStop :
        if( currentMode() == *mode ) return Prom::DoneAlready;
        else{
            _setSetedMode( *mode );
            return Prom::DoneWhait;
        }
    default : return RejAnnown;
    }
    return RejAnnown;
}
//------------------------------------------------------------------------------
void OWSinhroTransporter::_updateStateAndMode()
{
    if( _SS->isDetected() ) {
        switch ( setedMode() ) {
        case UnMdCleanStart:
            _setCurrentState( Prom::UnStClean );
            _setCurrentMode( Prom::UnMdStart );
            emit Cleaning();
            break;
        default:
            _setCurrentState( Prom::UnStStarted );
            _setCurrentMode( Prom::UnMdStart );
            emit Started();
            break;
        }
    }
    else{
        switch ( setedMode() ) {
        case UnMdCleanStop:
            _setCurrentState( Prom::UnStCleaned );
            _setCurrentMode( Prom::UnMdCleanStop );
            break;
        default:
            _setCurrentState( Prom::UnStStoped );
            _setCurrentMode( Prom::UnMdStop );
            break;
        }
        emit Stoped();
    }
}
//------------------------------------------------------------------------------
