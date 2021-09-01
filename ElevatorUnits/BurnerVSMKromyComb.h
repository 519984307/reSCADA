#ifndef BURNERVSMKROMYCOMB_H
#define BURNERVSMKROMYCOMB_H

#include "RouteUnit.h"
#include "BurnerVSM_Kromy.h"

#include <QObject>

class InDiscretETag;
class InETag;
class OutETag;
class OutDiscretETag;

class BurnerVSMKromyComb : public BurnerVSM_Kromy
{
    Q_OBJECT
public:
    explicit BurnerVSMKromyComb(int ID,
                                 QString Name,
                                 QString TagPefix);

void setCombustor(Unit * Combustor);


protected:
    Prom::SetModeResp _customSetMode(Prom::UnitModes *mode, bool UserOrSys) override;
    Unit * _combustor = nullptr;
    QTimerExt * _combStartDelay = nullptr;
    QTimerExt * _combStopDelay = nullptr;

signals:
    void SetStartCombDelay(QVariant);
    void SetStopCombDelay(QVariant);

public slots:
    void stop() { setMode(Prom::UnMdColling, true); }

protected slots:
    void _updateStateAndMode() override;
    void _doOnModeChange() override{};
    void setStartCombDelay(QVariant);
    void setStopCombDelay(QVariant);
    void combStartDelayEnd();
    void combStopDelayEnd();

    // Unit interface
public:
    bool addSubUnit(Unit *unit) override;

protected slots:
    void _updateSubUnitState(Unit * unit) override;
    void _updateSubUnitMode(Unit * unit) override;

    // Unit interface
protected slots:
    void _customConnectToGUI(QObject *guiItem, QObject *propWin) override;

    // Unit interface
public slots:
    void saveParam() override;
    void loadParam() override;

    // Unit interface
protected:
    void _alarmDo() override;

    // Unit interface
public slots:
    void reInitialise() override;
};


#endif // BURNERVSMKROMYCOMB_H
