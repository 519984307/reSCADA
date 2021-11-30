
#include <qsettings.h>

#include "static2posunit.h"
#include "electroengine.h"

#include "InDiscretETag.h"
#include "OutDiscretETag.h"
#include <QDebug>

Static2posUnit::Static2posUnit( Prom::UnitType Type,
                                int ID,
                                QString Name,
                                QString TagPefix,
                                Prom::UnitModes SaveMode,
                                std::vector<QString> EngSensorNames,
                                bool NOnotNCSensors)
    : RouteUnit( Type,
                 ID,
                 Name,
                 TagPefix,
                 false,
                 SaveMode )
{

    _engine = new ElectroEngine( this, EngSensorNames );
    connect( _engine, &ElectroEngine::Alarm,       this, &Static2posUnit::detectAlarm, Qt::QueuedConnection);
    connect( _engine, &ElectroEngine::StateUpdate, this, &Static2posUnit::updateState , Qt::QueuedConnection);

    if( unitType == Prom::TypeGate || unitType == Prom::TypeUnderGate ){
        _POS1 = new InDiscretETag( this,
                                   "Концевик открытия",
                                   ".openS", true, false,
                                   NOnotNCSensors );
        _POS2 = new InDiscretETag( this,
                                   "Концевик закрытия",
                                   ".closeS",1,0,
                                   NOnotNCSensors );
    }
    else if ( unitType == Prom::TypeValve ) {
        _POS1 = new InDiscretETag( this,
                                   "Концевик 1",
                                   ".pos1S", true, false,
                                   NOnotNCSensors );

        _POS2 = new InDiscretETag( this,
                                   "Концевик 2",
                                   ".pos2S", true, false,
                                   NOnotNCSensors );
    }

    if( _POS1 != nullptr && _POS2 != nullptr ){

        _POS1->onlyChange();

        connect( _engine->startForward, &OutDiscretETag::s_on,  _POS1, &InDiscretETag::needBeDetectedAlarm /*, Qt::QueuedConnection*/);
        connect( _engine->startForward, &OutDiscretETag::s_off, _POS1, &InDiscretETag::onlyChange          /*, Qt::QueuedConnection*/);
        connect( _POS1, &InDiscretETag::s_valueChd, this, &Static2posUnit::updateState /*, Qt::QueuedConnection*/);

        connect( _POS1, &InDiscretETag::Detected,   _engine->startForward, &OutDiscretETag::off   /*, Qt::QueuedConnection*/);

        _POS2->onlyChange();

        connect( _engine->startBackward, &OutDiscretETag::s_on,  _POS2, &InDiscretETag::needBeDetectedAlarm /*, Qt::QueuedConnection*/);
        connect( _engine->startBackward, &OutDiscretETag::s_off, _POS2, &InDiscretETag::onlyChange         /*, Qt::QueuedConnection*/ );
        connect( _POS2, &InDiscretETag::s_valueChd, this, &Static2posUnit::updateState /*, Qt::QueuedConnection*/);

        connect( _POS2, &InDiscretETag::Detected,   _engine->startBackward, &OutDiscretETag::off   /*, Qt::QueuedConnection*/);
    }
}
//-----------------------------------------------------------------------------------
Prom::SetModeResp Static2posUnit::_customSetMode( Prom::UnitModes *mode, bool )
{
    if( _alarm )
        return RejAlarm;
    if( ! _sensorsConnected || ! _engine  )
        return RejNoCond;
    using namespace Prom;
    switch( *mode ) {
    case UnMdStop:{
        _engine->Stop();
        if( _engine->GetNowState() == Prom::EngStoped )
            return DoneAlready;
        else {
            return DoneWhait;
            _setSetedMode( *mode );
        }
        break;
    }
    case UnMd1pos:{
        switch ( currentState() ) {

        case UnSt2pos:
        case UnStMidlle:
            _engine->Stop();
            if( _engine->StartForward() ){
                _setSetedMode( *mode );
                /*TEST*/if( _POS1->isImit() && _POS2->isImit() && _engine->startForward->isImit() )
                    /*TEST*/QTimer::singleShot( 1000, [ this ](  ) {
                    this->_POS1->writeImitVal( false );
                    this->_POS2->writeImitVal( true );
                } );
                return  DoneWhait;
            }
            else return RejTransPr;
        case UnSt1pos:
            return  DoneAlready;
        case UnSt1posCommand:
        case UnSt1posMove:
        case UnSt1posStopCommand:
            _setSetedMode( *mode );
            return  DoneWhait;
        default:{
            _engine->Stop();
            if( _engine->StartForward() ){
                _setSetedMode( *mode );
                return DoneWhait;
            }
            else return RejTransPr;
        }
        }
    }
        break;
    case UnMd2pos:{

        switch ( currentState() ) {
        case UnSt1pos:
        case UnStMidlle:
            _engine->Stop();
            if( _engine->StartBackward() ){
                _setSetedMode( *mode );
                /*TEST*/if( _POS1->isImit() && _POS2->isImit() && _engine->startForward->isImit() )
                    /*TEST*/QTimer::singleShot( 1000, [ this ](  ) {
                    this->_POS1->writeImitVal( true );
                    this->_POS2->writeImitVal( false );
                } );
                return  DoneWhait;
            }
            else return RejTransPr;
        case UnSt2pos:
            return  DoneAlready;
        case UnSt2posCommand:
        case UnSt2posMove:
        case UnSt2posStopCommand:
            _setSetedMode( *mode );
            return  DoneWhait;
        default:{
            _engine->Stop();
            _setSetedMode( UnMdStop );
            if( _engine->StartBackward() ){
                _setSetedMode( *mode );
                return DoneWhait;
            }
            else return RejTransPr;
        }
        }
    }
    default: return RejAnnown;
    };
    //return RejAnnown;
}

