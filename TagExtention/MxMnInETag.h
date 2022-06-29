#ifndef MxMnInETag_H
#define MxMnInETag_H

#include <QObject>
#include "ETag.h"
//#include "../../SCADAenums.h"
//#include <QVariant>

class MxMnInETag: public ETag
{
    Q_OBJECT
public:
    explicit MxMnInETag(Unit * Owner,
                    Prom::ESTagType Type,
                    QString Name,
                    QString DBName,
                    QVariant MaxLevel,
                    QVariant MinLevel,
                    QVariant ChageStep = 0,
                    bool InOrOutDetect = false,
                    bool TunableSetTime = true,
                    bool TunablePulseTime = false,
                    bool TunabDetectLevel = false,
                    bool EgnorableAlarm = true,
                    bool InGUI = true,
                    Prom::ETagValConv Convertion = Prom::VCNo,
                    int TimeMax = 0);

    const bool tunabDetectLevel;
    bool isDetected (bool * Ok = nullptr) const { if(Ok)*Ok = _ok; return _detect; }
    bool isMaxDetected (bool * Ok = nullptr) const { if(Ok)*Ok = _ok; return _maxDetect; }
    bool isMinDetected (bool * Ok = nullptr) const { if(Ok)*Ok = _ok; return _minDetect; }

    bool isOk() const { return _ok; }
    void reInitialise() override;

    QVariant maxLevel() const;
    QVariant minLevel() const;
    void _customConnectToGUI(QObject *guiItem, QObject *engRow) override;
    void setTimeMax( int );
    int timeMax();

protected:
    bool _onlyChange = true;
    bool _detect = false;
    bool _inOrOutDetect = true;//Определяет в каком диапазоне тег детектирован,
    //true - детектирован между мин. и макс., выше макс. и ниже ммин. не
    //детектирован. false - наоборот.
    bool _maxDetect = false;
    bool _minDetect = false;
    QVariant _maxLevel = 0;
    QVariant _minLevel = 0;
    double _correction = 0;
    bool _alarmOn = false;
    bool _DnotU = false; //Определяет нормальное (не аварийное) состояние тега
    // true - в нормальном состоянии он дедектирован, false - не детектирован
    bool _detectPulse = false;
    bool _trig = true;
    bool _preDetect = true;
//    bool _timeMaxValue = false;
//    unsigned int _timeMaxInterval_ms = 1000;
    QVariant _timeMaxValue{0};
    QVariant _timeMinValue{0};
    QVariant _timeLastValue{0};
    QTimerExt * _timeMax{ nullptr };

    void needBe(bool DtctOrNot, bool AlarmOn, bool SetTimer = true);
    virtual bool _checkDetect ();

signals:
    void s_detected();
    void s_maxDetected();
    void s_minDetected();
    void s_undetected();
    void s_maxLevelChanged(QVariant setVal);
    void s_minLevelChanged(QVariant setVal);

public slots:
    void writeImit(bool setImit)  override;
    void writeImitVal(QVariant setVal)  override;
    void setMaxLevel(QVariant MaxVal);
    void setMinLevel(QVariant MinVal);

    void needBeDetectedAlarm(){ needBe(true, true); }
    void needBeDetectedNoAlarm(){ needBe(true, false); }
    void needBeUndetectedAlarm(){ needBe(false, true); }
    void needBeUndetectedNoAlarm(){ needBe(false, false); }

    void needBeDetectedAlarmNoTime(){ needBe(true, true, false); }
    void needBeDetectedNoAlarmNoTime(){ needBe(true, false, false); }
    void needBeUndetectedAlarmNoTime(){ needBe(false, true, false); }
    void needBeUndetectedNoAlarmNoTime(){ needBe(false, false, false); }

    void onlyChange();
    virtual void pulsSensor(bool);

    void saveParam() override;
    void loadParam() override;

protected slots:
    void _setTimerEnd() override;
    void _qualityChangedSlot() override;

    void pulseTimerEnd() override;
    void _checkVal() override;
    void _checkPulse() override;
    void _timeMaxStep();
};

#endif // MXMNINLESTAG_H
