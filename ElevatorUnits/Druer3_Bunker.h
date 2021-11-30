#ifndef DRUER3_BUNKER_H
#define DRUER3_BUNKER_H

#include "RouteUnit.h"

class InDiscretETag;
class OutDiscretETag;
class InETag;
class OutETag;

class Druer3_Bunker : public RouteUnit
{
    Q_OBJECT
public:
    explicit Druer3_Bunker( int ID,
                             QString Name,
                             QString TagPefix);

    OutDiscretETag * _needLoad { nullptr };

protected:

    bool _lvlAlarm { false };

    InDiscretETag * _HL1 { nullptr };
    InDiscretETag * _ML1 { nullptr };
    InDiscretETag * _LL1 { nullptr };

    InDiscretETag * _HL2 { nullptr };
    InDiscretETag * _ML2 { nullptr };
    InDiscretETag * _LL2 { nullptr };

    InDiscretETag * _HL3 { nullptr };
    InDiscretETag * _ML3 { nullptr };
    InDiscretETag * _LL3 { nullptr };
    Prom::SetModeResp _customSetMode(Prom::UnitModes *mode, bool UserOrSys) override;
    Prom::UnitModes _ModeOfCommand(Prom::RouteCommand *Command) override;

signals:
    void   ActivateHL1();
    void DeactivateHL1();
    void   ActivateML1();
    void DeactivateML1();
    void   ActivateLL1();
    void DeactivateLL1();

    void   ActivateHL2();
    void DeactivateHL2();
    void   ActivateML2();
    void DeactivateML2();
    void   ActivateLL2();
    void DeactivateLL2();

    void   ActivateHL3();
    void DeactivateHL3();
    void   ActivateML3();
    void DeactivateML3();
    void   ActivateLL3();
    void DeactivateLL3();
    //void SendDispName(QString dispName);
    void s_belowLL();

public slots:
    void AddToCurrentRoteNeedUnload() { _AddToCurrentRoute(Prom::UnMdNeedUnload); }
    void AddToCurrentRoteNeedLoad()   { _AddToCurrentRoute(Prom::UnMdNeedLoad); }


    void _updateStateAndMode() override;
protected slots:
    void _customConnectToGUI(QObject * guiItem,  QObject * propWin) override;

    // Unit interface
    bool _resetAlarm(bool upClassAlarm) override;
};

#endif // DRUER3_BUNKER_H
