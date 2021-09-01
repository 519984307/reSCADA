//#include <Simargl/score.h>
#include <qdebug.h>
#include "TransporterPStoKM.h"
#include "electroengine.h"
#include "../../SCADAenums.h"
#include "InDiscretETag.h"
#include "OutDiscretETag.h"


TransporterPStoKM::TransporterPStoKM( int ID,
                                      QString Name,
                                      QString TagPefix,
                                      std::vector<QString> EngSensorNames,
                                      bool PS_NOnotNC )

    : OneEngRouteUnit(  Prom::TypeTransporter,
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
    else _PS->onlyChange();

    connect( _engine->KMforward, &InDiscretETag::Detected,   _PS, &InDiscretETag::needBeDetectedAlarm,   Qt::QueuedConnection);
    connect( _engine->KMforward, &InDiscretETag::Undetected, _PS, &InDiscretETag::onlyChange, Qt::QueuedConnection);
    connect( _engine->startForward, &OutDiscretETag::s_on,   _PS, &InDiscretETag::needBeDetectedAlarm,   Qt::QueuedConnection);
    connect( _engine->startForward, &OutDiscretETag::s_off, _PS, &InDiscretETag::onlyChange, Qt::QueuedConnection);
}
