#ifndef BURNERSIMP_H
#define BURNERSIMP_H

#ifndef UNIT_H
#include "unit.h"
#endif

#include <QObject>

class InDiscretETag;
class InETag;
class OutETag;
class OutDiscretETag;

class BurnerSimp : public Unit
{
    Q_OBJECT
public:
    explicit BurnerSimp(int ID,
                    QString Name,
                    QString TagPefix,
                    Prom::UnitModes SaveMode = Prom::UnMdStop );

protected:
    OutDiscretETag * _startLG = nullptr;
    InDiscretETag * _startedLG = nullptr;
    InDiscretETag * _alarmLG = nullptr;
    InDiscretETag * _QF = nullptr;
    InDiscretETag * _step = nullptr;
    InETag * _T = nullptr;
    OutETag * _TAlarmLvl  = nullptr;
    OutETag * _setT = nullptr;

    OutETag * _P  = nullptr;
    OutETag * _I = nullptr;
    OutETag * _D  = nullptr;

signals:
    void Started();//для визуализации
    void Stoped(); //для визуализации
    void ManualStarted();//для визуализации
    void NoDef();  //для визуализации

    void SetCurrentTemp( QVariant );//для визуализации
    void SetTargetTemp( QVariant );//для визуализации
    void SetThreshTemp( QVariant );//для визуализации

public slots:
    void _updateStateAndMode() override;
    Prom::SetModeResp _customSetMode( Prom::UnitModes *, bool  ) override { return Prom::RejAnnown; }
    void _doOnModeChange() override{};
protected slots:
    void _customConnectToGUI( QObject *guiItem,  QObject *propWin ) override;
};


#endif // BURNERSIMP_H
