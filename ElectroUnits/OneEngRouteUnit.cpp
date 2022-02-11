
#include <qdebug.h>
#include "OneEngRouteUnit.h"
#include "electroengine.h"
#include "InDiscretETag.h"

OneEngRouteUnit::OneEngRouteUnit(Prom::UnitType Type,
                                  int *Id,
                                  QString Name,
                                  QString TagPefix,
                                  bool Mover,
                                  std::vector<QString> EngSensorNames,
                                  bool BlockedStop)
    : EngRouteUnit(Type,
                    Id,
                    Name,
                    TagPefix,
                    Mover,
                    EngSensorNames,
                    BlockedStop)
{
    _engine = new ElectroEngine(this , EngSensorNames);
    connect(_engine, &ElectroEngine::Alarm, this, &Unit::detectAlarm, Qt::QueuedConnection);
    connect(_engine, &ElectroEngine::StateUpdate, this, &Unit::updateState , Qt::QueuedConnection);
}

//------------------------------------------------------------------------------
bool OneEngRouteUnit::_start()
{
    if(_engine) return _engine->StartForward();
    else return false;
}

//------------------------------------------------------------------------------
void OneEngRouteUnit::_stop()
{
    if(_engine) _engine->Stop();
}

//------------------------------------------------------------------------------
EngineStates OneEngRouteUnit::_getEngNowState()
{
    if(_engine) return _engine->GetNowState();
    else return Prom::EngNoDef;
}
//------------------------------------------------------------------------------
bool OneEngRouteUnit::_resetAlarm(bool upClassAlarm)
{
    _alarm = upClassAlarm;
    _alarm |= _engine->isAlarm();
    return Unit::_resetAlarm(_alarm);
}

//------------------------------------------------------------------------------

