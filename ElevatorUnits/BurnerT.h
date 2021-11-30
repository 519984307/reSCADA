#ifndef BURNERT_H
#define BURNERT_H

#ifndef UNIT_H
#include "unit.h"
#endif

#include <QObject>

class InDiscretETag;
class InETag;
class OutETag;

class BurnerT : public Unit
{
    Q_OBJECT
public:
    explicit BurnerT(int ID,
                    QString Name,
                    QString TagPefix,
                    Prom::UnitModes SaveMode = Prom::UnMdStop );

protected:
    InDiscretETag * _BRW = nullptr;
    InDiscretETag * _BRA = nullptr;
    InDiscretETag * _QK = nullptr;
    InETag * _T = nullptr;
    OutETag * _setT = nullptr;

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


#endif // BURNER_H