//-----------------------------------------------------------------------------------
bool Static2posUnit::_midleMode()
{
    using namespace Prom;
    if( ( currentMode() ==  UnMdMidle || currentMode() == UnMdNoDef ) &&
            ( _engine->GetNowState() == EngForvard
              || _engine->GetNowState() == EngBackward ) )
        return true;
    else return false;
}

//-----------------------------------------------------------------------------------
void Static2posUnit::_updateStateAndMode()
{
    static Prom::UnitModes modeTMP;

    if( ( ! _engine ) || ( ! _POS1 ) || ( ! _POS2 ) ){
        _setCurrentState( Prom::UnStNoDef );
        return;
    }

    switch( _engine->GetNowState() ) {
    case Prom::EngNoDef:{
        _setCurrentState( Prom::UnStNoDef );
        _setCurrentMode( Prom::UnMdNoDef );
        break;
    }
    case Prom::EngToStopForward: {
        _setCurrentState( Prom::UnSt1posStopCommand );
        emit Pos1StopCommand();
        break;
    }
    case Prom::EngToStopBackward: {
        _setCurrentState( Prom::UnSt2posStopCommand );
        emit Pos2StopComand();
        break;
    }
    case Prom::EngStoped: {
        if( _POS1->isDetected() ) {
            if( _POS2->isDetected() ) {
                _setCurrentState( Prom::UnStNoDef );
                modeTMP = Prom::UnMdNoDef ;
                emit NoDefPos();
            }
            else {
                _setCurrentState( Prom::UnSt1pos );
                modeTMP = Prom::UnMd1pos;
                emit Pos1();
            }
        }
        else {
            if( _POS2->isDetected() ) {
                _setCurrentState( Prom::UnSt2pos );
                modeTMP = Prom::UnMd2pos;
                emit Pos2();
            }
            else {
                _setCurrentState( Prom::UnStMidlle );
                modeTMP = Prom::UnMdMidle;
                emit MiddlePos();
            }
        }
        if( setedMode() == Prom::UnMdStop ){
            _setCurrentMode( Prom::UnMdStop );
        }
        else _setCurrentMode( modeTMP );

        emit Stoped();
        break;
    }
    case Prom::EngToForvard: {
        _setCurrentState( Prom::UnSt1posCommand );
        emit Pos1Command();
        break;
    }
    case Prom::EngForvard: {
        _setCurrentState( Prom::UnSt1posMove );
        emit Pos1Move();
        if( _engine->startBackward->isImit()
                && _engine->startForward->isImit()
                && _POS2->isImit() && _POS1->isImit() ){
            //            QTimer::singleShot( 2000, [ this ](  ){ this->_POS1->WriteImitVal( false ); } );///
            //            QTimer::singleShot( 1000, [ this ](  ){ this->_POS2->WriteImitVal( true ); } );///
        }
        break;
    }
    case Prom::EngToBackward: {
        _setCurrentState( Prom::UnSt2posCommand );
        emit Pos2Command();
        break;
    }
    case Prom::EngBackward: {
        _setCurrentState( Prom::UnSt2posMove );
        //_mayResetAlarm = true;
        emit Pos2Move();
        if( _engine->startForward->isImit()
                && _engine->startBackward->isImit()
                && _POS1->isImit() && _POS2->isImit() ){
            //            QTimer::singleShot( 2000, [ this ](  ){ this->_POS2->WriteImitVal( false ); } );///
            //            QTimer::singleShot( 1000, [ this ](  ){ this->_POS1->WriteImitVal( true ); } );///
        }
        break;
    }
    case Prom::EngManualForward: {
        _setCurrentState( Prom::UnSt1posManual );
        emit Pos1Manual();
        break;
    }
    case Prom::EngManualBackward: {
        _setCurrentState( Prom::UnSt2posManual );
        emit Pos2Manual();
        break;
    }
    case Prom::EngManualMove: {
        _setCurrentState( Prom::UnStManualMove );
        emit NoDefManual();
        break;
    }
    case Prom::EngAlarm: {
        //_mayResetAlarm = false;
        detectAlarm( "авария электродвигателя - " + _engine->AlarmDescription() );
        break;
    }
    default:{
        //_mayResetAlarm = false;
        detectAlarm( "недопустимое состояние двигателя" );
    }
    }
}

