#include <QSettings>
#include "Dryer2VSM_DobrNr.h"
#include "InDiscretETag.h"
#include "OutDiscretETag.h"
#include "OutETag.h"

Dryer2VSM_DobrNr::Dryer2VSM_DobrNr( int ID,
                                    QString Name,
                                    QString TagPefix )
    :RouteUnit( Prom::TypeDryer2VSM_DobrNr,
                ID,
                Name,
                TagPefix,
                false,
                Prom::UnMdStop )
{
    _alarmOut  = new OutDiscretETag( this, Prom::PreSet, "Авария в ЩСУ 1", ".alarmOut" );
    _alarmIn  = new InDiscretETag( this, "Авария из ЩСУ", ".alarmIn", true, false, true, false );
    _alarmIn->needBeUndetectedAlarm();
    _fire      = new OutDiscretETag( this, Prom::PreSet, "Пожар", ".fire");
    _wrngSound = new OutDiscretETag( this, Prom::PreSet, "Сирена предупреждения", ".WSD");
    _alarmSound= new OutDiscretETag( this, Prom::PreSet, "Сирена аварии", ".ASD");
    _start     = new OutDiscretETag( this, Prom::PreSet, "Пуск сушилки", ".start");
    connect( _start, &OutDiscretETag::s_on, this, &Dryer2VSM_DobrNr::Started, Qt::QueuedConnection );
    _stop      = new OutDiscretETag( this, Prom::PreSet, "Стоп сушилки", ".stop");
    connect( _stop,  &OutDiscretETag::s_on, this, &Dryer2VSM_DobrNr::Stoped,  Qt::QueuedConnection );

//    _inHumidity   = new InETag(this, TpIn, "Вх. влагомер", ".humIn", false, 1000, 5, false, false, false, false, true, VCFloatInIntToDouble);
//    _outHumidity  = new InETag(this, TpIn, "Вых. влагомер", ".humOut", false, 1000, 5, false, false, false, false, true, VCFloatInIntToDouble);

    _T1 = new InETag( this, Prom::TpIn, "Датчик t°C 1", ".T1", true, 30, 5, false, false, false, false, true, Prom::VCdiv10 );
    _T1->needBeUndetectedAlarm();
    _T2 = new InETag( this, Prom::TpIn, "Датчик t°C 2", ".T2", true, 30, 5, false, false, false, false, true, Prom::VCdiv10 );
    _T2->needBeUndetectedAlarm();
    _T3 = new InETag( this, Prom::TpIn, "Датчик t°C 3", ".T3", true, 30, 5, false, false, false, false, true, Prom::VCdiv10 );
    _T3->needBeUndetectedAlarm();
    _T4 = new InETag( this, Prom::TpIn, "Датчик t°C 4", ".T4", true, 30, 5, false, false, false, false, true, Prom::VCdiv10 );
    _T4->needBeUndetectedAlarm();
    _T5 = new InETag( this, Prom::TpIn, "Датчик t°C 5", ".T5", true, 30, 5, false, false, false, false, true, Prom::VCdiv10 );
    _T5->needBeUndetectedAlarm();
    _T6 = new InETag( this, Prom::TpIn, "Датчик t°C 6", ".T6", true, 30, 5, false, false, false, false, true, Prom::VCdiv10 );
    _T6->needBeUndetectedAlarm();
    _T7 = new InETag( this, Prom::TpIn, "Датчик t°C 7", ".T7", true, 30, 5, false, false, false, true, true, Prom::VCdiv10 );
    _T7->needBeUndetectedAlarm();
    _T8 = new InETag( this, Prom::TpIn, "Датчик t°C 8", ".T8", true, 30, 5, false, false, false, true, true, Prom::VCdiv10 );
    _T8->needBeUndetectedAlarm();
    _T9 = new InETag( this, Prom::TpIn, "Датчик t°C 9", ".T9", true, 30, 5, false, false, false, true, true, Prom::VCdiv10 );
    _T9->needBeUndetectedAlarm();
    _T10 = new InETag( this, Prom::TpIn, "Датчик t°C 10", ".T10", true, 30, 5, false, false, false, true, true, Prom::VCdiv10 );
    _T10->needBeUndetectedAlarm();
    _T11 = new InETag( this, Prom::TpIn, "Датчик t°C 11", ".T11", true, 30, 5, false, false, false, true, true, Prom::VCdiv10 );
    _T11->needBeUndetectedAlarm();
    _T12 = new InETag( this, Prom::TpIn, "Датчик t°C 12", ".T12", true, 30, 5, false, false, false, true, true, Prom::VCdiv10 );
    _T12->needBeUndetectedAlarm();
    _T13 = new InETag( this, Prom::TpIn, "Датчик t°C 13", ".T13", true, 30, 5, false, false, false, true, true, Prom::VCdiv10 );
    _T13->needBeUndetectedAlarm();
    _T14 = new InETag( this, Prom::TpIn, "Датчик t°C 14", ".T14", true, 30, 5, false, false, false, true, true, Prom::VCdiv10 );
    _T14->needBeUndetectedAlarm();
    _T15 = new InETag( this, Prom::TpIn, "Датчик t°C 15", ".T15", true, 30, 5, false, false, false, false, true, Prom::VCdiv10 );
    _T15->needBeUndetectedAlarm();
    _T16 = new InETag( this, Prom::TpIn, "Датчик t°C 16", ".T16", true, 30, 5, false, false, false, false, true, Prom::VCdiv10 );
    _T16->needBeUndetectedAlarm();

    ///отключить потом
    setT1    = new OutETag( this, Prom::TpOut, Prom::PreSet, "Уставка порога t°C 1",  ".setT1",  false, false, false, true, Prom::VCdiv10, true);
    setT2    = new OutETag( this, Prom::TpOut, Prom::PreSet, "Уставка порога t°C 2",  ".setT2",  false, false, false, true, Prom::VCdiv10, true);
    setT3    = new OutETag( this, Prom::TpOut, Prom::PreSet, "Уставка порога t°C 3",  ".setT3",  false, false, false, true, Prom::VCdiv10, true);
    setT4    = new OutETag( this, Prom::TpOut, Prom::PreSet, "Уставка порога t°C 4",  ".setT4",  false, false, false, true, Prom::VCdiv10, true);
    setT5    = new OutETag( this, Prom::TpOut, Prom::PreSet, "Уставка порога t°C 5",  ".setT5",  false, false, false, true, Prom::VCdiv10, true);
    setT6    = new OutETag( this, Prom::TpOut, Prom::PreSet, "Уставка порога t°C 6",  ".setT6",  false, false, false, true, Prom::VCdiv10, true);
    setT15   = new OutETag( this, Prom::TpOut, Prom::PreSet, "Уставка порога t°C 15", ".setT15", false, false, false, true, Prom::VCdiv10, true);
    setT16   = new OutETag( this, Prom::TpOut, Prom::PreSet, "Уставка порога t°C 16", ".setT16", false, false, false, true, Prom::VCdiv10, true);

    connect( _T1, &InDiscretETag::s_delectLevelChanged, this, &Dryer2VSM_DobrNr::ChangeDetectLvlTempHL, Qt::QueuedConnection );
    connect( _T2, &InDiscretETag::s_delectLevelChanged, this, &Dryer2VSM_DobrNr::ChangeDetectLvlTempHL, Qt::QueuedConnection );
    connect( _T3, &InDiscretETag::s_delectLevelChanged, this, &Dryer2VSM_DobrNr::ChangeDetectLvlTempHL, Qt::QueuedConnection );
    connect( _T4, &InDiscretETag::s_delectLevelChanged, this, &Dryer2VSM_DobrNr::ChangeDetectLvlTempHL, Qt::QueuedConnection );
    connect( setT1, &OutETag::s_valueChd,            this, &Dryer2VSM_DobrNr::ChangeDetectLvlTempHL, Qt::QueuedConnection );
    connect( setT2, &OutETag::s_valueChd,            this, &Dryer2VSM_DobrNr::ChangeDetectLvlTempHL, Qt::QueuedConnection );
    connect( setT3, &OutETag::s_valueChd,            this, &Dryer2VSM_DobrNr::ChangeDetectLvlTempHL, Qt::QueuedConnection );
    connect( setT4, &OutETag::s_valueChd,            this, &Dryer2VSM_DobrNr::ChangeDetectLvlTempHL, Qt::QueuedConnection );

    connect( _T5, &InDiscretETag::s_delectLevelChanged, this, &Dryer2VSM_DobrNr::ChangeDetectLvlTempML1, Qt::QueuedConnection );
    connect( _T6, &InDiscretETag::s_delectLevelChanged, this, &Dryer2VSM_DobrNr::ChangeDetectLvlTempML1, Qt::QueuedConnection );
    connect( setT5, &OutETag::s_valueChd,              this, &Dryer2VSM_DobrNr::ChangeDetectLvlTempML1, Qt::QueuedConnection );
    connect( setT6, &OutETag::s_valueChd,              this, &Dryer2VSM_DobrNr::ChangeDetectLvlTempML1, Qt::QueuedConnection );

    connect( _T7, &InDiscretETag::s_delectLevelChanged, this, &Dryer2VSM_DobrNr::ChangeDetectLvlTempML2, Qt::QueuedConnection );
    connect( _T8, &InDiscretETag::s_delectLevelChanged, this, &Dryer2VSM_DobrNr::ChangeDetectLvlTempML2, Qt::QueuedConnection );
    connect( _T9, &InDiscretETag::s_delectLevelChanged, this, &Dryer2VSM_DobrNr::ChangeDetectLvlTempML2, Qt::QueuedConnection );
    connect( _T10, &InDiscretETag::s_delectLevelChanged, this, &Dryer2VSM_DobrNr::ChangeDetectLvlTempML2, Qt::QueuedConnection );
    connect( _T11, &InDiscretETag::s_delectLevelChanged, this, &Dryer2VSM_DobrNr::ChangeDetectLvlTempML2, Qt::QueuedConnection );
    connect( _T12, &InDiscretETag::s_delectLevelChanged, this, &Dryer2VSM_DobrNr::ChangeDetectLvlTempML2, Qt::QueuedConnection );
    connect( _T13, &InDiscretETag::s_delectLevelChanged, this, &Dryer2VSM_DobrNr::ChangeDetectLvlTempML2, Qt::QueuedConnection );
    connect( _T14, &InDiscretETag::s_delectLevelChanged, this, &Dryer2VSM_DobrNr::ChangeDetectLvlTempML2, Qt::QueuedConnection );

    connect( _T15, &InDiscretETag::s_delectLevelChanged, this, &Dryer2VSM_DobrNr::ChangeDetectLvlTempLL, Qt::QueuedConnection );
    connect( _T16, &InDiscretETag::s_delectLevelChanged, this, &Dryer2VSM_DobrNr::ChangeDetectLvlTempLL, Qt::QueuedConnection );
    connect( setT15, &OutETag::s_valueChd,                 this, &Dryer2VSM_DobrNr::ChangeDetectLvlTempLL, Qt::QueuedConnection );
    connect( setT16, &OutETag::s_valueChd,                 this, &Dryer2VSM_DobrNr::ChangeDetectLvlTempLL, Qt::QueuedConnection );

}
//------------------------------------------------------------------------------
void Dryer2VSM_DobrNr::_alarmDo()
{
    _alarmOut->on();
    _alarmSound->on();
}
//------------------------------------------------------------------------------
void Dryer2VSM_DobrNr::_resetAlarmDo()
{
    _alarmOut->off();
    _alarmSound->off();
    _stop->off();
}

