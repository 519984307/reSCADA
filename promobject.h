#ifndef PROMOBJECT_H
#define PROMOBJECT_H

#include <QVector>
#include <QObject>
#include <QVariant>
#include <QThread>
#include <QDateTime>
#include <QQmlContext>

#ifndef SCADAENUMS_H
#include "../../SCADAenums.h"
#endif // SCADAENUMS_H

class QSettings;
class Route;
class TransporterVSM;
class OWUnit;
class NoriaOW;
class NoriaVSM;
class OneEngRouteUnit;
class STagTree;
class Static2posUnit;
class Unit;
class QFile;
class Log;
class LogThread;
class QDateTime;
class TSP;
class TSPWindow;
class QSound;

class PromObject : public QObject
{
    Q_OBJECT
public:
    explicit PromObject( /*QQmlContext *QMLcontext,*/
                         QObject *GUI = nullptr,
                         QObject *parent = nullptr);
    ~PromObject();
    QSettings * iniUnit;
    QSettings * iniRoute;
    TSP * tsp  = nullptr;

    bool SetCurrentRoute(Route * pRoute);
    void MegaRescan();
    Route * ptrCurrentRoute() { return _currentRoute; }
    void addUnit(Unit * Un);
    void addSubUnit(Unit * Un);
    int getCurrUserN() const;
    Unit * UnitByName(QString Name);
    QQmlContext *getQMLcontext() const;

signals:
    void s_globalRestAlarm();
    void s_globalStop(bool);
    void s_globalRescanSig();
    void s_moverStop();
    void s_loaded();

    void s_routeCreated    (QVariant);         //для визуализации
    void s_routeDeleted    (QVariant);         //для визуализации
    void s_routeStart      (QVariant);         //для визуализации
    void s_routeStarted    (QVariant);         //для визуализации
    void s_routePartStarted(QVariant);         //для визуализации
    void s_routeClean      (QVariant);         //для визуализации
    void s_routeStop       (QVariant);         //для визуализации
    void s_routeStoped     (QVariant);         //для визуализации
    void s_routeCleaning   (QVariant);         //для визуализации
    void s_routeSetCurrent (QVariant);         //для визуализации
    void s_routeNameUpdate (QVariant, QVariant);//для визуализации
    void s_routeUnitUpdate (QVariant, QVariant);//для визуализации
    void s_logging(Prom::MessType MessTypeID, QDateTime DateTime, QString User, QString Source, QString Message);
    void s_setCurrentRoute(Route *pRoute);
    void s_addMessage(QVariant Date, QVariant Time, QVariant Source, QVariant Message);
    //void preDelSelf();

public slots:
    void alarmDo();
    void CreateRoute    ();
    void DeleteRoute    (int id);              //для визуализации
    void StartRoute     (int id);              //для визуализации
    void FreezeRoute    (int id);              //для визуализации
    void StopRoute      (int id);              //для визуализации
    void CleanRoute     (int id);              //для визуализации
    void DeleteRouteLastUnit(int id);          //для визуализации
    void ClearRoute     (int id);              //для визуализации
    void SetRouteCurrent(int id);              //для визуализации
    void UpdateRouteName(int id, QString name);//для визуализации
    //void delSelf();
    void ShowTags();
    void init();
    void init(QObject *GUI);
    void UserGlobalStop(){ emit s_globalStop(true); }
    void SysGlobalStop(){ emit s_globalStop(false); }
    void GlobalRescan();
    void Logging (Prom::MessType MessTypeID,  QDateTime DateTime, bool UserOrSys, QString Source, QString Message);
    void saveSettings( QString FileName = "" );
    void saveSettingsNP(){saveSettings();};
    //void connectToGUI(Unit * unit);

protected:
    bool _deleted = false;
    QQmlContext *_QMLcontext{nullptr};
    QObject * _gui{nullptr};
    //SCore *  _core;
    int _genRouteID = 1;
    //void _alarmSound();
    //void _moverAlarmSound();
    Route * _currentRoute = nullptr;
    QList <Route *> _RouteList;
    Route * _routeByID(int id);
    int _indOfRouteByID(int id);
    STagTree *tagTree = nullptr;
    QVector<Unit*> _units;
    int currUserN  = 1;
    QThread * _unitsThread;
    QThread * _routeThread;
    QThread * _tspThread;
    LogThread * _logThread;
    void _createRoute(int ID);
    TSPWindow * _tspWin = nullptr;
    void _createUnits();
private:
    QSound * _alarmSoundF = nullptr;
    pthread_mutex_t mutex;
};


#endif
