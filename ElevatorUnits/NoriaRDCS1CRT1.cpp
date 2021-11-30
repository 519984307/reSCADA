#include "NoriaRDCS1CRT1.h"
#include "electroengine.h"
#include "InDiscretETag.h"
#include "InETag.h"



NoriaRDCS1CRT1::NoriaRDCS1CRT1( int ID,
                      QString Name,
                      QString TagPefix,
                      std::vector<QString> EngSensorNames,
                      bool RDCS_NCnotNO )

    : Noria(           ID,
                       Name,
                       TagPefix,
                       EngSensorNames )
{
    _RDCS = new InDiscretETag( this, "Реле контроля скорости", ".RDCS", true, false, RDCS_NCnotNO );

    if( _engine->KMforward->isDetected() )_RDCS->needBeDetectedAlarm();
    else _RDCS->needBeUndetectedAlarm();
    connect( _engine->KMforward, &InDiscretETag::Detected,   _RDCS, &InDiscretETag::needBeDetectedAlarm , Qt::QueuedConnection);
    connect( _engine->KMforward, &InDiscretETag::Undetected, _RDCS, &InDiscretETag::needBeUndetectedAlarm , Qt::QueuedConnection);

    _CRT = new InETag( this, Prom::TpIn, "Ток нагрузки", ".current", true, 30, 1, true, false, true );

    _CRT->needBeUndetectedAlarm();
    connect( _engine->KMforward,    &InDiscretETag::Detected,   _CRT, &InDiscretETag::needBeUndetectedAlarm,       Qt::QueuedConnection);
}

//------------------------------------------------------------------------------
