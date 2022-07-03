#include "InDiscretETag.h"
#include <QDebug>
#include "ETag.h"

using Prom::MessType;

InDiscretETag::InDiscretETag(Unit * Owner,
                             QString Name,
                             QString DBName,
                             QVariant Hi,
                             QVariant Low,
                             bool NOnotNC,
                             bool TunableSetTime,
                             bool TunablePulseTime,
                             bool EgnorableAlarm,
                             bool InGUI,
                             Prom::ETagValConv Convertion,
                             int TimeMax )
    : InETag(Owner,
              Prom::TpDiscretIn,
              Name,
              DBName,
              NOnotNC,
              0,
              0,
              TunableSetTime,
              TunablePulseTime,
              false,
              EgnorableAlarm,
              InGUI,
              Convertion,
              TimeMax ),
      _hi(Hi), _low(Low)

{
    _detect = _checkDetect();
    _preDetect = _detect;
}

//------------------------------------------------------------------------------
bool InDiscretETag::_checkDetect()
{
    return _highOrLow ? (value().toDouble() >= _hi.toDouble())
                      : (value().toDouble() <= _low.toDouble());
}

//------------------------------------------------------------------------------
void InDiscretETag::_customConnectToGUI(QObject *, QObject *engRow)
{
    QVariant ret;
    QObject * tmpSgSt;
    //InETag::_customConnectToGUI(guiItem, engRow);
    //!создал в строке главный раздел (с кубиками)
    QMetaObject::invokeMethod(engRow, "addPropertySignal", Qt::DirectConnection,
                               Q_RETURN_ARG(QVariant, ret),
                               Q_ARG(QVariant, this->fullTagName() + "_PrSig"),
                               Q_ARG(QVariant, this->getName()));
    //tmpSgSt = propWin->findChild<QObject*>(est->fullTagName() + "_PrSig");
    tmpSgSt = qvariant_cast< QObject* >(ret);
    //получил указатель на главный раздел
    //-----подключил сигналы к кубикам
    connect(tmpSgSt, SIGNAL(s_imChanged(     bool    )), this,      SLOT(writeImit(      bool    )), Qt::QueuedConnection);
    connect(tmpSgSt, SIGNAL(s_imValChanged(  QVariant)), this,      SLOT(writeImitVal(   QVariant)), Qt::QueuedConnection);
    connect(this,      SIGNAL(s_imitationChd(      QVariant)), tmpSgSt, SLOT(setIm(       QVariant)), Qt::QueuedConnection);
    connect(this,      SIGNAL(s_imitationValueChd(   QVariant)), tmpSgSt, SLOT(setImVal(    QVariant)), Qt::QueuedConnection);
    connect(this,      SIGNAL(s_liveValueChd(QVariant)), tmpSgSt, SLOT(setVal(      QVariant)), Qt::QueuedConnection);
    connect(this,      SIGNAL(s_qualityChd(QVariant)), tmpSgSt, SLOT(setConnected(QVariant)), Qt::QueuedConnection);
    //-----подключил сигналы к кубикам
}

//------------------------------------------------------------------------------
