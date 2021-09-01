#ifndef DRUER1VSM_ROSTOK_H
#define DRUER1VSM_ROSTOK_H

#include "RouteUnit.h"
#include <QTimer>

class InDiscretETag;
class OutDiscretETag;
class InETag;
class OutETag;
class Static2posUnit;

class Druer1VSM_Rostok : public RouteUnit
{
    Q_OBJECT
public:
    explicit Druer1VSM_Rostok(  int ID,
                               QString Name,
                               QString TagPefix );

    bool addSubUnit( Unit * unit ) override;
protected:

    OutDiscretETag * _sound = nullptr;
    OutDiscretETag * _alarmLamp  = nullptr;
    OutDiscretETag * _start      = nullptr;
    OutDiscretETag * _cooling    = nullptr;
    OutDiscretETag * _coolingStop= nullptr;
    OutDiscretETag * _alarmStop  = nullptr;
    OutDiscretETag * _smoke      = nullptr;
    OutDiscretETag * _fire       = nullptr;
    OutDiscretETag * _resetAppAlarm = nullptr;

    InDiscretETag * _AS     = nullptr;
    InDiscretETag * _smokeS = nullptr;
    InDiscretETag * _fireS  = nullptr;

    OutETag * _coolingTemp   = nullptr;
    OutETag * _coolingDelay  = nullptr;
    OutETag * _brsStartDelay = nullptr;
    OutETag * _CFstartDelay  = nullptr;

    InETag * _tempHL  = nullptr;
    InETag * _tempML  = nullptr;
    InETag * _tempLL  = nullptr;
    InETag * _tempOut  = nullptr;
//    InETag * _tempIn  = nullptr;

    OutETag * _tempHLAlarmLvl  = nullptr;
    OutETag * _tempMLAlarmLvl  = nullptr;
    OutETag * _tempLLAlarmLvl  = nullptr;
    OutETag * _tempOutAlarmLvl  = nullptr;
//    OutETag * _tempInAlarmLvl  = nullptr;

    QVector <Unit *> CFs;
    Unit * _bunker = nullptr;
    Unit * _unloadTable = nullptr;
    Unit * _burner = nullptr;

    Prom::SetModeResp _customSetMode( Prom::UnitModes *mode, bool UserOrSys ) override;
    void _alarmSubUnitDo( Unit * unit ) override;
    void _alarmDo() override;
    void _resetAlarmDo() override;

private:
    Prom::SetModeResp _startSubUnitByType( Prom::UnitType type, bool UserOrSys );
    Prom::SetModeResp _stopSubUnitByType( Prom::UnitType type, bool UserOrSys );
    void _setblockSubUnitByType( Prom::UnitType type, bool block = true );
    Prom::UnitModes _chackModByType( Prom::UnitType type );
    //Prom::UnitModes _CFmode = Prom::UnMdStop;

signals:
    void SetCoolDelay( QVariant );
    void Stoped();
    void Started();

public slots:
    void AddToCurrentRoteStarted() { _AddToCurrentRoute( Prom::UnMdStart );}
    void AddToCurrentRoteStoped()  { _AddToCurrentRoute( Prom::UnMdStop ); }
    void _updateStateAndMode() override;

    void Cooling(){ setMode( Prom::UnMdColling, true ); }
    void AlarmStop(){ setMode( Prom::UnMdFreeze, true ); }
    void Smoke(){ setMode( Prom::UnMdSmoke, true ); }
    void Fire(){ setMode( Prom::UnMdFire, true ); }
    void Start() { if( ! _blocked ) setMode( Prom::UnMdStart, true ); }
    void Stop() { setMode( Prom::UnMdStop, true ); }
    void CoolingStop(){  setMode( Prom::UnMdCollingStop, true ); }

protected slots:
    void _customConnectToGUI( QObject * guiItem,  QObject * propWin ) override;
    void _updateSubUnitMode( Unit * unit ) override;
    void _updateSubUnitState( Unit * unit ) override;

    // Unit interface
    bool _resetAlarm( bool upClassAlarm = false ) override;
};

#endif // DRUER1VSM_ROSTOK_H