//------------------------------------------------------------------------------
void Dryer2VSM_DobrNr::GetLvlTempHL( QVariant Lvl )
{
    Lvl = Lvl.toDouble();
    _T1->setDetectLevel( Lvl );
    _T2->setDetectLevel( Lvl );
    _T3->setDetectLevel( Lvl );
    _T4->setDetectLevel( Lvl );
    setT1->setValue( Lvl );
    setT2->setValue( Lvl );
    setT3->setValue( Lvl );
    setT4->setValue( Lvl );
}

//------------------------------------------------------------------------------
void Dryer2VSM_DobrNr::ChangeDetectLvlTempHL( QVariant Val )
{
    if( _T1->detectLevel() == Val &&
            _T2->detectLevel() == Val &&
            _T3->detectLevel() == Val &&
            _T4->detectLevel() == Val &&
            setT1->value() == Val &&
            setT2->value() == Val &&
            setT3->value() == Val &&
            setT4->value() == Val ) emit SetLvlTempHL( Val );
    //else emit SetLvlTempHL( 0 );
}

//------------------------------------------------------------------------------
void Dryer2VSM_DobrNr::GetLvlTempML1( QVariant Lvl)
{
    _T5->setDetectLevel( Lvl );
    _T6->setDetectLevel( Lvl );
    setT5->setValue( Lvl );
    setT6->setValue( Lvl );
}
//------------------------------------------------------------------------------
void Dryer2VSM_DobrNr::ChangeDetectLvlTempML1( QVariant Val )
{
    if( _T5->detectLevel() == Val &&
            _T6->detectLevel() == Val &&
            setT5->value() == Val &&
            setT6->value() == Val ) emit SetLvlTempML1( Val );
    //else emit SetLvlTempML1( 0 );
}

