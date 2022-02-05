#include "TransporterFC_PStoKM.h"
#include "electroengine.h"
#include "InDiscretETag.h"
#include "OutDiscretETag.h"

TransporterFC_PStoKM::TransporterFC_PStoKM( int ID,
                                            QString Name,
                                            QString TagPefix,
                                            std::vector<QString> EngSensorNames,
                                            bool PS_NOnotNC )

    : OneEngRouteFCUnit(  Prom::TypeTransporter,
                          ID,
                          Name,
                          TagPefix,
                          true,
                          EngSensorNames )
{
    _PS = new InDiscretETag( this,
                             "Датчик подпора",
                             ".PS", true, false,
                             PS_NOnotNC );

    if( _engine->KMforward->isDetected() )
        _PS->needBeDetectedAlarm();
    else _PS->needBeUndetectedAlarm();

    connect( _engine->KMforward, &InDiscretETag::s_detected,   _PS, &InDiscretETag::needBeDetectedAlarm,   Qt::QueuedConnection);
    connect( _engine->KMforward, &InDiscretETag::s_undetected, _PS, &InDiscretETag::onlyChange, Qt::QueuedConnection);
    connect( _engine->startForward, &OutDiscretETag::s_on,   _PS, &InDiscretETag::needBeDetectedAlarm,   Qt::QueuedConnection);
    connect( _engine->startForward, &OutDiscretETag::s_off, _PS, &InDiscretETag::onlyChange, Qt::QueuedConnection);
}
