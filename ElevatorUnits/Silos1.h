#ifndef SILOS1_H
#define SILOS1_H

#ifndef UNIT_H
#include "RouteUnit.h"
#endif

#include <QObject>

class InDiscretETag;

class Silos1 : public RouteUnit
{
    Q_OBJECT
public:
    explicit Silos1( int ID,
                     QString Name,
                     QString TagPefix,
                     bool HLNOnotNC = false,
                     Prom::UnitModes SaveMode = Prom::UnMdEmpty );

protected:
    InDiscretETag * _HL = nullptr;
    Prom::SetModeResp _customSetMode( Prom::UnitModes *mode, bool UserOrSys ) override;
    void _updateStateAndMode() override;
    Prom::UnitModes _ModeOfCommand( Prom::RouteCommand *Command ) override;

signals:
    void ActivateHL();
    void DeactivateHL();

public slots:
    void AddToCurrentRoteFull()  { _AddToCurrentRoute( Prom::UnMdFull ); }
    void AddToCurrentRoteEmpty() { _AddToCurrentRoute( Prom::UnMdEmpty ); }

protected slots:
    void _customConnectToGUI( QObject * guiItem, QObject * propWin ) override;

};

#endif // SILOS1_H
