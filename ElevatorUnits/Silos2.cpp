#include "Silos2.h"
#include "InDiscretETag.h"
#include <QSettings>

Silos2::Silos2( int ID,
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
    connect( _HL, &InDiscretETag::s_valueChd,    this, &Silos2::updateState,  Qt::QueuedConnection );
    connect( _HL, &InDiscretETag::s_detected,   this, &Silos2::ActivateHL,   Qt::QueuedConnection );
    connect( _HL, &InDiscretETag::s_undetected, this, &Silos2::DeactivateHL, Qt::QueuedConnection );

    _LL = new InDiscretETag( this, "Датчик нижнего уровня", ".LL", true, false, HLNOnotNC, false );
    _LL->onlyChange();
    connect( _LL, &InDiscretETag::s_valueChd,    this, &Silos2::updateState,  Qt::QueuedConnection );
    connect( _LL, &InDiscretETag::s_detected,   this, &Silos2::ActivateLL,   Qt::QueuedConnection );
    connect( _LL, &InDiscretETag::s_undetected, this, &Silos2::DeactivateLL, Qt::QueuedConnection );

}

//------------------------------------------------------------------------------
void Silos2::_updateStateAndMode()
{
    if( _HL->isDetected() ) {
        _currentMode = Prom::UnMdFull;
    }
    else if( _LL->isDetected() ) {
        _currentMode = Prom::UnMdLow;
    }
    else {
        _currentMode = Prom::UnMdEmpty;
    }
}

//------------------------------------------------------------------------------
SetModeResp Silos2::_customSetMode(UnitModes *mode, bool )
{
    switch ( *mode ) {
    case Prom::UnMdFull :
    case Prom::UnMdMidle:
    case Prom::UnMdEmpty:
        if( _currentMode == *mode ) return Prom::DoneAlready;
        else{
            _setedMode = *mode;
            return Prom::DoneWhait;
        }
    default : return RejAnnown;
    }
    return RejAnnown;
}

//------------------------------------------------------------------------------
void Silos2::_customConnectToGUI(QObject *guiItem, QObject *propWin)
{
    connect( this,    SIGNAL( ActivateHL() ),            guiItem, SLOT( activateHL() ),            Qt::QueuedConnection );
    connect( this,    SIGNAL( DeactivateHL() ),          guiItem, SLOT( deactivateHL() ),          Qt::QueuedConnection );
    connect( this,    SIGNAL( ActivateLL() ),            guiItem, SLOT( activateLL() ),            Qt::QueuedConnection );
    connect( this,    SIGNAL( DeactivateLL() ),          guiItem, SLOT( deactivateLL() ),          Qt::QueuedConnection );
    connect( this,    SIGNAL( s_shangeExName(QVariant) ),  propWin, SLOT( setExName(QVariant) ),     Qt::QueuedConnection );
    connect( propWin, SIGNAL( changedExName(QString) ),  this,    SLOT( setExName(QString) ),      Qt::QueuedConnection );
    connect( guiItem, SIGNAL( addToCurrentRoteFull() ),  this,    SLOT( AddToCurrentRoteFull() ),  Qt::QueuedConnection );
    connect( guiItem, SIGNAL( addToCurrentRoteEmpty() ), this,    SLOT( AddToCurrentRoteEmpty() ), Qt::QueuedConnection );
}

//------------------------------------------------------------------------------






















