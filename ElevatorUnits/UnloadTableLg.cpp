#include "UnloadTableLg.h"
#include "InDiscretETag.h"
#include "OutETag.h"
#include "OutDiscretETag.h"
#include "electroengine.h"

UnloadTableLg::UnloadTableLg( int ID,
                              QString Name,
                              QString TagPefix,
                              bool Mover,
                              std::vector<QString> EngSensorNames  )

    : OneEngRouteFClUnit( TypeDryerUnloadTable,
                          ID,
                          Name,
                          TagPefix,
                          Mover,
                          EngSensorNames )
{
    //_pos = new InDiscretETag( this, "Положение стола", ".sensor", true, false, true, true );
    //_pos->onlyChange();
    _pos = new InDiscretETag( this,
                              "Положение стола",
                              ".sensor", false, true,
                              true, true, true );

    if( _engine->KMforward->isDetected() )
    {
        _pos->pulsSensor( true );
    }
    else{
        _pos->pulsSensor( false );
    }
    _pos->needBeDetectedAlarm();

    connect( _engine->KMforward, &InDiscretETag::Detected,   _pos, &InDiscretETag::needBeDetectedAlarm, Qt::QueuedConnection);
    connect( _engine->KMforward, &InDiscretETag::Undetected, _pos, &InDiscretETag::needBeDetectedAlarm, Qt::QueuedConnection);

    connect( _engine->KMforward, &InDiscretETag::Detected,   _pos, &InDiscretETag::pulsSensorON,  Qt::QueuedConnection);
    connect( _engine->KMforward, &InDiscretETag::Undetected, _pos, &InDiscretETag::pulsSensorOFF, Qt::QueuedConnection);

    _forseSop = new OutDiscretETag( this, Prom::PreSet,
                                    "Аварийный стоп", ".forseStop");
}

//-----------------------------------------------------------------------------------
void UnloadTableLg::GetSpeed( QVariant )
{

}
//-----------------------------------------------------------------------------------
void UnloadTableLg::_customConnectToGUI( QObject *guiItem,  QObject *propWin)
{
    OneEngRouteFClUnit::_customConnectToGUI( guiItem, propWin );
    //connect( guiItem, SIGNAL( speedChanged( QVariant ) ),   _SetFR,  SLOT( setValue( QVariant ) ),       Qt::QueuedConnection );
    //connect( _SetFR,  SIGNAL( s_valueChd( QVariant ) ), guiItem, SLOT( setSpeed( QVariant ) ),       Qt::QueuedConnection );
}

//-----------------------------------------------------------------------------------
SetModeResp UnloadTableLg::_customSetMode( UnitModes *Mode, bool UserOrSys )
{
    if( *Mode == Prom::UnMdFreeze ){
        if( _forseSop )_forseSop->on();
        Prom::UnitModes M = Prom::UnMdStop;
        return OneEngRouteUnit::_customSetMode( &M, UserOrSys );
    }
    else if( _forseSop )_forseSop->off();

    return OneEngRouteUnit::_customSetMode( Mode, UserOrSys );
}

//-----------------------------------------------------------------------------------
