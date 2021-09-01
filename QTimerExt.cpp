#include <qsettings.h>
#include <qdebug.h>
#include "QTimerExt.h"

extern QSettings * g_unitINI;

//------------------------------------------------------------------------------
QTimerExt::QTimerExt(QObject *parent):QTimer(parent)
{

}

//------------------------------------------------------------------------------
void QTimerExt::start()
{
    QTimer::start();
    emit StartSig(interval() / 1000);
}

//------------------------------------------------------------------------------
void QTimerExt::start(int msec)
{
    QTimer::start(msec);
    emit StartSig(interval() / 1000);
}

//------------------------------------------------------------------------------
void QTimerExt::stop()
{
    QTimer::stop();
    emit StopSig();
}

//------------------------------------------------------------------------------
