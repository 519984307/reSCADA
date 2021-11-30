#include "Dryer2Bunker.h"
#include "InDiscretETag.h"
#include <QSettings>

Dryer2Bunker::Dryer2Bunker( int ID,
                              QString Name,
                              QString TagPefix )
    :RouteUnit( Prom::TypeDryer3_Bunker,
                ID,
                Name,
                TagPefix,
                false,
                Prom::UnMdStop )
{
    _useMayResAlarm = true;
    _HL1 = new InDiscretETag( this, "Датчик верхнего уровня 1", ".HL1", true, false, true, false );
    _HL1->onlyChange();
    connect( _HL1, &InDiscretETag::s_valueChd,    this, &Dryer2Bunker::updateState,  Qt::QueuedConnection );
    connect( _HL1, &InDiscretETag::Detected,   this, &Dryer2Bunker::ActivateHL1,   Qt::QueuedConnection );
    connect( _HL1, &InDiscretETag::Undetected, this, &Dryer2Bunker::DeactivateHL1, Qt::QueuedConnection );

    _ML1 = new InDiscretETag( this, "Датчик среднего уровня 1", ".ML1", true, false, true, false );
    _ML1->onlyChange();
    connect( _ML1, &InDiscretETag::s_valueChd,    this, &Dryer2Bunker::updateState,  Qt::QueuedConnection );
    connect( _ML1, &InDiscretETag::Detected,   this, &Dryer2Bunker::ActivateML1,   Qt::QueuedConnection );
    connect( _ML1, &InDiscretETag::Undetected, this, &Dryer2Bunker::DeactivateML1, Qt::QueuedConnection );

    _LL1 = new InDiscretETag( this, "Датчик нижнего уровня 1", ".LL1", true, false, true, false );
    _LL1->onlyChange();
    connect( _LL1, &InDiscretETag::s_valueChd,    this, &Dryer2Bunker::updateState,  Qt::QueuedConnection );
    connect( _LL1, &InDiscretETag::Detected,   this, &Dryer2Bunker::ActivateLL1,   Qt::QueuedConnection );
    connect( _LL1, &InDiscretETag::Undetected, this, &Dryer2Bunker::DeactivateLL1, Qt::QueuedConnection );

    //----
    _HL2 = new InDiscretETag( this, "Датчик верхнего уровня 2", ".HL2", true, false, true, false );
    _HL2->onlyChange();
    connect( _HL2, &InDiscretETag::s_valueChd,    this, &Dryer2Bunker::updateState,  Qt::QueuedConnection );
    connect( _HL2, &InDiscretETag::Detected,   this, &Dryer2Bunker::ActivateHL2,   Qt::QueuedConnection );
    connect( _HL2, &InDiscretETag::Undetected, this, &Dryer2Bunker::DeactivateHL2, Qt::QueuedConnection );

    _ML2 = new InDiscretETag( this, "Датчик среднего уровня 2", ".ML2", true, false, true, false );
    _ML2->onlyChange();
    connect( _ML2, &InDiscretETag::s_valueChd,    this, &Dryer2Bunker::updateState,  Qt::QueuedConnection );
    connect( _ML2, &InDiscretETag::Detected,   this, &Dryer2Bunker::ActivateML2,   Qt::QueuedConnection );
    connect( _ML2, &InDiscretETag::Undetected, this, &Dryer2Bunker::DeactivateML2, Qt::QueuedConnection );

    _LL2 = new InDiscretETag( this, "Датчик нижнего уровня 2", ".LL2", true, false, true, false );
    _LL2->onlyChange();
    connect( _LL2, &InDiscretETag::s_valueChd,    this, &Dryer2Bunker::updateState,  Qt::QueuedConnection );
    connect( _LL2, &InDiscretETag::Detected,   this, &Dryer2Bunker::ActivateLL2,   Qt::QueuedConnection );
    connect( _LL2, &InDiscretETag::Undetected, this, &Dryer2Bunker::DeactivateLL2, Qt::QueuedConnection );

}
//------------------------------------------------------------------------------
void Dryer2Bunker::_updateStateAndMode()
{
    bool alarmLvl = false;
    //    if( _LL1->isDetected() ){
    //        if( ! _ML1->isDetected() )
    //            if( _HL1->isDetected() )
    //                alarmLvl = true;
    //    }
    //    else {
    //        if( _ML1->isDetected() || _HL1->isDetected() )
    //            alarmLvl = true;
    //    }

    //    if( _LL2->isDetected() ){
    //        if( ! _ML2->isDetected() )
    //            if( _HL2->isDetected() )
    //                alarmLvl = true;
    //    }
    //    else {
    //        if( _ML2->isDetected() || _HL2->isDetected() )
    //            alarmLvl = true;
    //    }

    //    if( _LL3->isDetected() ){
    //        if( ! _ML3->isDetected() )
    //            if( _HL3->isDetected() )
    //                alarmLvl = true;
    //    }
    //    else {
    //        if( _ML3->isDetected() || _HL3->isDetected() )
    //            alarmLvl = true;
    //    }

    static unsigned short int H {0};

    if( _LL1->isDetected() && _LL2->isDetected() ) {

        if( _ML1->isDetected() && _ML2->isDetected() ) {

            H = _HL1->isDetected() + _HL2->isDetected();
            if( H > 1 ) {
                _setCurrentState( Prom::UnStHiLvl );
                _setCurrentMode( Prom::UnMdNeedUnload );
            }
            else {
                _setCurrentState( Prom::UnStMidLvl );
                //_setCurrentMode( Prom::UnMdNeedLoad );
            }
        }
        else{
            _setCurrentState( Prom::UnStLowLvl);
            _setCurrentMode( Prom::UnMdNeedLoad );
        }
    }
    else{
        //if( ! ( _HL1->isDetected() || _HL2->isDetected() || _HL3->isDetected() ) ) {
            _setCurrentState( Prom::UnStEmpty );
            _setCurrentMode( Prom::UnMdNeedLoad );
       // }
    }
    if( alarmLvl && ! _lvlAlarm ){
        _lvlAlarm = true;
        _setCurrentMode( Prom::UnMdNeedLoad );
        detectAlarm( "отказ одного из датчиков уровня, либо затор продукта" );
    }
    else {
        _lvlAlarm = false;
    }
}

