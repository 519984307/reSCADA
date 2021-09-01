#include <qdebug.h>
#include "NoriaSSPSTESnd.h"
#include "SCADAenums.h"
#include "InDiscretETag.h"
#include "OutDiscretETag.h"
#include "electroengine.h"


NoriaSSPSTESnd::NoriaSSPSTESnd( int ID,
                  QString Name,
                  QString TagPefix,
                  std::vector<QString> EngSensorNames )

    : Noria(  ID,
              Name,
              TagPefix,
              EngSensorNames )
{
    _TE = new InDiscretETag( this,
                               "Датчик схода ленты №1",
                               ".TE", true, false,
                               true, false );
    _TE->needBeDetectedAlarm();
    _SS = new InDiscretETag( this,
                              "Датчик движения",
                              ".SS", true, false,
                              true, true, true );

    _SS->pulsSensor( true );
    if( _engine->startForward->isOn() )_SS->needBeDetectedAlarm();
    else _SS->needBeUndetectedNoAlarm();
    connect( _engine->startForward,  &OutDiscretETag::s_on,  _SS, &InDiscretETag::needBeDetectedAlarm     , Qt::QueuedConnection);
    connect( _engine->startForward,  &OutDiscretETag::s_off, _SS, &InDiscretETag::needBeUndetectedNoAlarmNoTime , Qt::QueuedConnection);

    _PS = new InDiscretETag( this,
                              "Датчик подпора",
                              ".PS", true, false,
                              true, false );
    _PS->needBeDetectedAlarm();
    connect( _engine->startForward,  &OutDiscretETag::s_on,  _PS, &InDiscretETag::needBeDetectedAlarm     , Qt::QueuedConnection);

    _sound = new OutDiscretETag( this, Prom::PreSet, "Сирена предупреждения", ".sound");
    connect( _engine->startForward,  &OutDiscretETag::s_on, _sound, &OutDiscretETag::s_on, Qt::QueuedConnection);
    _sound->pulsOut( true );

}
