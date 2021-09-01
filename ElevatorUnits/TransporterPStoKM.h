#ifndef TRANSPORTERPSTOKM_H
#define TRANSPORTERPSTOKM_H

#ifndef ONEENGUNIT_H
#include "OneEngRouteUnit.h"
#endif

class InDiscretETag;

class TransporterPStoKM : public OneEngRouteUnit
{
    Q_OBJECT

public:
    explicit TransporterPStoKM( int ID,
                              QString Name,
                              QString TagPefix,
                              std::vector<QString> EngSensorNames,
                              bool PS_NOnotNC );

protected:
    InDiscretETag * _PS = nullptr;
};

#endif // TRANSPORTERPSTOKM_H
