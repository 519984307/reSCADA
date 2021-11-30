#ifndef UNLOADTABLE_KROMY_H
#define UNLOADTABLE_KROMY_H

#include "OneEngRouteUnit.h"
class InETag;
class OutETag;
class OutDiscretETag;
class InDiscretETag;

class UnloadTable_Kromy : public OneEngRouteUnit
{
    Q_OBJECT
public:
    UnloadTable_Kromy(int ID,
                       QString Name,
                       QString TagPefix,
                       bool Mover,
                       std::vector<QString> EngSensorNames);

protected:
    bool _mayStart { false };
    bool _waitForMayStart { false };
    OutETag * _SetFR   { nullptr };
    OutDiscretETag * _forseSop   { nullptr };
    InDiscretETag * _pos { nullptr };
    Prom::SetModeResp _customSetMode(Prom::UnitModes *Mode, bool UserOrSys) override;
    void _updateStateAndMode() override;

signals:
    void WaitForStart(bool);

public slots:
    void setSpeed(QVariant Speed);
    void setMayStart(bool mayStart);
    void mayStart();
    void notMayStart();

protected slots:
    void _customConnectToGUI(QObject * guiItem,  QObject * propWin) override;  // Unit interface

};

#endif // UNLOADTABLE_KROMY_H
