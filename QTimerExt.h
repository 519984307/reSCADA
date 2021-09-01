#ifndef QTIMEREXT_H
#define QTIMEREXT_H
#include <QVariant>
#include <QObject>
#include <QDateTime>

#ifndef SCADAENUMS_H
#include "../../SCADAenums.h"
#endif // SCADAENUMS_H
#include <QTimer>

class QTimerExt: public QTimer
{
    Q_OBJECT
public:
    explicit QTimerExt(QObject *parent = nullptr);
//    void singleShot(int msec, const QObject *receiver, const char *member);
//    void singleShot(int msec, Qt::TimerType timerType, const QObject *receiver, const char *member);

signals:
    void StartSig(QVariant);
    void StopSig();

public slots:
    void start();
    void start(int msec);
    void stop();
};


#endif //QTIMEREXT_H
