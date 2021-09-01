#ifndef DRUER1VSM_VELES_H
#define DRUER1VSM_VELES_H

#include "RouteUnit.h"
#include <QTimer>

class InDiscretETag;
class OutDiscretETag;
class InETag;
class OutETag;
class Static2posUnit;

class Druer1VSM_Veles : public RouteUnit
{
    Q_OBJECT
public:
    explicit Druer1VSM_Veles(  int ID,
                               QString Name,
                               QString TagPefix );

    bool addSubUnit( Unit * unit ) override;
protected:

    OutDiscretETag * _alarmSound = nullptr;
    OutDiscretETag * _startSound = nullptr;
    OutDiscretETag * _start      = nullptr;
    OutDiscretETag * _stop       = nullptr;
    OutDiscretETag * _fire       = nullptr;
    OutDiscretETag * _alarmOut      = nullptr;

    InDiscretETag * _alarmIn      = nullptr;

    InETag * _tempHL  = nullptr;
    InETag * _tempML  = nullptr;
    InETag * _tempLL  = nullptr;
    InETag * _tempEx  = nullptr;

    OutETag * _overTempHL  = nullptr;
    OutETag * _overTempML  = nullptr;
    OutETag * _overTempLL  = nullptr;
    OutETag * _overTempEx  = nullptr;

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
    void Start() { if( ! _blocked ) setMode( Prom::UnMdStart, true ); }
    void Stop() { setMode( Prom::UnMdStop, true ); }
    void CoolingStop();

protected slots:
    void _customConnectToGUI( QObject * guiItem,  QObject * propWin ) override;
    void _updateSubUnitMode( Unit * unit ) override;
    void _updateSubUnitState( Unit * unit ) override;
};

#endif // DRUER1VSM_VELES_H
