#ifndef TRANSPORTERPS1CB1_H
#define TRANSPORTERPS1CB1_H

#ifndef ONEENGUNIT_H
#include "OneEngRouteUnit.h"
#endif

class InDiscretETag;

class TransporterPS1CB1 : public OneEngRouteUnit
{
    Q_OBJECT

public:
    explicit TransporterPS1CB1( int ID,
                              QString Name,
                              QString TagPefix,
                              std::vector<QString> EngSensorNames,
                              bool PS_NOnotNC,
                              bool CB_NOnotNC );

protected:
    InDiscretETag * _PS = nullptr;
    InDiscretETag * _CB = nullptr;

};

#endif // TRANSPORTERPS1CB1_H
