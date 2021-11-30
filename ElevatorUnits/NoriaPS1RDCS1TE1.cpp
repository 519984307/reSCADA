#include <qdebug.h>
#include "NoriaPS1RDCS1TE1.h"
#include "electroengine.h"
#include "SCADAenums.h"
#include "InDiscretETag.h"
#include "OutDiscretETag.h"
#include "InETag.h"



NoriaVSM_PS1RDCS1TE1::NoriaVSM_PS1RDCS1TE1( int ID,
                                            QString Name,
                                            QString TagPefix,
                                            std::vector<QString> EngSensorNames,
                                            bool PS_NCnotNO,
                                            bool RDCS_NCnotNO,
                                            bool TE_NCnotNO
                                            )

    : Noria( ID,
             Name,
             TagPefix,
             EngSensorNames )
{
    _PS = new InDiscretETag( this, "Датчик подпора",".PS", true, false, PS_NCnotNO, false );
    _PS->needBeDetectedAlarmNoTime();

    _RDCS = new InDiscretETag( this, "Реле контроля скорости", ".RDCS", true, false, RDCS_NCnotNO );
    if( _engine->startForward->isOn() )_RDCS->needBeDetectedAlarm();
    else _RDCS->needBeUndetectedNoAlarmNoTime();
    connect( _engine->KMforward, &InDiscretETag::Detected,  _RDCS, &InDiscretETag::needBeDetectedAlarm , Qt::QueuedConnection);
    connect( _engine->KMforward, &InDiscretETag::Undetected, _RDCS, &InDiscretETag::needBeUndetectedNoAlarm , Qt::QueuedConnection);

    _TE = new InDiscretETag( this, "Датчик схода ленты", ".TE", true, false,TE_NCnotNO, false );
    _TE->needBeDetectedAlarmNoTime();

    _CRT = new InETag( this, Prom::TpIn, "Ток нагрузки", ".current", true, 30, 1, true, false, true );
    _CRT->needBeUndetectedAlarm();
    connect( _engine->KMforward,    &InDiscretETag::Detected,   _CRT, &InDiscretETag::needBeUndetectedAlarm,       Qt::QueuedConnection);
    connect( _engine->KMforward,    &InDiscretETag::Undetected, _CRT, &InDiscretETag::needBeUndetectedAlarmNoTime, Qt::QueuedConnection);
}

//------------------------------------------------------------------------------

