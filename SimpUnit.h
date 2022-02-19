#ifndef SIMPUNIT_H
#define SIMPUNIT_H

#include "SCADAenums.h"

#include "unit.h"
#ifndef UNIT_H
#endif

class SimpUnit : public Unit
{
    Q_OBJECT

public:

    explicit SimpUnit(Prom::UnitType Type,
                   int *Id,
                   QString Name,
                   QString TagPrefix,
                   bool Mover = false,//TODO Убрать в отдельный параметрдочерних классов!
                   Prom::UnitModes SaveMode = Prom::UnMdNoDef,
                   QSettings * Ini = nullptr);

    // Unit interface
protected:
    Prom::SetModeResp _customSetMode(Prom::UnitModes *mode, bool UserOrSys)override{};
    void _doOnModeChange()override{};

protected slots:
    void _customConnectToGUI(QObject *guiItem, QObject *propWin)override{};
    void _updateStateAndMode()override{};
};
#endif
