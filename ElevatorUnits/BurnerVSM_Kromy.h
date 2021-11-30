#ifndef BURNERVSM_KROMY_H
#define BURNERVSM_KROMY_H
#include <QObject>
#include "RouteUnit.h"

class InDiscretETag;
class InETag;
class OutETag;
class OutDiscretETag;

class BurnerVSM_Kromy : public RouteUnit
{
    Q_OBJECT
public:
    explicit BurnerVSM_Kromy(int ID,
                              QString Name,
                              QString TagPefix);

protected:
    OutDiscretETag * _start = nullptr;
    OutETag * _targetTemp = nullptr;
    OutETag * _alarmTemp = nullptr;
    InETag * _temp = nullptr;
    OutDiscretETag * _ignorTRM = nullptr;

    InDiscretETag * _relayKM = nullptr;
    InDiscretETag * _startSig = nullptr;

    InDiscretETag * _startedPID = nullptr;
    InDiscretETag * _lessPID = nullptr;
    InDiscretETag * _morePID = nullptr;
    Prom::SetModeResp _customSetMode(Prom::UnitModes *mode, bool UserOrSys) override;

signals:
    void s_startComand();//для визуализации
    void s_started();    //для визуализации
    void s_stopComand(); //для визуализации
    void s_stoped();     //для визуализации
    void s_noDef();      //для визуализации
    void s_maxTargTempChd(QVariant);
    void s_targetTempChd(QVariant);
    void s_alarmTempChd(QVariant);

public slots:
    void stop();
    void start();
    void setTargetTemp(QVariant TargetTemp);
    void setAlarmTemp(QVariant TargetTemp);

protected slots:
     void _customConnectToGUI(QObject * guiItem,  QObject * propWin) override;
     void _updateStateAndMode() override;
     void _doOnModeChange() override{};
     void _hiLimTargetTempCorrect( QVariant AlarmTemp );

     // Unit interface
protected:
     void _setCurrentState(Prom::UnitStates currentState) override ;
};


#endif // BURNERVSM_KROMY_H
