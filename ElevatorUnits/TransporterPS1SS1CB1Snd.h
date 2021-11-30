#ifndef TRANSPORTERPS1SS1CB1SND_H
#define TRANSPORTERPS1SS1CB1SND_H

#ifndef ONEENGUNIT_H
#include "OneEngRouteUnit.h"
#endif

class InDiscretETag;
class OutDiscretETag;

class TransporterPS1SS1CB1Snd : public OneEngRouteUnit
{
    Q_OBJECT

public:
    explicit TransporterPS1SS1CB1Snd( int ID,
                              QString Name,
                              QString TagPefix,
                              std::vector<QString> EngSensorNames,
                              bool PS_NOnotNC,
                              bool CB_NOnotNC );

protected:
    InDiscretETag * _PS = nullptr;
    InDiscretETag * _CB = nullptr;
    InDiscretETag * _SS = nullptr;
    OutDiscretETag * _sound = nullptr;
};

#endif // TRANSPORTERPS1SS1CB1SND_H
