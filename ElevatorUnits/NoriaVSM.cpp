//
#include <qdebug.h>
#include "NoriaVSM.h"
#include "electroengine.h"
#include "SCADAenums.h"
#include "InDiscretETag.h"
#include "OutDiscretETag.h"


NoriaVSM::NoriaVSM( int ID,
                    QString Name,
                    QString TagPefix )

    : Noria(  ID,
              Name,
              TagPefix,
              Prom::EF_S1KM )
{
    _PS = new InDiscretETag( this,
                             "Датчик подпора",
                             ".PS", true, false,
                             true, true );

    _PS->needBeDetectedAlarm();
    connect( _engine->startForward,  &OutDiscretETag::s_on,  _PS, &InDiscretETag::needBeDetectedAlarm     , Qt::QueuedConnection);

    _RDCS = new InDiscretETag( this,
                               "Реле контроля скорости",
                               ".RDCS", true, false,
                               true );
    if( _engine->startForward->isOn() )_RDCS->needBeDetectedAlarm();
    else _RDCS->needBeUndetectedNoAlarm();
    connect( _engine->startForward, &OutDiscretETag::s_on,  _RDCS, &InDiscretETag::needBeDetectedAlarm , Qt::QueuedConnection);
    connect( _engine->startForward, &OutDiscretETag::s_off, _RDCS, &InDiscretETag::needBeUndetectedNoAlarm , Qt::QueuedConnection);

    _TE1 = new InDiscretETag( this,
                              "Датчик схода ленты №1",
                              ".TE1", true, false,
                              true, false );

    _TE1->needBeDetectedAlarm();
    _TE2 = new InDiscretETag( this,
                              "Датчик схода ленты №2",
                              ".TE2", true, false,
                              true, false );

    _TE2->needBeDetectedAlarm();
}
