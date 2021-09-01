#ifndef NORIAVSM1_H
#define NORIAVSM1_H

#include "OneEngRouteUnit.h"
#include "Noria.h"

class InDiscretETag;
class InETag;

class NoriaRDCS1CRT1 : public Noria
{
    Q_OBJECT

public:
    explicit NoriaRDCS1CRT1( int ID,
                        QString Name,
                        QString TagPefix,
                        std::vector<QString> EngSensorNames,
                        bool RDCS_NOnotNC );

protected:
    //InDiscretETag * _PS = nullptr;
    InDiscretETag * _RDCS = nullptr;
    //InDiscretETag * _TE = nullptr;
    InETag * _CRT = nullptr;
//    void _AlarmDo() override;

};

#endif // NORIAVSM1_H
