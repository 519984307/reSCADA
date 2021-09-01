#ifndef DRUERFAN_DOBRNR_H
#define DRUERFAN_DOBRNR_H
#include "OneEngRouteFCUnit.h"

class InDiscretETag;

class DryerFan_DobrNr : public OneEngRouteFCUnit
{
    Q_OBJECT
public:
    DryerFan_DobrNr( int ID,
                     QString Name,
                     QString TagPefix,
                     std::vector<QString> EngSensorNames );
protected:
    InDiscretETag * _OnFC    = nullptr;
    InETag * _T  = nullptr;

protected slots:
    void _customConnectToGUI( QObject * guiItem, QObject * propWin ) override;

};

#endif // DRUERFAN_DOBRNR_H
