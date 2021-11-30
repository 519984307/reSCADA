#include <qdebug.h>
#include "NoriaOW.h"
#include "SCADAenums.h"
#include "InDiscretETag.h"
#include "OutDiscretETag.h"
#include "electroengine.h"


NoriaOW::NoriaOW( int ID,
                  QString Name,
                  QString TagPefix )

    : Noria(  ID,
              Name,
              TagPefix,
              Prom::EF_S1KM )
{
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
    _TE3 = new InDiscretETag( this,
                               "Датчик схода ленты №3",
                               ".TE3", true, false,
                               true, false );
    _TE3->needBeDetectedAlarm();

    _TE4 = new InDiscretETag( this,
                               "Датчик схода ленты №4",
                               ".TE4", true, false,
                               true, false );
    _TE4->needBeDetectedAlarm();
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
                              true, false );
    _PS->needBeDetectedAlarm();
    connect( _engine->startForward,  &OutDiscretETag::s_on,  _PS, &InDiscretETag::needBeDetectedAlarm     , Qt::QueuedConnection);

}