//-----------------------------------------------------------------------------------
Prom::UnitModes Static2posUnit::_ModeOfCommand( Prom::RouteCommand *Command )
{
    using namespace Prom;
    switch ( *Command ) {
    case RtCmNo               : return currentMode();
    case RtCmStop             : return currentMode();
    case RtCmToRoute          :
    case RtCmToRoutForClean   :
    case RtCmCleanStart       : return _routeMode;
    case RtCmCleanStop        :
    case RtCmToSave           :
    case RtCmStopOnRouteAlarm :
        if( saveMode == Prom::UnMdNoDef ) return currentMode();
        else return saveMode;
    default: return UnMdNoDef;
    }
}
//-----------------------------------------------------------------------------------
void Static2posUnit::_alarmDo()
{
    if( _engine ) _engine->Stop();
}

//-----------------------------------------------------------------------------------
void Static2posUnit::_customConnectToGUI( QObject *guiItem, QObject * )
{
    connect( this,      SIGNAL( Pos1Move() ),                guiItem, SLOT( started1() ) ,               Qt::QueuedConnection );
    connect( this,      SIGNAL( Pos1Command() ),             guiItem, SLOT( startComand1() ) ,           Qt::QueuedConnection );
    connect( this,      SIGNAL( Pos2Move() ),                guiItem, SLOT( started2() ) ,               Qt::QueuedConnection );
    connect( this,      SIGNAL( Pos2Command() ),             guiItem, SLOT( startComand2() ) ,           Qt::QueuedConnection );

    connect( this,      SIGNAL( Pos1() ),                    guiItem, SLOT( pos1() ) ,                   Qt::QueuedConnection );
    connect( this,      SIGNAL( Pos2() ),                    guiItem, SLOT( pos2() ) ,                   Qt::QueuedConnection );
    connect( this,      SIGNAL( MiddlePos() ),               guiItem, SLOT( posMid() ) ,                 Qt::QueuedConnection );
    connect( this,      SIGNAL( NoDefPos() ),                guiItem, SLOT( posUndef() ) ,               Qt::QueuedConnection );

    connect( guiItem, SIGNAL( start1() ),                  this,      SLOT( ToPos1() ) ,                 Qt::QueuedConnection );
    connect( guiItem, SIGNAL( start2() ),                  this,      SLOT( ToPos2() ) ,                 Qt::QueuedConnection );

    connect( guiItem, SIGNAL( addToCurrentRoteIn1pos() ),  this,      SLOT( AddToCurrentRoteIn1pos() ) , Qt::QueuedConnection );
    connect( guiItem, SIGNAL( addToCurrentRoteIn2pos() ),  this,      SLOT( AddToCurrentRoteIn2pos() ) , Qt::QueuedConnection );
    connect( this,    SIGNAL( Stoped() ),                  guiItem,   SLOT( stoped() ) ,                 Qt::QueuedConnection );
    connect( guiItem, SIGNAL( stop() ),                    this,      SLOT( Stop() ) ,                   Qt::QueuedConnection );
}

//-----------------------------------------------------------------------------------























