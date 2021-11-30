//#include <Simargl/score.h>
#include <qdebug.h>
#include "MultyEngineRouteUnit.h"
#include "electroengine.h"
#include <QTimer>

MultyEngineRouteUnit::MultyEngineRouteUnit(Prom::UnitType Type,
                                            int ID,
                                            QString Name,
                                            QString TagPefix,
                                            int EngineCount,
                                            std::vector<QString> EngSensorNames,
                                            bool BlockedStop)
    : EngRouteUnit(Type,
                    ID,
                    Name,
                    TagPefix,
                    true,
                    EngSensorNames,
                    BlockedStop)
{
    if(EngineCount > 0){
        for(int i = 0; i < EngineCount; i ++){
            _engins.push_back(new ElectroEngine(this , EngSensorNames, QString(".e" + QString::number(i + 1))));
            connect(_engins[ i ], &ElectroEngine::Alarm, this, &MultyEngineRouteUnit::detectAlarm , Qt::QueuedConnection);
            connect(_engins[ i ], &ElectroEngine::StateUpdate, this, &MultyEngineRouteUnit::updateState , Qt::QueuedConnection);
        }
    }
    else {
        _alarmInit = true;
        detectAlarm("Недопустимое колличество двигателей (" + QString::number(EngineCount) + ").");
    }
}

//------------------------------------------------------------------------------
bool MultyEngineRouteUnit::_start()
{
    bool started = false;
    foreach(ElectroEngine * engine, _engins){
        started = engine->StartForward();
    }
    if(! started){
        _stop();
    }
    return started;
}
//------------------------------------------------------------------------------
void MultyEngineRouteUnit::_stop()
{
    foreach(ElectroEngine * engine, _engins){
        engine->Stop();
    }
}
//------------------------------------------------------------------------------
Prom::EngineStates MultyEngineRouteUnit::_getEngNowState()
{
    static Prom::EngineStates enSt = Prom::EngNoDef;
    if(_engins[ 0 ]){
        enSt = _engins[ 0 ]->GetNowState();
        foreach(ElectroEngine * engine, _engins){
            if(engine->GetNowState() > enSt) enSt = engine->GetNowState();
        }
    }
    return enSt;
}

bool MultyEngineRouteUnit::_resetAlarm( bool upClassAlarm)
{
    _alarm = upClassAlarm;
    foreach (ElectroEngine *eng, _engins) {
        _alarm |= eng->isAlarm();
    }
    return Unit::_resetAlarm(_alarm);
}

//------------------------------------------------------------------------------

