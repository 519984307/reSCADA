#include "Silos1.h"
#include "InDiscretETag.h"
#include <QSettings>

Silos1::Silos1( int ID,
                QString Name,
                QString TagPefix,
                bool HLNOnotNC,
                Prom::UnitModes SaveMode
                )
    :RouteUnit( Prom::TypeSilos,
                ID,
                Name,
                TagPefix,
                false,
                SaveMode )
{
    _HL = new InDiscretETag( this, "Датчик верхнего уровня", ".HL", true, false, HLNOnotNC, false );
    _HL->onlyChange();
    connect( _HL, &InDiscretETag::s_valueChd,    this, &Silos1::updateState,  Qt::QueuedConnection );
    connect( _HL, &InDiscretETag::s_detected,   this, &Silos1::ActivateHL,   Qt::QueuedConnection );
    connect( _HL, &InDiscretETag::s_undetected, this, &Silos1::DeactivateHL, Qt::QueuedConnection );
}
//------------------------------------------------------------------------------
void Silos1::_updateStateAndMode()
{
    if( _HL->isDetected() ) {
        _setCurrentMode( Prom::UnMdFull );
        //emit Full();
    }
    else {
        _setCurrentMode( Prom::UnMdEmpty );
        //emit Empty();
    }
}

//------------------------------------------------------------------------------
Prom::SetModeResp Silos1::_customSetMode( Prom::UnitModes *mode, bool  )
{
    switch ( *mode ) {
    case Prom::UnMdFull :
    case Prom::UnMdEmpty:
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
void Silos1::_customConnectToGUI( QObject *guiItem,  QObject *propWin)
{
    connect( this,       SIGNAL( ActivateHL()              ), guiItem,  SLOT( activateHL()          ),   Qt::QueuedConnection );
    connect( this,       SIGNAL( DeactivateHL()            ), guiItem,  SLOT( deactivateHL()        ),   Qt::QueuedConnection );
    connect( this,       SIGNAL( s_shangeExName(QVariant) ), propWin, SLOT( setExName(QVariant) ),   Qt::QueuedConnection );
    connect( propWin, SIGNAL( changedExName(QString) ), this,       SLOT( setExName(QString) ),   Qt::QueuedConnection );
    connect( guiItem,  SIGNAL( addToCurrentRoteFull()    ), this, SLOT( AddToCurrentRoteFull()      ),   Qt::QueuedConnection );
    connect( guiItem,  SIGNAL( addToCurrentRoteEmpty()   ), this, SLOT( AddToCurrentRoteEmpty()     ),   Qt::QueuedConnection );
}

//-----------------------------------------------------------------------------------
Prom::UnitModes Silos1::_ModeOfCommand( Prom::RouteCommand *Command )
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

















