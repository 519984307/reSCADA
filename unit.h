#ifndef UNIT_H
#define UNIT_H
//#ifndef SCADAENUMS_H

#include <QVariant>
#include <QObject>
#include <QDateTime>
#include "SCADAenums.h"
#include "QTimerExt.h"

class Route;
class QSettings;
class PromObject;
class ElectroEngine;
class ETag;

class Unit : public QObject
{
    Q_OBJECT


public:

    explicit Unit(Prom::UnitType Type,
                   int *Id,
                   QString Name,
                   QString TagPrefix,
                   bool Mover = false,//TODO Убрать в отдельный параметрдочерних классов!
                   Prom::UnitModes SaveMode = Prom::UnMdNoDef,
                   QSettings * Ini = nullptr);

    const Prom::UnitType unitType;
    const Prom::UnitModes saveMode;
    const bool mover = false;
    QString tagPrefix = "";
    QSettings *ini;

    void addETag(ETag * tag);
    bool isAlarm() { return _alarm; }
    bool isAlarmConnection(){ return _alarmConnection;}
    bool isBlocked() { return _blocked; }
    Prom::UnitStates prevState(){ return _prevState; }

    bool inSaveMode() const { if(saveMode == Prom::UnMdNoDef) return true;
                              else return  _currentMode == saveMode; }
    bool isSensorsConnected() { return  _sensorsConnected; }
    bool isFirstLoad() const { return _firstLoad; }
    void setAlarmTag(QString tagName);
    QThread * ownThread = nullptr;
    QVector<ETag *> tags() const { return _tags; }
    QString exName() const { return _exName; };
    bool isMultyUnit() { return _subUnits.count() > 0; }
    virtual bool addSubUnit(Unit * unit);
    QVector<Unit *> subUnits() const;
    void moveToThread(QThread * thread);
    virtual Prom::SetModeResp setMode(Prom::UnitModes mode, bool UserOrSys);
    void setBlocked(bool blocked);

    Prom::UnitModes setedMode() { return _setedMode ; }
    Prom::UnitModes currentMode() { return _currentMode ; }
    Prom::UnitStates currentState() { return _currentState ; }
    bool isCurrOrSetedMode(Prom::UnitModes Mode) { return (_currentMode == Mode || _setedMode == Mode) ; }
    bool isCurrOrSetedModeIn(QVector<Prom::UnitModes> Modes);
    void addTimer(QTimerExt * Timer);
    bool setedModeIn(QVector<Prom::UnitModes> modes);
    bool setedModeNotIn(QVector<Prom::UnitModes> modes);
    bool currentModeIn(QVector<Prom::UnitModes> modes);
    bool currentModeNotIn(QVector<Prom::UnitModes> modes);
    bool stateIn(QVector<Prom::UnitStates> states);
    bool stateNotIn(QVector<Prom::UnitStates> states);
    virtual void allTimerStop();
    bool anySubUnitHaveMode(Prom::UnitModes Mode, Prom::UnitType Type = Prom::TypeNoDef);
    bool sensorsConnected() const  { return _sensorsConnected; }

    PromObject *owner() const;
    void setOwner(PromObject *Owner);

    int id() const;
    void setId(int value);

protected:
    int _id;
    PromObject * _owner{nullptr};
    QVector < ETag* > _tags;
    QVector < Unit* > _subUnits;
    bool _blocked = false;
    bool _firstLoad = true;
    bool _sensorsConnected = true;
    bool _alarmConnection = false;
    bool _alarmInit = false;
//    bool _mayResetAlarm = true;
    QTimerExt * _cleanTimer;
    bool _cleaned = false;

    Prom::UnitModes _prevMode = Prom::UnMdNoDef;
    Prom::UnitStates _prevState = Prom::UnStNotConnected;

    virtual Prom::SetModeResp _customSetMode(Prom::UnitModes *mode, bool UserOrSys) = 0;


    bool _alarm = false;
    QString _exName;
    virtual void _alarmDo();
    virtual void _alarmSubUnitDo(Unit * unit);
    virtual void _resetAlarmDo() {};
    virtual void _doOnModeChange() = 0;
    void _checkMode();
    void _checkState();
    virtual void _setCurrentMode(Prom::UnitModes currentMode, bool resultMode = true);
    virtual void _setSetedMode(Prom::UnitModes setedMode);
    virtual void _rejectSetedMode();
    virtual void _setCurrentState(Prom::UnitStates currentState);

    Prom::UnitModes _currentMode = Prom::UnMdNoDef;
    Prom::UnitStates _currentState = Prom::UnStNoDef;
    Prom::UnitModes _setedMode = Prom::UnMdNoDef;
    bool _useMayResAlarm = false;

private:
    QVector< QTimerExt * > _allTimers;

signals:
    void s_quitAlarm(QVariant);//для визуализации
    void s_alarm(QVariant);//для визуализации
    void s_alarmReseted();
    void s_modeChange(Unit *);
    void s_stateChange(Unit *);
    //void NotConnected();//для визуализации
    void s_loggingSig  (Prom::MessType MessTypeID,  QDateTime DateTime, bool UserOrSys, QString Source, QString Message);
    void s_connected();
    void s_disconnected();
    void s_shangeExName(QVariant exName);
    void s_changeCleanDelay(QVariant Mnt);
    void s_alarmForAnderUnit(Unit *, QString);
    void s_cleanTimerStart();

public slots:
    void freeze(bool UserOrSys) { setMode(Prom::UnMdStop, UserOrSys); }//одинаково действует на всех юнитах - немедленная остановка
    virtual bool resetAlarm(){ return _resetAlarm(); }
    virtual void updateState();
    virtual void saveParam();
    virtual void loadParam();
    virtual void reInitialise();
    void setExName( QString exName) { _exName = exName; }
    void writeCleanDelay(QVariant);
    void logging(Prom::MessType MessTypeID, QDateTime DateTime, bool UserOrSys, QString Source, QString Message);
    bool connectToGUI(const QObject * GUI);
    virtual void detectAlarm(QVariant Description);
    virtual void detectSubUnitAlarm(Unit * unit, QString Description);

protected slots:
    virtual bool _resetAlarm(bool upClassAlarm = false);
    virtual void _cleanTimeEnd(){};
    virtual void _sensorConnect();
    virtual void _customConnectToGUI(QObject * guiItem, QObject * propWin = nullptr) = 0;
    virtual void _updateSubUnitState(Unit *){};
    virtual void _updateSubUnitMode(Unit *){};
    virtual void _updateStateAndMode() = 0;
};
#endif
