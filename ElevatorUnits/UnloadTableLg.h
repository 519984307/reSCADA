#ifndef UNLOADTABLELG_H
#define UNLOADTABLELG_H

#include "OneEngRouteFClUnit.h"
class InETag;
class OutETag;
class OutDiscretETag;
class InDiscretETag;

class UnloadTableLg : public OneEngRouteFClUnit
{
    Q_OBJECT
public:
    UnloadTableLg( int ID,
                       QString Name,
                       QString TagPefix,
                       bool Mover,
                       std::vector<QString> EngSensorNames );
protected:
    OutDiscretETag * _forseSop   = nullptr;
    InDiscretETag * _pos = nullptr;
signals:
    //void SetSpeed ( QVariant );

public slots:
    void GetSpeed ( QVariant Value );

protected slots:
    void _customConnectToGUI( QObject * guiItem,  QObject * propWin ) override;

    // Unit interface
protected:
    Prom::SetModeResp _customSetMode(Prom::UnitModes *Mode, bool UserOrSys) override;
};

#endif // UNLOADTABLE_KROMY_H
