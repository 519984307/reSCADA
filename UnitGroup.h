#ifndef UNITGROUP_H
#define UNITGROUP_H
#include <QVariant>
#include <QObject>
#include <QDateTime>

#ifndef SCADAENUMS_H
#include "../../SCADAenums.h"
#endif // SCADAENUMS_H

class Route;
class QSettings;
class PromObject;
class ElectroEngine;
class ETag;
class QTimer;
class Unit;
//struct UnitGroup {
//    UnitGroup(){};
//    QVector< Unit * > units;
//    Prom::UnitModes _setedMode = Prom::UnMdNoDef;
//    Prom::UnitModes _currentMode = Prom::UnMdNoDef;
//    Prom::UnitStates _currState = Prom::UnStNoDef;
//    QVector< Prom::UnitModes > allOrNoOneModes;
//    QVector< Prom::UnitModes > atLeastOneModes;
//    QVector< Prom::UnitStates > allOrNoOneStates;
//    QVector< Prom::UnitStates > atLeastOneStates;
//};

class UnitGroup : public QObject
{
    Q_OBJECT

public:

    explicit UnitGroup( Unit * owner,
                        QString Name,
                        Prom::UnitModes SaveMode = Prom::UnMdNoDef,
                        QSettings * Ini = nullptr );

    const Prom::UnitModes saveMode;
    const Unit * owner = nullptr;
    QSettings *ini;

    virtual void addUnit( Unit * unit );
    QVector<Unit *> units() const;

    void setBlocked( bool blocked );
    bool isBlocked() { return _blocked; }

    virtual Prom::SetModeResp setMode( Prom::UnitModes mode, bool UserOrSys );
    Prom::UnitModes setedMode() { return _setedMode ; }
    Prom::UnitModes currentMode() { return _currentMode ; }
    bool inSaveMode() const { if( saveMode == Prom::UnMdNoDef ) return true;
        else return  _currentMode == saveMode; }
    Prom::UnitStates currentState() { return _currentState ; }
    Prom::UnitStates prevState(){ return _prevState; }

protected:
    QVector < Unit* > _units;
    bool _blocked = false;
    Prom::UnitModes _prevMode = Prom::UnMdNoDef;
    Prom::UnitStates _prevState = Prom::UnStNotConnected;

    virtual Prom::SetModeResp _customSetMode( Prom::UnitModes *mode, bool UserOrSys ) = 0;
    virtual void _updateStateAndMode() = 0;

    virtual void _alarmDo();
    virtual void _resetAlarmDo() {};
    virtual void _doOnModeChange() = 0;
    void _checkMode();
    void _checkState();
    void _setCurrentMode( Prom::UnitModes currentMode );
    void _setSetedMode( Prom::UnitModes setedMode );
    void _setCurrentState( Prom::UnitStates currentState);

    Prom::UnitModes _currentMode = Prom::UnMdNoDef;
    Prom::UnitStates _currentState = Prom::UnStNoDef;
    Prom::UnitModes _setedMode = Prom::UnMdNoDef;

signals:
    void ModeChange( UnitGroup * );
    void StateChange( UnitGroup * );
    void LoggingSig  ( Prom::MessType MessTypeID,  QDateTime DateTime, bool UserOrSys, QString Source, QString Message );

public slots:
    void Freeze( bool UserOrSys ) { setMode( Prom::UnMdStop, UserOrSys ); }//одинаково действует на всех юнитах - немедленная остановка
    virtual bool ResetAlarm();
    virtual void UpdateState();
    virtual void SaveParam();
    virtual void LoadParam();
    virtual void RescanUnit();

    void ConnectToGUI( const QObject * GUI );
    virtual void DetectAlarm( QString Description );

protected slots:
    virtual void _CustomConnectToGUI( QObject * guiItem, QObject * propWin ) = 0;
    virtual void _UpdateSubUnitState( Unit * unit );
    virtual void _UpdateSubUnitMode( Unit * unit );
};
#endif //UNITGROUP_H
