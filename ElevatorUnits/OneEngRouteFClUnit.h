#ifndef ONEENGROUTEFCUNIT_H
#define ONEENGROUTEFCUNIT_H

#include "OneEngRouteUnit.h"
class InETag;
class OutETag;
class InDiscretETag;

class OneEngRouteFClUnit : public OneEngRouteUnit
{
    Q_OBJECT
public:
    OneEngRouteFClUnit( Prom::UnitType Type,
                        int ID,
                        QString Name,
                        QString TagPefix,
                        bool Mover,
                        std::vector<QString> EngSensorNames );
protected:
    InETag  * _OutSp = nullptr;
    OutETag * _SetSp = nullptr;

signals:
    void SetSpeed ( QVariant );

public slots:
    void GetSpeed ( QVariant Value );

protected slots:
    void _customConnectToGUI( QObject *guiItem,  QObject *propWin ) override;

};

#endif // ONEENGROUTEFCUNIT_H
