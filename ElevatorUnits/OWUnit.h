#ifndef OWUNIT_H
#define OWUNIT_H

#ifndef ONEENGROUTEUNIT_H
#include "OneEngRouteUnit.h"
#endif

class InDiscretETag;

class OWUnit : public OneEngRouteUnit
{
    Q_OBJECT

public:
    explicit OWUnit( Prom::UnitType Type,
                     int ID,
                     QString Name,
                     QString TagPefix,
                      std::vector<QString> EngSensorNames,
                     bool PS_NOnotNC );

protected:
    InDiscretETag * _SS = nullptr;
    InDiscretETag * _PS = nullptr;

signals:


protected slots:

public slots:

};

#endif // OWUNIT_H
