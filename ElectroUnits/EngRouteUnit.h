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
                           bool BlockedStop = false,
                           Prom::UnitModes SaveMode = Prom::UnMdStop);

protected:

    virtual Prom::SetModeResp _customSetMode(Prom::UnitModes *mode, bool UserOrSys) override;
    virtual bool _start() = 0;
    virtual void _stop() = 0;
    virtual Prom::EngineStates _getEngNowState() = 0;
private:
    bool _blockedStop = false;

signals:
    void s_startComand();      //для визуализации
    void s_started();         //для визуализации
    void s_stopComand();     //для визуализации
    void s_stoped();        //для визуализации
    void s_cleaning();
    void s_manualStarted();//для визуализации
    void s_noDef();       //для визуализации

protected slots:
    void _customConnectToGUI(QObject * guiItem,  QObject * propWin) override;

public slots:

    virtual void addToCurrentRoteStoped()  { _addToCurrentRoute(Prom::UnMdStop ); }//для визуализации
    virtual void addToCurrentRoteStarted() { _addToCurrentRoute(Prom::UnMdStart); }//для визуализации
    virtual void start()      { if(! _blocked)setMode(Prom::UnMdStart, true); }//для визуализации
    virtual void stop()       { if(! (_blockedStop && _blocked))setMode(Prom::UnMdStop, true); }//для визуализации
    //virtual void BlockedStop(){ if(! _blocked)SetMode(Prom::UnMdStop, true); }//для визуализации
    virtual void cleanStop()  { if(! _blocked)setMode(Prom::UnMdCleanStop, true); } ;//для визуализации

    void _updateStateAndMode() override;
    void _cleanTimeEnd() override;

};

#endif // ENGROUTEUNIT_H
