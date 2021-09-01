#ifndef UNDERGATE_H
#define UNDERGATE_H

#include "static2posunit.h"
#include <QObject>

class UnderGate: public Static2posUnit
{
    Q_OBJECT

public:
    UnderGate( int ID,
               QString Name,
               QString TagPefix,
               std::vector<QString> EngSensorNames,
               bool NOnotNCSensors );

protected:

    Prom::UnitModes _ModeOfCommand( Prom::RouteCommand *Command ) override;
};

#endif // UNDERGATE_H
