#include <QSettings>
#include "Silos3.h"
#include "InDiscretETag.h"


Silos3::Silos3( int ID,
                QString Name,
                QString TagPefix,
                bool HLNOnotNC,
                Prom::UnitModes SaveMode )
    :RouteUnit( Prom::TypeSilos,
                ID,
                Name,
                TagPefix,
                false,
                SaveMode )
{
    _HL = new InDiscretETag( this, "Датчик верхнего уровня", ".HL", true, false, HLNOnotNC, false );
    _HL->onlyChange();
    connect( _HL, &InDiscretETag::s_valueChd,    this, &Silos3::updateState,  Qt::QueuedConnection );
    connect( _HL, &InDiscretETag::s_detected,   this, &Silos3::ActivateHL,   Qt::QueuedConnection );
    connect( _HL, &InDiscretETag::s_undetected, this, &Silos3::DeactivateHL, Qt::QueuedConnection );

    _ML = new InDiscretETag( this, "Датчик среднего уровня", ".ML", true, false, HLNOnotNC, false );
    _ML->onlyChange();
    connect( _ML, &InDiscretETag::s_valueChd,    this, &Silos3::updateState,  Qt::QueuedConnection );
    connect( _ML, &InDiscretETag::s_detected,   this, &Silos3::ActivateML,   Qt::QueuedConnection );
    connect( _ML, &InDiscretETag::s_undetected, this, &Silos3::DeactivateML, Qt::QueuedConnection );

    _LL = new InDiscretETag( this, "Датчик нижнего уровня", ".LL", true, false, HLNOnotNC, false );
    _LL->onlyChange();
    connect( _LL, &InDiscretETag::s_valueChd,    this, &Silos3::updateState,  Qt::QueuedConnection );
    connect( _LL, &InDiscretETag::s_detected,   this, &Silos3::ActivateLL,   Qt::QueuedConnection );
    connect( _LL, &InDiscretETag::s_undetected, this, &Silos3::DeactivateLL, Qt::QueuedConnection );

}
//------------------------------------------------------------------------------
void Silos3::_updateStateAndMode()
{
    if( _HL->isDetected() ) {
        _setCurrentMode( Prom::UnMdFull );
    }
    else if( _ML->isDetected() ) {
        _setCurrentMode( Prom::UnMdMidle );
    }
    else if( _LL->isDetected() ) {
        _setCurrentMode( Prom::UnMdLow );
    }
    else {
        _setCurrentMode( Prom::UnMdEmpty );
    }
}

//------------------------------------------------------------------------------
Prom::SetModeResp Silos3::_customSetMode(Prom::UnitModes *mode , bool UserOrSys)
{
    switch ( *mode ) {
    case Prom::UnMdFull :
    case Prom::UnMdMidle:
    case Prom::UnMdLow:
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
void Silos3::_customConnectToGUI( QObject *guiItem,  QObject *propWin)
{
    connect( this,       SIGNAL( ActivateHL()              ), guiItem,  SLOT( activateHL()          ),   Qt::QueuedConnection );
    connect( this,       SIGNAL( DeactivateHL()            ), guiItem,  SLOT( deactivateHL()        ),   Qt::QueuedConnection );
    connect( this,       SIGNAL( ActivateML()              ), guiItem,  SLOT( activateML()          ),   Qt::QueuedConnection );
    connect( this,       SIGNAL( DeactivateML()            ), guiItem,  SLOT( deactivateML()        ),   Qt::QueuedConnection );
    connect( this,       SIGNAL( ActivateLL()              ), guiItem,  SLOT( activateLL()          ),   Qt::QueuedConnection );
    connect( this,       SIGNAL( DeactivateLL()            ), guiItem,  SLOT( deactivateLL()        ),   Qt::QueuedConnection );
    connect( this,       SIGNAL( s_shangeExName(QVariant) ), propWin, SLOT( setExName(QVariant) ),   Qt::QueuedConnection );
    connect( propWin, SIGNAL( changedExName(QString) ), this,       SLOT( setExName(QString) ),   Qt::QueuedConnection );
    connect( guiItem,  SIGNAL( addToCurrentRoteFull()    ), this, SLOT( AddToCurrentRoteFull()      ),   Qt::QueuedConnection );
    connect( guiItem,  SIGNAL( addToCurrentRoteEmpty()   ), this, SLOT( AddToCurrentRoteEmpty()     ),   Qt::QueuedConnection );
}

//------------------------------------------------------------------------------





















