#ifndef NORIAOWSSPSTESND_H
#define NORIAOWSSPSTESND_H

#include "Noria.h"

class InDiscretETag;
class OutDiscretETag;

class NoriaSSPSTESnd : public Noria
{
    Q_OBJECT

public:
    explicit NoriaSSPSTESnd( int ID,
                      QString Name,
                      QString TagPefix,
                      std::vector<QString> EngSensorNames );

protected:

    InDiscretETag * _TE = nullptr;
    InDiscretETag * _SS = nullptr;
    InDiscretETag * _PS = nullptr;
    OutDiscretETag * _sound = nullptr;

signals:


protected slots:

public slots:

};

#endif // NORIAOWSSPSTESND_H
