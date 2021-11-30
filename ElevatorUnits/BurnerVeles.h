#ifndef BURNERVELES_H
#define BURNERVELES_H

#ifndef UNIT_H
#include "unit.h"
#endif

#include <QObject>

class InDiscretETag;
class InETag;
class OutETag;

class BurnerVeles : public Unit
{
    Q_OBJECT
public:
    explicit BurnerVeles(int ID,
                    QString Name,
                    QString TagPefix );

protected:
    InDiscretETag * _BRW = nullptr;
    InDiscretETag * _BRA = nullptr;
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


#endif // BURNERVELES_H
