#ifndef ENGROUTEUNIT_H
#define ENGROUTEUNIT_H

#include "RouteUnit.h"

class EngRouteUnit : public RouteUnit
{
    Q_OBJECT

public:
    explicit EngRouteUnit(Prom::UnitType Type,
                           int *Id,
                           QString Name,
                           QString TagPefix,
                           bool Mover,
                           std::vector<QString> EngSensorNames,
                           bool BlockedStop = false);

protected:

    virtual Prom::SetModeResp _customSetMode(Prom::UnitModes *mode, bool UserOrSys) override;
    virtual bool _start() = 0;
    virtual void _stop() = 0;
    virtual Prom::EngineStates _getEngNowState() = 0;
private:
    bool _blockedStop = false;

signals:
    void StartComand();      //для визуализации
    void Started();         //для визуализации
    void StopComand();     //для визуализации
    void Stoped();        //для визуализации
    void Cleaning();
    void ManualStarted();//для визуализации
    void NoDef();       //для визуализации

protected slots:
    void _customConnectToGUI(QObject * guiItem,  QObject * propWin) override;

public slots:

    virtual void AddToCurrentRoteStoped()  { _AddToCurrentRoute(Prom::UnMdStop ); }//для визуализации
    virtual void AddToCurrentRoteStarted() { _AddToCurrentRoute(Prom::UnMdStart); }//для визуализации
    virtual void Start()      { if(! _blocked)setMode(Prom::UnMdStart, true); }//для визуализации
    virtual void Stop()       { if(! (_blockedStop && _blocked))setMode(Prom::UnMdStop, true); }//для визуализации
    //virtual void BlockedStop(){ if(! _blocked)SetMode(Prom::UnMdStop, true); }//для визуализации
    virtual void CleanStop()  { if(! _blocked)setMode(Prom::UnMdCleanStop, true); } ;//для визуализации

    void _updateStateAndMode() override;
    void _cleanTimeEnd() override;

};

#endif // ENGROUTEUNIT_H
