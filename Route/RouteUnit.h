#ifndef ROUTEUNIT_H
#define ROUTEUNIT_H

#ifndef UNIT_H
#include "unit.h"
#endif

#include <QObject>

class Route;

class RouteUnit : public Unit
{
    Q_OBJECT
public:
    explicit RouteUnit(Prom::UnitType Type,
                        int *Id,
                        QString Name,
                        QString TagPefix,
                        bool Mover,
                        Prom::UnitModes SaveMode);
    ~RouteUnit();

    bool InRouteCommand(Route * route, Prom::RouteCommand Command) const;
    Route* MyRoute() { return _myRoute; }
//    Prom::UnitModes routeAlarmMode() const { return _routeAlarmMode; };
//    void setRouteAlarmMode(const Prom::UnitModes &routeAlarmMode){ _routeAlarmMode = routeAlarmMode; };
//    Prom::RouteCommand routeCommand() const { return _routeCommand; };
//    void setRouteCommand(const Prom::RouteCommand &routeCommand) { _routeCommand = routeCommand; };
//    Prom::RouteUnitEventCommand eventCommand() const { return _eventCommand; }
//    void ResetEventCommand();

protected:
    pthread_mutex_t mutex;
    Route * _myRoute = nullptr;
    Prom::UnitModes    _routeMode =      Prom::UnMdNoDef;
    //Prom::UnitModes    _routeAlarmMode = Prom::UnMdFreeze;
    Prom::RouteCommand _routeCommand = Prom::RtCmNo;
    //bool _rtCmDone = false;
    //Prom::RouteUnitEventCommand _eventCommand =   Prom::RtUnNo;
    void _AddToCurrentRoute(Prom::UnitModes mode);
    void _doOnModeChange() override;
//    virtual void _CustomModeOfCommand(Prom::RouteCommand * Command, Prom::UnitModes * Mode) = 0;
    virtual Prom::UnitModes _ModeOfCommand(Prom::RouteCommand *Command);
    virtual bool _midleMode(){ return  false; }
    void _alarmDo() override;

signals:
    void AddInRouteSig(RouteUnit *, Prom::UnitModes);
    void CommandFromRouteSig(Prom::RouteCommand Command);
    void SetInRoute(QVariant NofRoute);
    void InformToRoute(RouteUnit * RUnit, Prom::RouteCommand ResponseCommand, Prom::RouteUnitInfo UnitInfo);

public slots:
    Prom::UnitModes RouteMode(Route * rout);
    //bool SetRouteMode(Route * rout, Prom::UnitModes mode);
    Prom::UnitModes MyRouteMode() { return RouteMode(_myRoute); }
    void CommandFromRoute(Prom::RouteCommand Command);
    bool SetMyRoute(Route * route, Prom::UnitModes mode);
    void AddInRoute(Route * route, Prom::UnitModes mode);
    /*!для визуализации*/

protected slots:

//    void _DetectAlarm(QString Description) override;

};

#endif // ROUTEUNIT_H
