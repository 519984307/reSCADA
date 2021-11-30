#ifndef TRANSPORTERFC_PSTOKM_H
#define TRANSPORTERFC_PSTOKM_H

#include "OneEngRouteFCUnit.h"


class InDiscretETag;

class TransporterFC_PStoKM : public OneEngRouteFCUnit
{
    Q_OBJECT

public:
    explicit TransporterFC_PStoKM( int ID,
                                 QString Name,
                                 QString TagPefix,
                                 std::vector<QString> EngSensorNames,
                                 bool PS_NOnotNC );

protected:
    InDiscretETag * _PS = nullptr;
};

#endif // TRANSPORTERFC_PSTOKM_H
