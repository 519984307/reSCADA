#ifndef NORIAVSM_H
#define NORIAVSM_H

#include "Noria.h"

class InDiscretETag;

class NoriaVSM : public Noria
{
    Q_OBJECT

public:
    explicit NoriaVSM( int ID,
                       QString Name,
                       QString TagPefix  );

protected:
    InDiscretETag * _PS = nullptr;
    InDiscretETag * _RDCS = nullptr;
    InDiscretETag * _TE1 = nullptr;
    InDiscretETag * _TE2 = nullptr;

};

#endif // NORIAVSM_H
