#ifndef DRUERFAN_DOBRNRST_H
#define DRUERFAN_DOBRNRST_H
#include "DryerFan_DobrNr.h"

class InDiscretETag;

class DryerFan_DobrNrST : public DryerFan_DobrNr
{
    Q_OBJECT
public:
    DryerFan_DobrNrST( int ID,
                       QString Name,
                       QString TagPefix,
                       std::vector<QString> EngSensorNames );
protected:
    OutETag * _setT = nullptr;

protected slots:
    void _customConnectToGUI( QObject *guiItem,  QObject *propWin );

};

#endif // DRUERFAN_DOBRNRST_H
