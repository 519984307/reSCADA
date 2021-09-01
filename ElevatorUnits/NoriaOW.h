#ifndef NORIAOW_H
#define NORIAOW_H

#include "Noria.h"

class InDiscretETag;

class NoriaOW : public Noria
{
    Q_OBJECT

public:
    explicit NoriaOW( int ID,
                      QString Name,
                      QString TagPefix  );

protected:

    InDiscretETag * _TE1 = nullptr;
    InDiscretETag * _TE2 = nullptr;
    InDiscretETag * _TE3 = nullptr;
    InDiscretETag * _TE4 = nullptr;
    InDiscretETag * _SS = nullptr;
    InDiscretETag * _PS = nullptr;

signals:


protected slots:

public slots:

};

#endif // NORIAOW_H
