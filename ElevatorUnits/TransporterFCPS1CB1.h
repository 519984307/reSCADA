#ifndef TRANSPORTERFCPS1CB1_H
#define TRANSPORTERFCPS1CB1_H

#include "OneEngRouteFCUnit.h"


class InDiscretETag;

class TransporterFCPS1CB1 : public OneEngRouteFCUnit
{
    Q_OBJECT

public:
    explicit TransporterFCPS1CB1( int ID,
                                 QString Name,
                                 QString TagPefix,
                                 std::vector<QString> EngSensorNames,
                                 bool PS_NOnotNC,
                                 bool CB_NOnotNC );

protected:
    InDiscretETag * _PS = nullptr;
    InDiscretETag * _CB = nullptr;

};

#endif // TRANSPORTERFC_VSM1_H
