#ifndef ROUTE_H
#define ROUTE_H

#include <QVariant>
#include <QObject>
//#include "../../SCADAenums.h"
#include "promobject.h"

class RouteUnit;
class QTimerExt;

class Route : public QObject
{
    Q_OBJECT
public:
    explicit Route(PromObject * Owner, int ID, QString Name = "");
    ~ Route();

    PromObject * owner;
    const int ID;
    bool CheckConnectAllUnits();
    bool IsAllUnuitsConnected(){ return _allUnitsConnected; }
    Prom::UnitModes UnitRouteMode(RouteUnit * unit);
    bool SetUnitRouteMode(RouteUnit * Unit, Prom::UnitModes Mode);
    bool IsActive() const { return _active; }

protected:
    bool _active = false;
    QList <RouteUnit*> _unitList;
    QList <Prom::UnitModes> _unitMode;
    bool _allUnitsConnected = false; /// индикатор подсоединения ВСЕХ юнитов
    enum CommandType{
        UpStepByStep,
        DownStepByStep,
        Parallel
    };
    struct RouteCmd{
        Prom::RouteCommand name = Prom::RtCmNo;        
        CommandType type = Parallel;
        bool workAftFin = false;
        int beginUn = -1;
        int endUn = -1;

        bool finCmd = false;
        bool good = true;
        int last = -1;
        unsigned int respCount = 0;
        unsigned int ascCount = 0;
        bool done();
        unsigned int count();
        int next();        
    };
    //bool _ChekComm(unsigned int Ind);
    //RouteCmd _command;
    QList <RouteCmd> _workCmds;
    QList <RouteCmd> _queueCmds;
    bool _ignorAlarm = false;
    bool _ConnectUnit(RouteUnit * RUnit);
    bool _ConnectAllUnits();
    void _CheckConnection();

    QTimerExt * _sTimer;
    QString _UnitsString();
    int _lastStN = -1;
    RouteCmd * _genCom = nullptr;//главная команда маршрута по ней определяется его состояние
    void SetCommand(RouteCmd Command, bool GnrlCm = false);
    void SetAsCurrent();
    void RescanRoute();
    void Save();
    void Load();

protected slots:
    void _StartTimerSlot();
    void _CommandToUnit(RouteUnit * RUnit, Prom::RouteCommand Command);
    //void _Logging(Prom::MessType MessTypeID, bool UserOrSys, QString Message);
    void SetName(QString name);
    void Start();
    void Stop();
    void Freeze();
    void CleanStop();
    void CleanUnitList();
    void DeleteLastUnit();
    void DisconnectAllUnit();

signals:
    void GetReady();
    void RouteAlarm();

    void Created     (QVariant);          //для визуализации
    void Active      (QVariant);          //для визуализации
    void NotActive   (QVariant);          //для визуализации
    void StartCommand(QVariant);          //для визуализации
    void Started     (QVariant);          //для визуализации
    void PartStarted (QVariant);          //для визуализации
    void StopCommand (QVariant);          //для визуализации
    void Stoped      (QVariant);          //для визуализации
    void CleanCommand(QVariant);          //для визуализации

    void NameUpdate  (QVariant, QVariant);//для визуализации
    void UnitUpdate  (QVariant, QVariant);//для визуализации
    void AddUnitSig  (Route*, Prom::UnitModes);
    void setModeToUnitSig(Prom::UnitModes);
    void Logging  (Prom::MessType MessTypeID, QDateTime DateTime, bool UserOrSys, QString Source, QString Message);
//    void LoggingUser  (Prom::MessType MessTypeID,  QDateTime DateTime, QString Source, QString Message);
//    void LoggingSystem(Prom::MessType MessTypeID,  QDateTime DateTime, QString Source, QString Message);

    void SetNameSig(QString name);

    void StartSig();
    void StopSig();
    void FreezeSig();
    void CleanStopSig();
    void CleanUnitListSig();
    void DeleteLastUnitSig();
    void DisconnectAllUnitSig();


public slots:
    void InformFromUnit(RouteUnit * RUnit, Prom::RouteCommand ResponseCommand, Prom::RouteUnitInfo UnitInfo);
    void AddUnit(RouteUnit * unit, Prom::UnitModes mode);
    bool DelUnit(RouteUnit * unit);
    bool DisconnectUnit(RouteUnit * RUnit);

};

#endif

