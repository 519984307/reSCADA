//#include <Simargl/score.h>
#include <qdebug.h>
#include "TransporterPS1SS1CB1Snd.h"
#include "electroengine.h"
#include "../../SCADAenums.h"
#include "InDiscretETag.h"
#include "OutDiscretETag.h"


TransporterPS1SS1CB1Snd::TransporterPS1SS1CB1Snd( int ID,
                                  QString Name,
                                  QString TagPefix,
                                  std::vector<QString> EngSensorNames,
                                  bool PS_NOnotNC,
                                  bool CB_NOnotNC )

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
                             PS_NOnotNC, true );

    _PS->needBeUndetectedAlarm();

    _SS = new InDiscretETag( this,
                              "Датчик движения",
                              ".SS", true, false,
                              true, true, true );
    _SS->pulsSensor( true );
    if( _engine->startForward->isOn() )_SS->needBeDetectedAlarm();
    else _SS->needBeUndetectedNoAlarm();
    connect( _engine->startForward,  &OutDiscretETag::s_on,  _SS, &InDiscretETag::needBeDetectedAlarm     , Qt::QueuedConnection);
    connect( _engine->startForward,  &OutDiscretETag::s_off, _SS, &InDiscretETag::needBeUndetectedNoAlarm , Qt::QueuedConnection);

    _CB = new InDiscretETag( this,
                             "Датчик обрава цепи",
                             ".CB", true, false,
                             CB_NOnotNC, false );
    _CB->needBeDetectedAlarm();

    _sound = new OutDiscretETag( this, Prom::PreSet, "Сирена предупреждения", ".sound");
    connect( _engine->startForward,  &OutDiscretETag::s_on, _sound, &OutDiscretETag::s_on , Qt::QueuedConnection);
    _sound->pulsOut( true );
}