//------------------------------------------------------------------------------
void Dryer2VSM_DobrNr::GetLvlTempML2( QVariant Lvl)
{
    _T7->setDetectLevel( Lvl );
    _T8->setDetectLevel( Lvl );
    _T9->setDetectLevel( Lvl );
    _T10->setDetectLevel( Lvl );
    _T11->setDetectLevel( Lvl );
    _T12->setDetectLevel( Lvl );
    _T13->setDetectLevel( Lvl );
    _T14->setDetectLevel( Lvl );
}

//------------------------------------------------------------------------------
void Dryer2VSM_DobrNr::ChangeDetectLvlTempML2(QVariant Val)
{
    if(  _T7->detectLevel() == Val &&
         _T8->detectLevel() == Val &&
         _T9->detectLevel() == Val &&
         _T10->detectLevel() == Val &&
         _T11->detectLevel() == Val &&
         _T12->detectLevel() == Val &&
         _T13->detectLevel() == Val &&
         _T14->detectLevel() == Val ) emit SetLvlTempML2( Val );
    //else emit SetLvlTempML2( 0 );
}

//------------------------------------------------------------------------------
void Dryer2VSM_DobrNr::GetLvlTempLL( QVariant Lvl)
{
    _T15->setDetectLevel( Lvl );
    _T16->setDetectLevel( Lvl );
    setT15->setValue( Lvl );
    setT16->setValue( Lvl );
}

