#include <qdebug.h>
#include "OWUnit.h"
#include "electroengine.h"
#include "SCADAenums.h"
#include "InDiscretETag.h"
#include "OutDiscretETag.h"


OWUnit::OWUnit( Prom::UnitType Type,
                int ID,
                QString Name,
                QString TagPefix,
                std::vector<QString> EngSensorNames,
                bool PS_NOnotNC)

    : OneEngRouteUnit(  Type,
                        ID,
                        Name,
                        TagPefix,
                        true,
                        EngSensorNames )
{
    _SS = new InDiscretETag( this,
                             "Датчик движения",
                             ".SS", true, false,
                             true, true, true );

    _SS->pulsSensor( true );
    if( _engine->startForward->isOn() )_SS->needBeDetectedAlarm();
    else _SS->needBeUndetectedNoAlarm();
    connect( _engine->startForward,  &OutDiscretETag::s_on,  _SS, &InDiscretETag::needBeDetectedAlarm     , Qt::QueuedConnection);
    connect( _engine->startForward,  &OutDiscretETag::s_off, _SS, &InDiscretETag::needBeUndetectedNoAlarm , Qt::QueuedConnection);

    _PS = new InDiscretETag( this,
                             "Датчик подпора",
                             ".PS", true, false,
                             PS_NOnotNC, true );
    _PS->needBeDetectedAlarmNoTime();
    connect( _engine->startForward,  &OutDiscretETag::s_on,  _PS, &InDiscretETag::needBeDetectedAlarmNoTime, Qt::QueuedConnection);

}
