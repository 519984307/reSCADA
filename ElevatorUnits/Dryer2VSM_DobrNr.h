#ifndef DRYER2VSM_DOBRNR_H
#define DRYER2VSM_DOBRNR_H

#ifndef UNIT_H
#include "RouteUnit.h"
#endif

#include <QObject>

class InDiscretETag;
class OutDiscretETag;
class InETag;
class OutETag;

class Dryer2VSM_DobrNr : public RouteUnit
{
    Q_OBJECT
public:
    explicit Dryer2VSM_DobrNr( int ID,
                               QString Name,
                               QString TagPefix );

protected:
    InETag * _inHumidity { nullptr };
    InETag * _outHumidity { nullptr };

    OutDiscretETag * _alarmOut   = nullptr;
    InDiscretETag *  _alarmIn    = nullptr;
    OutDiscretETag * _fire       = nullptr;
    OutDiscretETag * _wrngSound  = nullptr;
    OutDiscretETag * _alarmSound = nullptr;
    OutDiscretETag * _start      = nullptr;
    OutDiscretETag * _stop       = nullptr;

    OutETag * setT1 = nullptr;
    OutETag * setT2 = nullptr;
    OutETag * setT3 = nullptr;
    OutETag * setT4 = nullptr;
    OutETag * setT5 = nullptr;
    OutETag * setT6 = nullptr;
    OutETag * setT15 = nullptr;
    OutETag * setT16 = nullptr;



    InETag * _T1  = nullptr;
    InETag * _T2  = nullptr;
    InETag * _T3  = nullptr;
    InETag * _T4  = nullptr;
    InETag * _T5  = nullptr;
    InETag * _T6  = nullptr;
    InETag * _T7  = nullptr;
    InETag * _T8  = nullptr;
    InETag * _T9  = nullptr;
    InETag * _T10 = nullptr;
    InETag * _T11 = nullptr;
    InETag * _T12 = nullptr;
    InETag * _T13 = nullptr;
    InETag * _T14 = nullptr;
    InETag * _T15 = nullptr;
    InETag * _T16 = nullptr;

    void _alarmDo() override;
    void _resetAlarmDo() override;

signals:
    //---- Уставки t°C
    void SetLvlTempHL  ( QVariant );
    void SetLvlTempML1 ( QVariant );
    void SetLvlTempML2 ( QVariant );
    void SetLvlTempLL  ( QVariant );

    //---- Пуск Стоп
    void Started();
    void Stoped();

public slots:
    //    //---- Уставки t°C
    void GetLvlTempHL  ( QVariant );
    void ChangeDetectLvlTempHL  (QVariant Val);
    void GetLvlTempML1 ( QVariant );
    void ChangeDetectLvlTempML1 (QVariant Val);
    void GetLvlTempML2 ( QVariant );
    void ChangeDetectLvlTempML2 (QVariant Val);
    void GetLvlTempLL  ( QVariant );
    void ChangeDetectLvlTempLL (QVariant Val);

    //    //---- Пуск Стоп
    void Start();
    void Stop();


    void AddToCurrentRoteFull()  { _AddToCurrentRoute( Prom::UnMdFull ); }

protected slots:
    Prom::SetModeResp _customSetMode( Prom::UnitModes *mode, bool UserOrSys ) override;
    void _customConnectToGUI( QObject * guiItem,  QObject * propWin ) override;

protected:
    void _updateStateAndMode() override;
};

#endif // DRYER2VSM_DOBRNR_H
