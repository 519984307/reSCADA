#ifndef DRUER2BUNKER_H
#define DRUER2BUNKER_H

#include "RouteUnit.h"

class InDiscretETag;
class OutDiscretETag;
class InETag;
class OutETag;

class Dryer2Bunker : public RouteUnit
{
    Q_OBJECT
public:
    explicit Dryer2Bunker(  int ID,
                             QString Name,
                             QString TagPefix );

    OutDiscretETag * _needLoad { nullptr };

protected:

    bool _lvlAlarm { false };

    InDiscretETag * _HL1 { nullptr };
    InDiscretETag * _ML1 { nullptr };
    InDiscretETag * _LL1 { nullptr };

    InDiscretETag * _HL2 { nullptr };
    InDiscretETag * _ML2 { nullptr };
    InDiscretETag * _LL2 { nullptr };

    Prom::SetModeResp _customSetMode(Prom::UnitModes *mode, bool UserOrSys) override;
    Prom::UnitModes _ModeOfCommand( Prom::RouteCommand *Command ) override;

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

    //void SendDispName( QString dispName );

public slots:
    void AddToCurrentRoteNeedUnload() { _AddToCurrentRoute( Prom::UnMdNeedUnload ); }
    void AddToCurrentRoteNeedLoad()   { _AddToCurrentRoute( Prom::UnMdNeedLoad ); }


    void _updateStateAndMode() override;
protected slots:
    void _customConnectToGUI( QObject * guiItem,  QObject * propWin ) override;

    // Unit interface
    bool _resetAlarm(bool upClassAlarm) override;
};

#endif // DRUER2BUNKER_H