//------------------------------------------------------------------------------
void Dryer2VSM_DobrNr::ChangeDetectLvlTempLL(QVariant Val)
{
    if( _T15->detectLevel() == Val &&
            _T16->detectLevel() == Val &&
            setT15->value() == Val &&
            setT16->value() == Val ) emit SetLvlTempLL( Val );
    //else emit SetLvlTempLL( 0 );
}

//------------------------------------------------------------------------------
void Dryer2VSM_DobrNr::Start()
{
    if( (! _stop->isOn()) && ! _alarm ){
        _start->on();
        emit Started();
        logging( Prom::MessChangeCommand, QDateTime::currentDateTime(), true, objectName(), QString( "Пуск сушилки" ) );
    }
}
//------------------------------------------------------------------------------
void Dryer2VSM_DobrNr::Stop()
{
    _start->off();
    _stop->on();
    QTimer::singleShot( 5000, [ this ](  ) { this->_stop->off(); } );
    logging( Prom::MessChangeCommand, QDateTime::currentDateTime(), true, objectName(), QString( "Остановка сушилки" ) );

}
//------------------------------------------------------------------------------
void Dryer2VSM_DobrNr::_updateStateAndMode()
{

}

//------------------------------------------------------------------------------
Prom::SetModeResp Dryer2VSM_DobrNr::_customSetMode( Prom::UnitModes *mode, bool  )
{
    return RejAnnown;
}

