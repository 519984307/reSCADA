#ifndef DRYER3VSM_KROMY_H
#define DRYER3VSM_KROMY_H

#include "RouteUnit.h"
#include <QTimer>
#include "PID.h"

class InDiscretETag;
class OutDiscretETag;
class InETag;
class OutETag;

class Dryer3VSM_Kromy : public RouteUnit
{
    Q_OBJECT
    //    Q_PROPERTY(QVariant targetLowTemp  READ targetLowTemp  WRITE setTargetLowTemp  NOTIFY s_targetLowTempChd)
    //    Q_PROPERTY(QVariant targetHumidity READ targetHumidity WRITE setTargetHumidity NOTIFY s_targetHumidityChd)
    //    Q_PROPERTY(QVariant maxBernerTemp  READ maxBernerTemp  WRITE setMaxBernerTemp  NOTIFY s_maxBernerTempChd)
    //    Q_PROPERTY(QVariant maxProductTemp READ maxProductTemp WRITE setMaxProductTemp NOTIFY s_maxProductTempChd)
    //    Q_PROPERTY(QVariant maxUTSpeed     READ maxUTSpeed     WRITE setMaxUTSpeed     NOTIFY s_maxUTSpeedChd)
    //Q_PROPERTY(PID * brnrTempProdTempPID READ brnrTempProdTempPID)
    //Q_PROPERTY(InETag * temp1 READ temp1)
public:
    explicit Dryer3VSM_Kromy( int ID,
                              QString Name,
                              QString TagPefix);

    bool addSubUnit(Unit * unit) override;

    //    QVariant targetLowTemp() const;
    //    QVariant targetHumidity() const;
    //    QVariant maxProductTemp() const;
    //    QVariant maxUTSpeed() const;
    //    QVariant maxBernerTemp() const;

    void setShHum(  Unit *ShHumIn, bool InOrOut );
    void setBurner( Unit * Berner, uint Number );

protected:
    OutDiscretETag * _vsAlarm { nullptr };
    OutDiscretETag * _vsWarning { nullptr };

    InDiscretETag * _as { nullptr };

    InETag * _temp1  { nullptr };
    InETag * _temp2  { nullptr };
    InETag * _temp3  { nullptr };
    InETag * _temp4  { nullptr };
    InETag * _temp5  { nullptr };
    InETag * _temp6  { nullptr };
    InETag * _temp7  { nullptr };
    InETag * _temp8  { nullptr };
    InETag * _temp9  { nullptr };
    InETag * _temp10 { nullptr };
    InETag * _temp11 { nullptr };
    InETag * _temp12 { nullptr };
    InETag * _temp13 { nullptr };
    InETag * _temp14 { nullptr };
    InETag * _temp15 { nullptr };

    OutETag * _alarmTemp1  { nullptr };
    OutETag * _alarmTemp2  { nullptr };
    OutETag * _alarmTemp3  { nullptr };
    OutETag * _alarmTemp4  { nullptr };
    OutETag * _alarmTemp5  { nullptr };
    OutETag * _alarmTemp6  { nullptr };
    OutETag * _alarmTemp7  { nullptr };
    OutETag * _alarmTemp8  { nullptr };
    OutETag * _alarmTemp9  { nullptr };
    OutETag * _alarmTemp10 { nullptr };
    OutETag * _alarmTemp11 { nullptr };
    OutETag * _alarmTemp12 { nullptr };
    OutETag * _alarmTemp13 { nullptr };
    OutETag * _alarmTemp14 { nullptr };
    OutETag * _alarmTemp15 { nullptr };

    InETag * _inHumidity { nullptr };
    InETag * _outHumidity { nullptr };

    OutDiscretETag * _needLoad       { nullptr };
    OutDiscretETag * _needUnload     { nullptr };
    OutDiscretETag * _needCircul     { nullptr };
    InDiscretETag  * _load   { nullptr };
    InDiscretETag  * _unload  { nullptr };

    bool _autoLoad { false };
    //bool _unloadOn { false };
    //bool _circuleOn { true };

