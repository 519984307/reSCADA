
#include <qdebug.h>
#include "LocalFilterOW.h"
#include "electroengine.h"
#include "SCADAenums.h"
#include "InDiscretETag.h"
#include "OutDiscretETag.h"


LocalFilterOW::LocalFilterOW( int ID,
                              QString Name,
                              QString TagPefix,
                              std::vector<QString> EngSensorNames )

    : OneEngRouteUnit(  Prom::TypeLocalFilter,
                        ID,
                        Name,
                        TagPefix,
                        true,
                        EngSensorNames )
{
    _FI = new OutDiscretETag( this, Prom::PreSet, "Пуск фильтра", "-FI.start");
    connect( _engine->startForward, &OutDiscretETag::s_on, _FI, &OutDiscretETag::on );
    connect( _engine->startForward, &OutDiscretETag::s_off, _FI, &OutDiscretETag::off );
}