//------------------------------------------------------------------------------
void Dryer2VSM_DobrNr::_customConnectToGUI( QObject *guiItem, QObject *)
{
    //connect(_inHumidity,   SIGNAL(s_valueChd(QVariant)),    guiItem, SLOT(setHumIn(QVariant) ),          Qt::QueuedConnection);
    //connect(_outHumidity,  SIGNAL(s_valueChd(QVariant)),    guiItem, SLOT(setHumOut(QVariant)),          Qt::QueuedConnection);

    connect( _T1 ,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp1 ( QVariant ) ),   Qt::QueuedConnection );
    connect( _T2 ,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp2 ( QVariant ) ),   Qt::QueuedConnection );
    connect( _T3 ,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp3 ( QVariant ) ),   Qt::QueuedConnection );
    connect( _T4 ,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp4 ( QVariant ) ),   Qt::QueuedConnection );
    connect( _T5 ,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp5 ( QVariant ) ),   Qt::QueuedConnection );
    connect( _T6 ,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp6 ( QVariant ) ),   Qt::QueuedConnection );
    connect( _T7 ,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp7 ( QVariant ) ),   Qt::QueuedConnection );
    connect( _T8 ,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp8 ( QVariant ) ),   Qt::QueuedConnection );
    connect( _T9 ,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp9 ( QVariant ) ),   Qt::QueuedConnection );
    connect( _T10,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp10( QVariant ) ),   Qt::QueuedConnection );
    connect( _T11,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp11( QVariant ) ),   Qt::QueuedConnection );
    connect( _T12,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp12( QVariant ) ),   Qt::QueuedConnection );
    connect( _T13,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp13( QVariant ) ),   Qt::QueuedConnection );
    connect( _T14,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp14( QVariant ) ),   Qt::QueuedConnection );
    connect( _T15,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp15( QVariant ) ),   Qt::QueuedConnection );
    connect( _T16,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp16( QVariant ) ),   Qt::QueuedConnection );

    connect( guiItem, SIGNAL( lineLimitTempHLChanged ( QVariant ) ), this, SLOT( GetLvlTempHL ( QVariant ) ),   Qt::QueuedConnection );
    connect( guiItem, SIGNAL( lineLimitTempML1Changed( QVariant ) ), this, SLOT( GetLvlTempML1( QVariant ) ),   Qt::QueuedConnection );
    connect( guiItem, SIGNAL( lineLimitTempML2Changed( QVariant ) ), this, SLOT( GetLvlTempML2( QVariant ) ),   Qt::QueuedConnection );
    connect( guiItem, SIGNAL( lineLimitTempLLChanged ( QVariant ) ), this, SLOT( GetLvlTempLL ( QVariant ) ),   Qt::QueuedConnection );

    connect( this,    SIGNAL( SetLvlTempHL ( QVariant ) ),        guiItem, SLOT( setLineLimitTempHL ( QVariant ) ),   Qt::QueuedConnection );
    connect( this,    SIGNAL( SetLvlTempML1( QVariant ) ),        guiItem, SLOT( setLineLimitTempML1( QVariant ) ),   Qt::QueuedConnection );
    connect( this,    SIGNAL( SetLvlTempML2( QVariant ) ),        guiItem, SLOT( setLineLimitTempML2( QVariant ) ),   Qt::QueuedConnection );
    connect( this,    SIGNAL( SetLvlTempLL ( QVariant ) ),        guiItem, SLOT( setLineLimitTempLL ( QVariant ) ),   Qt::QueuedConnection );

    connect( guiItem, SIGNAL( start() ),   this,    SLOT( Start() ),   Qt::QueuedConnection );
    connect( guiItem, SIGNAL( stop() ),    this,    SLOT( Stop() ),    Qt::QueuedConnection );
    connect( this,    SIGNAL( Stoped() ),  guiItem, SLOT( stoped() ),  Qt::QueuedConnection );
    connect( this,    SIGNAL( Started() ), guiItem, SLOT( started() ), Qt::QueuedConnection );
}

//------------------------------------------------------------------------------
