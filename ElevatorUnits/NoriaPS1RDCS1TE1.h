#ifndef NORIAVSM_PS1RDCS1TE1_H
#define NORIAVSM_PS1RDCS1TE1_H

#include "OneEngRouteUnit.h"
#include "Noria.h"

class InDiscretETag;
class InETag;

class NoriaVSM_PS1RDCS1TE1 : public Noria
{
    Q_OBJECT

public:
    explicit NoriaVSM_PS1RDCS1TE1( int ID,
                        QString Name,
                        QString TagPefix,
                        std::vector<QString> EngSensorNames,
                        bool PS_NOnotNC,
                        bool RDCS_NOnotNC,
                        bool TE_NOnotNC
                        );

protected:
    InDiscretETag * _PS = nullptr;
    InDiscretETag * _RDCS = nullptr;
    InDiscretETag * _TE = nullptr;
    InETag * _CRT = nullptr;
    //void _AlarmDo() override;

};

#endif // NORIAVSM_PS1RDCS1TE1_H