    QTimerExt * _startCFDelay;
    QTimerExt * _startBrDelay;
    QTimerExt * _stopBrDelay;
    QTimerExt * _coolingTimer;
    QTimerExt * _humIn0StopTimer;
    QTimerExt * _humOut0StopTimer;

    int lastStartCF { -1};
    QVector <Unit *> CFs;
    QVector <Unit *> Brs;

    Prom::SetModeResp _customSetMode(Prom::UnitModes *mode, bool UserOrSys) override;

    void _alarmSubUnitDo(Unit * unit) override;
    void _alarmDo() override;
    Unit * _bunker { nullptr };
    Unit * _unloadTable { nullptr };
    void _setCurrentState(Prom::UnitStates CurrentState) override ;
    void _resetAlarmDo() override;

    QVariant _avProdLowTemp1();
    QVariant _avProdLowTemp2();
    QVariant _avProdLowTemp3();
    QVariant _targetLowTemp{0};
    QVariant _targetHumidity{0};
    QVariant _maxBernerTemp{0};
    QVariant _maxProductTemp{0};
    QVariant _maxUTSpeed{0};
//    void _allPIDstart();
//    void _allPIDstop();

    Unit * _shHumIn{ nullptr };
    Unit * _shHumOut{ nullptr };

    Unit * _burner1{ nullptr };
    Unit * _burner2{ nullptr };
    Unit * _burner3{ nullptr };

private:
    Prom::SetModeResp _startSubUnitByType(Prom::UnitType type, bool UserOrSys);
    Prom::SetModeResp _stopSubUnitByType(Prom::UnitType type, bool UserOrSys);
    Prom::SetModeResp _commandToSubUnitByType(Prom::UnitType type,
                                              Prom::UnitModes Command,
                                              bool UserOrSys,
                                              QVector<Prom::UnitModes>TargetUnitModes = {});
    void _setblockSubUnitByType(Prom::UnitType type, bool block = true);
    Prom::UnitModes _commonModeByType(QVector<Unit*> subUnits);
    bool _chackSubUnitModeByType(Prom::UnitType type, Prom::UnitModes Mode);
    Prom::UnitModes _CFmode = Prom::UnMdStop;
    Prom::UnitModes _BrMode = Prom::UnMdStop;
    PID *_brnrTempProdTempPID1;
    PID *_brnrTempProdTempPID2;
    PID *_brnrTempProdTempPID3;
    PID *_prodTempHumPID;
    //PID *_UTSpeedHumPID;
    bool _autoHumidity{false};
    bool _autoTemp{false};
    bool _auto0Stop{false};

signals:
    void s_test(QString);
    void s_coolDelayChd(QVariant);
    void s_startCFDelayChd(QVariant);
    void s_startBrDelayChd(QVariant);
    void s_stopBrDelayChd(QVariant);
    void SetStatus(QVariant);
    void SetCircule(QVariant);

    void s_passUnload(QVariant);
    void s_passCircule(QVariant);
    void s_passLoad(QVariant);

    void s_targetLowTempChd(QVariant);
    void s_targetHumidityChd(QVariant);
    void s_maxBernerTempChd(QVariant);
    void s_maxProductTempChd(QVariant);
    void s_maxUTSpeedChd(QVariant);

    void s_brnrTempProdTempPID_KpChd  (QVariant);
    void s_brnrTempProdTempPID_KiChd  (QVariant);
    void s_brnrTempProdTempPID_KdChd  (QVariant);
    void s_brnrTempProdTempPID_TdChd  (QVariant);
    void s_brnrTempProdTempPID_MaxUChd(QVariant);
    void s_brnrTempProdTempPID_MinUChd(QVariant);

    void s_prodTempHumPID_KpChd       (QVariant);
    void s_prodTempHumPID_KiChd       (QVariant);
    void s_prodTempHumPID_KdChd       (QVariant);
    void s_prodTempHumPID_TdChd       (QVariant);
    void s_prodTempHumPID_MaxUChd     (QVariant);
    void s_prodTempHumPID_MinUChd     (QVariant);

    void s_UTSpeedHumPID_KpChd        (QVariant);
    void s_UTSpeedHumPID_KiChd        (QVariant);
    void s_UTSpeedHumPID_KdChd        (QVariant);
    void s_UTSpeedHumPID_TdChd        (QVariant);
    void s_UTSpeedHumPID_MaxUChd      (QVariant);
    void s_UTSpeedHumPID_MinUChd      (QVariant);

