#ifndef DRUER1BUNKER_H
#define DRUER1BUNKER_H

#include "RouteUnit.h"

class InDiscretETag;
class OutDiscretETag;
class InETag;
class OutETag;
class Static2posUnit;

class Druer1Bunker : public RouteUnit
{
    Q_OBJECT
public:
    explicit Druer1Bunker(  int ID,
                               QString Name,
                               QString TagPefix );

protected:

    InDiscretETag * _HL = nullptr;
    InDiscretETag * _ML = nullptr;
    InDiscretETag * _LL = nullptr;
    Prom::SetModeResp _customSetMode(Prom::UnitModes *mode, bool UserOrSys) override;
    Prom::UnitModes _ModeOfCommand( Prom::RouteCommand *Command ) override;

signals:
    void   ActivateHL();
    void DeactivateHL();
    void   ActivateML();
    void DeactivateML();
    void   ActivateLL();
    void DeactivateLL();

    //void SendDispName( QString dispName );

public slots:

    void AddToCurrentRoteNeedUnload() { _addToCurrentRoute( Prom::UnMdNeedUnload ); }
    void AddToCurrentRoteNeedLoad()   { _addToCurrentRoute( Prom::UnMdNeedLoad ); }
    void _updateStateAndMode() override;

protected slots:
    void _customConnectToGUI( QObject * guiItem,  QObject * propWin ) override;

};

#endif // DRUER1BUNKER_H
