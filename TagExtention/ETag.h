#ifndef ESTAG_H
#define ESTAG_H

#include <QObject>
#include <QVariant>

#ifndef SCADAENUMS_H
#include "../../SCADAenums.h"
#endif // SCADAENUMS_H
#include "TSP/tag.h"
#include "QTimerExt.h"

class Unit;
class QTimerExt;

class ETag : public QObject
{
    Q_OBJECT

public:
    explicit ETag(Unit * Owner,
                    Prom::ESTagType Type,
                    QString Name,
                    QString DBName,
                    bool TunableSetTime = true,
                    bool TunablePulseTime = false,
                    bool EgnorableAlarm = true,
                    bool InGUI = true,
                    Prom::ETagValConv Convertion = Prom::VCNo);

    const Prom::ESTagType ttype;

    const bool tunableSetTime;
    const bool tunablePulseTime;
    const bool ignorableAlarm;
    const bool inGUI; //! /=>--

    int getSetTime() const;
    bool connected();
    bool isOk() const{ return _ok; }
    bool isImit() const{ return _imit; }
    QVariant imitValue() const{ return  _imitVal; }
    bool isAlarm() const{ return  _alarm; }
    bool isIgnorAlarm () const{ return _ignorAlarm; }
    virtual void reInitialise();
    //QVariant trueValue(bool *ok){ *ok = _ok; return _ok ? _value: 0; }
    QString getDBName() const { return _DBName; }
    QString getName()   const { return _name; }
    bool isPulseSensor() const { return _pulse; }
    Tag *getTag() const { return _tag; }
    QString fullTagName();
    QVariant value();
    virtual void connectToGUI(QObject *guiItem,  QObject *propWin);

protected:
    QVariant _value = 0;
    QVariant _preValue = 0;
    Unit * _owner;
    Tag * _tag = nullptr;
    QString _name;
    QString _DBName;
    QTimerExt * _setTimer = nullptr;
    QTimerExt * _pulseTimer = nullptr;
    bool _imit = false;
    QVariant _imitVal = 0;
    bool _alarmSetTime = false;
    //bool _ignorAlarmSetTime = false;
    bool _alarm = false;
    bool _ignorAlarm = false;//!
    bool _mayResetAlarm = true;
    bool _ok = false;
    QString _alarmStr = "";
    QString _resetAlarmStr = "";
    void _logging(Prom::MessType MessType,  QString Discription, bool imitation);
    QTimerExt * _testLog;
    bool _pulse = false;
    Prom::ETagValConv _conv = Prom::VCNo;
    unsigned int _pulseAsyncDelay = 0;
    bool _ferstLoad = true;
    virtual void _customConnectToGUI(QObject *,  QObject *){};
    int _pulseDuration{0};

signals:
    void s_qualityChd(QVariant);
    void s_alarm(QVariant Discription);
    void s_alarmReseted();
    void s_logging  (Prom::MessType MessTypeID,  QDateTime DateTime, bool UserOrSys, QString Source, QString Message);

    void s_valueChd(QVariant);
    void s_liveValueChd(QVariant);
    void s_imitationChd(QVariant);
    void s_imitationValueChd(QVariant);
    void s_ignorAlarmChd(QVariant);
    void s_setDelayChd(QVariant);
    void s_pulseDelayChd(QVariant);

protected slots:
    virtual void _checkVal() = 0;
    virtual void _qualityChangedSlot();
    virtual void _acceptValue(QVariant Value);
    virtual void _checkPulse() = 0;

public slots:
    virtual void writeImit(bool setImit) = 0;
    virtual void writeImitVal(QVariant setVal) = 0;
    virtual void saveParam();
    virtual void loadParam();
    virtual void setTimerEnd() = 0;
    virtual void setTimerStart();
    bool resetAlarm();
    void writeSetDelay(QVariant Sec);
    void writeIgnorAlarm(bool ignor);
    void setPulseDuration(QVariant Sec);
    virtual void pulseTimerEnd() = 0;
};

#endif // ESTAG_H
