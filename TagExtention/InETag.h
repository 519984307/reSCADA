#ifndef InETag_H
#define InETag_H

#include <QObject>
#include "ETag.h"
//#include "../../SCADAenums.h"
//#include <QVariant>

class InETag: public ETag
{
    Q_OBJECT

public:
    explicit InETag(Unit * Owner,
                    Prom::ESTagType Type,
                    QString Name,
                    QString DBName,
                    bool HighOrLow,
                    QVariant DetectLevel,
                    QVariant ChageStep = 0,
                    bool TunableSetTime = true,
                    bool TunablePulseTime = false,
                    bool TunabDetectLevel = false,
                    bool EgnorableAlarm = true,
                    bool InGUI = true,
                    Prom::ETagValConv Convertion = Prom::VCNo,
                    int TimeMax = 0);

    const bool tunabDetectLevel;
    bool isDetected (bool * Ok = nullptr) const { if(Ok)*Ok = _ok; return _detect; }
    bool isOk() const { return _ok; }
    //bool isImit() const { return _imit; }
    void reInitialise() override;

    QVariant detectLevel() const;
    bool highOrLow(){ return _highOrLow; };
    void _customConnectToGUI(QObject *guiItem, QObject *engRow) override;
    void setTimeMax( int );
    int timeMax();

protected:
    bool _onlyChange = true;
    bool _detect = false;
    bool _highOrLow;
    QVariant _detectLevel = 0;
    double _correction = 0;
    bool _alarmOn = false;
    bool _DnotU = false;
    bool _detectPulse = false;
    bool _trig = true;
    bool _preDetect = true;
//    bool _timeMaxValue = false;
//    unsigned int _timeMaxInterval_ms = 1000;
    QVariant _timeMaxValue{0};
    QVariant _timeLastValue{0};
    QTimerExt * _timeMax{ nullptr };

    void needBe(bool DtctOrNot, bool AlarmOn, bool SetTimer = true);
    virtual bool _checkDetect();

signals:
    void s_detected();
    void s_undetected();
    void s_delectLevelChanged(QVariant setVal);

    void highOrLowChanged();

public slots:
    void writeImit(bool setImit)  override;
    void writeImitVal(QVariant setVal)  override;
    void setDetectLevel(QVariant setVal);

    void needBeDetectedAlarm(){ needBe(true, true); }
    void needBeDetectedNoAlarm(){ needBe(true, false); }
    void needBeUndetectedAlarm(){ needBe(false, true); }
    void needBeUndetectedNoAlarm(){ needBe(false, false); }

    void needBeDetectedAlarmNoTime(){ needBe(true, true, false); }
    void needBeDetectedNoAlarmNoTime(){ needBe(true, false, false); }
    void needBeUndetectedAlarmNoTime(){ needBe(false, true, false); }
    void needBeUndetectedNoAlarmNoTime(){ needBe(false, false, false); }

    void onlyChange();
    virtual void pulsSensor(bool On);
    void pulsSensorON(){ pulsSensor(true); };
    void pulsSensorOFF(){ pulsSensor(false); };

    void _setTimerEnd()  override;
    void saveParam() override;
    void loadParam() override;

protected slots:
    void _qualityChangedSlot() override;

    void pulseTimerEnd() override;
    void _checkVal() override;
    void _checkPulse() override;
    void _timeMaxStep();

};

#endif // INLESTAG_H
