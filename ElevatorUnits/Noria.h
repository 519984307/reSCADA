#ifndef NORIA_H
#define NORIA_H

#include "OneEngRouteUnit.h"

class Noria : public OneEngRouteUnit
{
    Q_OBJECT

public:
    explicit Noria( int ID,
                    QString Name,
                    QString TagPefix,
                    std::vector<QString> EngSensorNames );

protected:

    Prom::UnitModes _ModeOfCommand( Prom::RouteCommand *Command ) override;
    void _alarmDo() override;
protected slots:
     void _customConnectToGUI( QObject * guiItem,  QObject * propWin ) override;

};

#endif // NORIAVSM1_H
