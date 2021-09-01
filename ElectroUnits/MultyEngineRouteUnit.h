#ifndef MULTYEGINEROUTEUNIT_H
#define MULTYEGINEROUTEUNIT_H

#include "EngRouteUnit.h"

class ElectroEngine;

class MultyEngineRouteUnit : public EngRouteUnit
{
    Q_OBJECT

public:
    explicit MultyEngineRouteUnit(Prom::UnitType Type,
                                   int ID,
                                   QString Name,
                                   QString TagPefix,
                                   int EngineCount,
                                   std::vector<QString> EngSensorNames,
                                   bool BlockedStop = false);

protected:

    std::vector<ElectroEngine *> _engins;

    bool _start() override;
    void _stop() override;
    Prom::EngineStates _getEngNowState() override;


    // Unit interface
protected slots:
    bool _resetAlarm( bool upClassAlarm = false) override;
};

#endif // MULTYEGINEMACHINE_H
