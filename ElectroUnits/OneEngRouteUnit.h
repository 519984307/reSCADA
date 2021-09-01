#ifndef ONEENGUNIT_H
#define ONEENGUNIT_H

#include "EngRouteUnit.h"

class ElectroEngine;

class OneEngRouteUnit : public EngRouteUnit
{
    Q_OBJECT

public:
    explicit OneEngRouteUnit(Prom::UnitType Type,
                              int ID,
                              QString Name,
                              QString TagPefix,
                              bool Mover,
                              std::vector<QString> EngSensorNames,
                              bool BlockedStop = false);

protected:
    ElectroEngine * _engine = nullptr;

    // EngRouteUnit interface
    bool _start() override;
    void _stop() override;
    Prom::EngineStates _getEngNowState() override;

protected slots:
    // Unit interface
    bool _resetAlarm(bool upClassAlarm = false) override;
};

#endif // ONEENGUNIT_H
