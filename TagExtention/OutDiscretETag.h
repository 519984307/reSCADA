#ifndef OUTDISCRETETAG_H
#define OUTDISCRETETAG_H

#include "OutETag.h"
//#include "../../SCADAenums.h"

class Unit;

class OutDiscretETag: public OutETag
{
    Q_OBJECT
public:
    OutDiscretETag(Unit * Owner,
                    Prom::OutESTagSetType SetType,
                    QString Name,
                    QString DBName,
                    QVariant Hi = true,
                    QVariant Low = false,
                    bool Save = false,
                    bool LoadDefault = true,
                    QVariant DefaultValue = false,
                    bool MenuChanged = false,
                    bool TunableSetTime = false,
                    bool TunablePulseTime = false,
                    bool EgnorableAlarm = false,
                    bool InGUI = true,
                    Prom::ETagValConv Convertion = Prom::VCNo,
                    bool TunableImpulseTime = false);
    bool isOn();


    void _customConnectToGUI(QObject *guiItem, QObject *engRow) override;

protected:
    bool _OnOff = false;
    /*! Верхнее или нижнее значение сигнала при пульсации */
    bool _pulseHL = false;
    /*! Верхнее значение сигнала. Соответствует включению. */
    QVariant _hi;
    /*! Нижнее значение сигнала. Соответствует выключению. */
    QVariant _low;
    //void WriteImitVal(QVariant setVal)  override;
    //void WriteImit(bool setImit)  override;

private:
    using OutETag::value;
    bool setValue(QVariant Value, bool notImit = false) override;
    QVariant _hiLimit   {0};
    QVariant _lowLimit  {0};
    quint8   _limitFlag {OUT_LIM_NO};
    QVariant _correctByLimits( QVariant Value);
    QVariant hiLimit() const;
    void setHiLimit(const QVariant &hiLimit);
    QVariant lowLimit() const;
    void setLowLimit(const QVariant &lowLimit);
    void setLimitFlag(const quint8 &LimitFlag);

signals:
    void s_on();
    void s_off();
    void ChangeImpulseDelay(QVariant);

public slots:
    bool on();
    bool off();
    void singleImpulse( int Duration, int Pulse );
    //virtual void setPulse(bool set);
    void blockIt() { _block = true; off(); }
    void pulsOut(bool on);

protected slots:
    void _checkVal() override;
    void pulseTimerEnd() override;
    virtual void impilseTimerEnd();

    // ETag interface
public slots:
    void saveParam() override;
    void loadParam() override;

public:
    void reInitialise() override;
};

#endif // OUTDISCRETETAG_H
