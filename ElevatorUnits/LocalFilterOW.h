#ifndef LOCALFILTEROW_H
#define LOCALFILTEROW_H

#ifndef ONEENGUNIT_H
#include "OneEngRouteUnit.h"
#endif

class OutDiscretETag;

class LocalFilterOW : public OneEngRouteUnit
{
    Q_OBJECT

public:
    explicit LocalFilterOW( int ID,
                            QString Name,
                            QString TagPefix,
                            std::vector<QString> EngSensorNames);

protected:
    OutDiscretETag * _FI = nullptr;

};

#endif // TRANSPORTERVSM_H
