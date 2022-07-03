#ifndef OUTETAG_H
#define OUTETAG_H

#include "ETag.h"
#include "SCADAenums.h"
class Unit;

class OutETag: public ETag
{
    Q_OBJECT
public:
    OutETag(Unit * Owner,
            Prom::ESTagType Type,
            Prom::OutESTagSetType SetType,
            QString Name,
            QString DBName,
            bool TunableSetTime = false,
            bool TunablePulseTime = false,
            bool EgnorableAlarm = false,
            bool InGUI = true,
            Prom::ETagValConv Convertion = Prom::VCNo,
            bool Save = false,
            bool LoadDefault = false,
            QVariant DefaultValue = 0,
            bool MenuChanged = false,
            quint8   LimitFlag = OUT_LIM_NO,
            QVariant HiLimit = 0,
            QVariant LowLimit = 0,
            bool TunableImpulseTime = false,
            QVariant ChageStep = 0);

    bool saveValue = false;
    bool loadDefalt {true};
    //virtual void setPulse(bool set){};
    QVariant hiLimit() const;
    void setHiLimit(const QVariant &hiLimit);
    QVariant lowLimit() const;
    void setLowLimit(const QVariant &lowLimit);
    void setLimitFlag(const quint8 &LimitFlag);
    int impulseDuration();
    const bool tunableImpulseTime;

protected:
    bool _block = false;
    void writeImit(bool setImit)  override;
    void writeImitVal(QVariant setVal)  override;
    void _qualityChangedSlot() override;
    QVariant _setedValue {0};
    Prom::OutESTagSetType _setType;
    QVariant _iniValue {0};

    QVariant _defaultValue {0};
    bool _menuChanged {false};
    void _customConnectToGUI(QObject *guiItem, QObject *engRow) override;
    QVariant _hiLimit   {0};
    QVariant _lowLimit  {0};
    quint8   _limitFlag {OUT_LIM_NO};
    QVariant _correctByLimits( QVariant Value);

    int _impulseDuration{0};
    QTimerExt * _impTimer;
    bool _impTrigger{false};

public slots:
    void blockIt() { _block = true; }
    void unBlockIt() { _block = false; }
    void _setTimerEnd()  override;
    virtual bool setValue(QVariant Value, bool notImit = false);
    //virtual bool setValue(QVariant Value){return setValue(Value, false);};
    void setImpulseDuration(QVariant Delay);

protected slots:
    void _checkVal() override;
    void _checkPulse() override;

public slots:
    void loadParam() override;
    void saveParam() override;
    void pulseTimerEnd() override {};
};

#endif // OUTETAG_H