    void s_autoHumidityChd            (QVariant);
    void s_autoTempChd                (QVariant);
    void s_auto0StopChd               (QVariant);

public slots:
    void AddToCurrentRoteStarted() { _AddToCurrentRoute(Prom::UnMdStart);}
    void AddToCurrentRoteStoped()  { _AddToCurrentRoute(Prom::UnMdStop); }

    void Start();
    void Stop();
    void Cooling();
    void CoolingStop();
    void GetCoolDelay(QVariant);
    void GetStartCFDelay(QVariant);
    void GetStartBrDelay(QVariant);
    void setStopBrDelay(QVariant);
    void saveParam() override;
    void loadParam() override;
    void reInitialise() override;

    void setUnload(bool unload);
    void setCircule(bool circule);
    void setAutoLoad(bool load);

protected slots:
    void _updateStateAndMode() override;
    void _customConnectToGUI(QObject * guiItem,  QObject * propWin) override;
    void _updateSubUnitMode(Unit * unit) override;
    void _updateSubUnitState(Unit * unit) override;
    void _coolingTimerEnd();
    void _startCFbyTimer();
    void _startBrByTimer();
    void _stopBrByTimer();
    bool _resetAlarm( bool upClassAlarm = false) override;

    void setTargetLowTemp( QVariant Temp );
    void setTargetHumidity( QVariant Hymidity );

    void setMaxBernerTemp(QVariant MaxBernerTemp);
    void setMaxProductTemp(QVariant);
    //void setMaxUTSpeed(QVariant);

    void setBrnrTempProdTempPID_Kp  (QVariant Kp  );
    void setBrnrTempProdTempPID_Ki  (QVariant Ki  );
    void setBrnrTempProdTempPID_Kd  (QVariant Kd  );
    void setBrnrTempProdTempPID_Td  (QVariant Td  );
    void setBrnrTempProdTempPID_MaxU(QVariant MaxU);
    void setBrnrTempProdTempPID_MinU(QVariant MinU);
    void setProdTempHumPID_Kp       (QVariant Kp  );
    void setProdTempHumPID_Ki       (QVariant Ki  );
    void setProdTempHumPID_Kd       (QVariant Kd  );
    void setProdTempHumPID_Td       (QVariant Td  );
    void setProdTempHumPID_MaxU     (QVariant MaxU);
    void setProdTempHumPID_MinU     (QVariant MinU);
//    void setUTSpeedHumPID_Kp        (QVariant Kp  );
//    void setUTSpeedHumPID_Ki        (QVariant Ki  );
//    void setUTSpeedHumPID_Kd        (QVariant Kd  );
//    void setUTSpeedHumPID_Td        (QVariant Td  );
//    void setUTSpeedHumPID_MaxU      (QVariant MaxU);
//    void setUTSpeedHumPID_MinU      (QVariant MinU);

    void _setAutoHumidity(bool);
    void _setAutoTemp(bool);
    void _setHumAuto0Stop(bool);

    void _humInAuto0Stop();
    void _humOutAuto0Stop();


    void _conditiontHumPID();
    void _conditionAllTempPid();

    void _loadUnloadCireculManag();
    void _bunkerBelowLL();

    // Unit interface
};

#endif // DRYER3VSM_KROMY_H




//enum SubUnitConnandType{
//    Parallel,
//    Up,
//    Down
//};

//struct SubUnitGroup{
//    QVector<Unit *> units;
//    Prom::UnitModes  gropMode = Prom::UnMdNoDef;
//    Prom::UnitModes  cmdMode = Prom::UnMdNoDef;
//    Prom::UnitStates cmdState = Prom::UnStNoDef;
//    SubUnitConnandType cmType = Parallel;
//    bool UsrOrSys = false;
//    unsigned int cmdN = 0;
//    QTimer cmdTimer;
//    void addUnit(Unit * unit);
//    void subUnitModeChange(Unit * unit);
//    void subUnitStateChange(Unit * unit);

//    bool cmdToNext();

//};
