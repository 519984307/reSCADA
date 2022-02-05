#include "Druer1Bunker.h"
#include "InDiscretETag.h"
#include <QSettings>
#include "static2posunit.h"

Druer1Bunker::Druer1Bunker( int ID,
                            QString Name,
                            QString TagPefix )
    :RouteUnit( Prom::TypeDryer3_Bunker,
                ID,
                Name,
                TagPefix,
                false,
                Prom::UnMdStop )
{
    _HL = new InDiscretETag( this, "Датчик верхнего уровня", ".HL", true, false, true, false );
    _HL->onlyChange();
    connect(  _HL, &InDiscretETag::s_valueChd,    this, &Druer1Bunker::updateState,  Qt::QueuedConnection );
    connect(  _HL, &InDiscretETag::s_detected,   this, &Druer1Bunker::ActivateHL,   Qt::QueuedConnection );
    connect(  _HL, &InDiscretETag::s_undetected, this, &Druer1Bunker::DeactivateHL, Qt::QueuedConnection );

    _ML = new InDiscretETag( this, "Датчик среднего уровня", ".ML", true, false, true, false );
    _ML->onlyChange();
    connect( _ML, &InDiscretETag::s_valueChd,    this, &Druer1Bunker::updateState,  Qt::QueuedConnection );
    connect( _ML, &InDiscretETag::s_detected,   this, &Druer1Bunker::ActivateML,   Qt::QueuedConnection );
    connect( _ML, &InDiscretETag::s_undetected, this, &Druer1Bunker::DeactivateML, Qt::QueuedConnection );

    _LL = new InDiscretETag( this, "Датчик нижнего уровня", ".LL", true, false, true, false );
    _LL->onlyChange();
    connect( _LL, &InDiscretETag::s_valueChd,    this, &Druer1Bunker::updateState,  Qt::QueuedConnection );
    connect( _LL, &InDiscretETag::s_detected,   this, &Druer1Bunker::ActivateLL,   Qt::QueuedConnection );
    connect( _LL, &InDiscretETag::s_undetected, this, &Druer1Bunker::DeactivateLL, Qt::QueuedConnection );
}
//------------------------------------------------------------------------------
void Druer1Bunker::_updateStateAndMode()
{
    if( _HL->isDetected() ) {
        _setCurrentState( Prom::UnStHiLvl );
        _setCurrentMode( Prom::UnMdNeedUnload );
    }
    else if( _ML->isDetected() ) {
        _setCurrentState( Prom::UnStMidLvl );
    }
    else if( _LL->isDetected() ) {
        _setCurrentState( Prom::UnStLowLvl );
        _setCurrentMode( Prom::UnMdNeedLoad );
    }
    else {
        _setCurrentState( Prom::UnStEmpty );
        _setCurrentMode( Prom::UnMdNeedLoad );
    }
}

//------------------------------------------------------------------------------
Prom::SetModeResp Druer1Bunker::_customSetMode( Prom::UnitModes *mode, bool )
{
    if( currentMode() == *mode ) return Prom::DoneAlready;
    else{
        switch ( *mode ) {
        case Prom::UnMdNeedLoad  :
        case Prom::UnMdNeedUnload :
            _setSetedMode( *mode );
            return Prom::DoneWhait;
        default : return RejAnnown;
        }
    }
}

//------------------------------------------------------------------------------
Prom::UnitModes Druer1Bunker::_ModeOfCommand( Prom::RouteCommand *Command )
{
    using namespace Prom;
    switch ( *Command ) {
    case RtCmNo               : return UnMdNoDef;
    case RtCmStop             :
    case RtCmToRoute          :
    case RtCmCleanStart       :
    case RtCmToRoutForClean   : return _routeMode;
    case RtCmCleanStop        :
    case RtCmToSave           :
    case RtCmStopOnRouteAlarm : return currentMode();
    default: return currentMode();
    }
}

//-----------------------------------------------------------------------------------
void Druer1Bunker::_customConnectToGUI( QObject *guiItem,  QObject * )
{
    connect( this,      SIGNAL( ActivateHL()   ), guiItem, SLOT( activateHL()   ),   Qt::QueuedConnection );
    connect( this,      SIGNAL( DeactivateHL() ), guiItem, SLOT( deactivateHL() ),   Qt::QueuedConnection );
    connect( this,      SIGNAL( ActivateML()   ), guiItem, SLOT( activateML()   ),   Qt::QueuedConnection );
    connect( this,      SIGNAL( DeactivateML() ), guiItem, SLOT( deactivateML() ),   Qt::QueuedConnection );
    connect( this,      SIGNAL( ActivateLL()   ), guiItem, SLOT( activateLL()   ),   Qt::QueuedConnection );
    connect( this,      SIGNAL( DeactivateLL() ), guiItem, SLOT( deactivateLL() ),   Qt::QueuedConnection );

    connect( guiItem, SIGNAL( addToCurrentRoteNeedUnload() ),this, SLOT( AddToCurrentRoteNeedUnload() ),   Qt::QueuedConnection );
    connect( guiItem, SIGNAL( addToCurrentRoteNeedLoad() ),  this, SLOT( AddToCurrentRoteNeedLoad() ),     Qt::QueuedConnection );
}

//------------------------------------------------------------------------------


















