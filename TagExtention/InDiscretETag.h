#ifndef INDISCRETESTAG_H
#define INDISCRETESTAG_H

#include <QObject>
#include "InETag.h"
//#include "../../SCADAenums.h"

class InDiscretETag: public InETag
{
    Q_OBJECT
public:
    InDiscretETag(Unit * Owner,
                   QString Name,
                   QString DBName,
                   QVariant Hi = true,
                   QVariant Low = false,
                   bool NOnotNC = true,
                   bool TunableSetTime = true,
                   bool TunablePulseTime = false,
                   bool EgnorableAlarm = true,
                   bool InGUI = true,
                   Prom::ETagValConv Convertion = Prom::VCNo,
                   int TimeMax = 0);

    void _customConnectToGUI(QObject *guiItem, QObject *engRow) override;

protected:
    bool _checkDetect() override;
    /*! Верхнее значение сигнала. Соответствует включению. */
    QVariant _hi;
    /*! Нижнее значение сигнала. Соответствует выключению. */
    QVariant _low;

public slots:
    void setNOnotNC(bool type) { _highOrLow = type; }

protected slots:

};

#endif // INDISCRETESTAG_H