//------------------------------------------------------------------------------
Prom::SetModeResp Dryer2Bunker::_customSetMode( Prom::UnitModes *mode, bool )
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
    return RejAnnown;
}
//------------------------------------------------------------------------------
Prom::UnitModes Dryer2Bunker::_ModeOfCommand( Prom::RouteCommand *Command )
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

//------------------------------------------------------------------------------
bool Dryer2Bunker::_resetAlarm( bool upClassAlarm )
{
    return RouteUnit::_resetAlarm( upClassAlarm || _lvlAlarm  );
}
//------------------------------------------------------------------------------
void Dryer2Bunker::_customConnectToGUI( QObject *guiItem,  QObject * )
{
    connect( this, SIGNAL( ActivateHL1()   ), guiItem, SLOT( activateHL1()   ),   Qt::QueuedConnection );
    connect( this, SIGNAL( DeactivateHL1() ), guiItem, SLOT( deactivateHL1() ),   Qt::QueuedConnection );
    connect( this, SIGNAL( ActivateML1()   ), guiItem, SLOT( activateML1()   ),   Qt::QueuedConnection );
    connect( this, SIGNAL( DeactivateML1() ), guiItem, SLOT( deactivateML1() ),   Qt::QueuedConnection );
    connect( this, SIGNAL( ActivateLL1()   ), guiItem, SLOT( activateLL1()   ),   Qt::QueuedConnection );
    connect( this, SIGNAL( DeactivateLL1() ), guiItem, SLOT( deactivateLL1() ),   Qt::QueuedConnection );

    connect( this, SIGNAL( ActivateHL2()   ), guiItem, SLOT( activateHL2()   ),   Qt::QueuedConnection );
    connect( this, SIGNAL( DeactivateHL2() ), guiItem, SLOT( deactivateHL2() ),   Qt::QueuedConnection );
    connect( this, SIGNAL( ActivateML2()   ), guiItem, SLOT( activateML2()   ),   Qt::QueuedConnection );
    connect( this, SIGNAL( DeactivateML2() ), guiItem, SLOT( deactivateML2() ),   Qt::QueuedConnection );
    connect( this, SIGNAL( ActivateLL2()   ), guiItem, SLOT( activateLL2()   ),   Qt::QueuedConnection );
    connect( this, SIGNAL( DeactivateLL2() ), guiItem, SLOT( deactivateLL2() ),   Qt::QueuedConnection );

    connect( guiItem, SIGNAL( addToCurrentRoteNeedUnload() ),this, SLOT( AddToCurrentRoteNeedUnload() ),   Qt::QueuedConnection );
    connect( guiItem, SIGNAL( addToCurrentRoteNeedLoad() ),  this, SLOT( AddToCurrentRoteNeedLoad() ),     Qt::QueuedConnection );
}

//------------------------------------------------------------------------------
