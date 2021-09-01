#ifndef ONEENGROUTEFCUNIT_H
#define ONEENGROUTEFCUNIT_H

#include "OneEngRouteUnit.h"
class InETag;
class OutETag;
class InDiscretETag;

class OneEngRouteFCUnit : public OneEngRouteUnit
{
    Q_OBJECT
public:
    OneEngRouteFCUnit(Prom::UnitType Type,
                       int ID,
                       QString Name,
                       QString TagPefix,
                       bool Mover,
                       std::vector<QString> EngSensorNames);
protected:
    InETag        * _OutFR   = nullptr;
    InETag        * _SetedFR = nullptr;
    OutETag       * _SetFR   = nullptr;

signals:
    void SetSpeed (QVariant);

public slots:
    void GetSpeed (QVariant Value);

protected slots:
    void _customConnectToGUI(QObject *guiItem,  QObject *propWin) override;

};

#endif // ONEENGROUTEFCLUNIT_H
