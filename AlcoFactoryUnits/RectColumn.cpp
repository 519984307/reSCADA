#include <QSettings>
#include "RectColumn.h"
#include "InDiscretETag.h"
#include "OutDiscretETag.h"
#include "OutETag.h"
#include "RegValve.h"

RectColumn::RectColumn( int * Id,
                       QString Name,
                       QString TagPefix )
  :Unit( TypeRectnColumn,
         Id,
         Name,
         TagPefix,
         false,
         Prom::UnMdStop )
{
  _tempOutWtr = new InETag( this, Prom::TpIn, "t°C отходящей воды", ".tWater", true, 80, 2, false, false, true, true, true );
  _tempOutWtr->needBeUndetectedAlarm();

  _tempTop = new InETag( this, Prom::TpIn, "t°C верха", ".tTop", true, 120, 2, false, false, true, true, true );
  _tempTop->needBeUndetectedAlarm();
  _presTop = new InETag( this, Prom::TpIn, "давление верха", ".pTop", true, 4, 0.05, false, false, true, true, true );
  _presTop->needBeUndetectedAlarm();

  _tempBottom = new InETag( this, Prom::TpIn, "t°C низа", ".tBottom", true, 120, 2, false, false, true, true, true );
  _tempTop->needBeUndetectedAlarm();
  _presBottom = new InETag( this, Prom::TpIn, "давление низа", ".pBottom", true, 4, 0.05, false, false, true, true, true );
  _presBottom->needBeUndetectedAlarm();

  _clWtrValve = new RegValve(Id, Name + " клапан охл. воды", tagPrefix + ".vWater");
  addSubUnit( _clWtrValve );

  _valveToPresTop = new PIDstep( this, "ПИД рег-р давления верха ",  ".pTopPID", PIDopt::allOn & ~PIDopt::kD );
  _valveToPresTop->process = _presTop;
  _valveToPresTop->impIn = _clWtrValve->getPosTag();
  _valveToPresTop->impOut = _clWtrValve->getPosSetPtTag();

  _steamValve = new RegValve(Id, Name + " клапан пара", tagPrefix + ".vSteam");
  addSubUnit( _steamValve );
  _valveToPresBottom = new PIDstep( this, "ПИД рег-р давления низа ", ".pBottomPID", PIDopt::allOn & ~PIDopt::kD );
  _valveToPresBottom->process = _presTop;
  _valveToPresBottom->impIn = _steamValve->getPosTag();
  _valveToPresBottom->impOut = _steamValve->getPosSetPtTag();
}
//------------------------------------------------------------------------------
RectColumn::~RectColumn()
{

}
//------------------------------------------------------------------------------
void RectColumn::_alarmDo()
{

}
//------------------------------------------------------------------------------
void RectColumn::_resetAlarmDo()
{

}

//------------------------------------------------------------------------------
void RectColumn::_updateStateAndMode()
{

}
//------------------------------------------------------------------------------
void RectColumn::_doOnModeChange()
{

}

//------------------------------------------------------------------------------
Prom::SetModeResp RectColumn::_customSetMode( Prom::UnitModes *mode, bool  )
{
  return RejAnnown;
}

//------------------------------------------------------------------------------
void RectColumn::_customConnectToGUI( QObject *guiItem, QObject *)
{
  //connect(_inHumidity,   SIGNAL(s_valueChd(QVariant)),    guiItem, SLOT(setHumIn(QVariant) ),          Qt::QueuedConnection);
  //connect(_outHumidity,  SIGNAL(s_valueChd(QVariant)),    guiItem, SLOT(setHumOut(QVariant)),          Qt::QueuedConnection);

//  connect( _T1 ,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp1 ( QVariant ) ),   Qt::QueuedConnection );
//  connect( _T2 ,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp2 ( QVariant ) ),   Qt::QueuedConnection );
//  connect( _T3 ,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp3 ( QVariant ) ),   Qt::QueuedConnection );
//  connect( _T4 ,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp4 ( QVariant ) ),   Qt::QueuedConnection );
//  connect( _T5 ,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp5 ( QVariant ) ),   Qt::QueuedConnection );
//  connect( _T6 ,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp6 ( QVariant ) ),   Qt::QueuedConnection );
//  connect( _T7 ,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp7 ( QVariant ) ),   Qt::QueuedConnection );
//  connect( _T8 ,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp8 ( QVariant ) ),   Qt::QueuedConnection );
//  connect( _T9 ,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp9 ( QVariant ) ),   Qt::QueuedConnection );
//  connect( _T10,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp10( QVariant ) ),   Qt::QueuedConnection );
//  connect( _T11,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp11( QVariant ) ),   Qt::QueuedConnection );
//  connect( _T12,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp12( QVariant ) ),   Qt::QueuedConnection );
//  connect( _T13,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp13( QVariant ) ),   Qt::QueuedConnection );
//  connect( _T14,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp14( QVariant ) ),   Qt::QueuedConnection );
//  connect( _T15,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp15( QVariant ) ),   Qt::QueuedConnection );
//  connect( _T16,    SIGNAL( s_valueChd( QVariant ) ), guiItem,  SLOT( setTemp16( QVariant ) ),   Qt::QueuedConnection );

//  connect( guiItem, SIGNAL( lineLimitTempHLChanged ( QVariant ) ), this, SLOT( GetLvlTempHL ( QVariant ) ),   Qt::QueuedConnection );
//  connect( guiItem, SIGNAL( lineLimitTempML1Changed( QVariant ) ), this, SLOT( GetLvlTempML1( QVariant ) ),   Qt::QueuedConnection );
//  connect( guiItem, SIGNAL( lineLimitTempML2Changed( QVariant ) ), this, SLOT( GetLvlTempML2( QVariant ) ),   Qt::QueuedConnection );
//  connect( guiItem, SIGNAL( lineLimitTempLLChanged ( QVariant ) ), this, SLOT( GetLvlTempLL ( QVariant ) ),   Qt::QueuedConnection );

//  connect( this,    SIGNAL( SetLvlTempHL ( QVariant ) ),        guiItem, SLOT( setLineLimitTempHL ( QVariant ) ),   Qt::QueuedConnection );
//  connect( this,    SIGNAL( SetLvlTempML1( QVariant ) ),        guiItem, SLOT( setLineLimitTempML1( QVariant ) ),   Qt::QueuedConnection );
//  connect( this,    SIGNAL( SetLvlTempML2( QVariant ) ),        guiItem, SLOT( setLineLimitTempML2( QVariant ) ),   Qt::QueuedConnection );
//  connect( this,    SIGNAL( SetLvlTempLL ( QVariant ) ),        guiItem, SLOT( setLineLimitTempLL ( QVariant ) ),   Qt::QueuedConnection );

//  connect( guiItem, SIGNAL( start() ),   this,    SLOT( Start() ),   Qt::QueuedConnection );
//  connect( guiItem, SIGNAL( stop() ),    this,    SLOT( Stop() ),    Qt::QueuedConnection );
//  connect( this,    SIGNAL( Stoped() ),  guiItem, SLOT( stoped() ),  Qt::QueuedConnection );
//  connect( this,    SIGNAL( Started() ), guiItem, SLOT( started() ), Qt::QueuedConnection );
}

//------------------------------------------------------------------------------

PID::PID(Unit *Owner, QString Name, QString TagPrefix, char Option )
  :tagPrefix(TagPrefix),
  name(Name),
  opt(Option)
{
  manOn = new OutDiscretETag( Owner, Prom::PreSet, name + " ручной режим", tagPrefix + ".manOn" );
  setPt = new OutETag( Owner, Prom::TpOut, Prom::PreSet, name + " уставка",  tagPrefix + ".setPt", false, false, false, true);
  if( opt & PIDopt::kP ) kP = new OutETag( Owner, Prom::TpOut, Prom::PreSet, name + " коэф. П",  tagPrefix + ".kP", false, false, false, true);
  if( opt & PIDopt::kI ) kI = new OutETag( Owner, Prom::TpOut, Prom::PreSet, name + " коэф. И",  tagPrefix + ".kI", false, false, false, true);
  if( opt & PIDopt::kD ) kD = new OutETag( Owner, Prom::TpOut, Prom::PreSet, name + " коэф. Д",  tagPrefix + ".kD", false, false, false, true);
}

//------------------------------------------------------------------------------
PIDstep::PIDstep(Unit *Owner, QString Name, QString TagPrefix, char Option )
  :PID(Owner, Name, TagPrefix, Option)
{
  manImpUp = new OutDiscretETag( Owner, Prom::PreSet, name + " руч. больше", tagPrefix + ".manImplMore" );
  manImpDown = new OutDiscretETag( Owner, Prom::PreSet, name + " руч. меньше", tagPrefix + ".manImplLess" );
  impUp = new InDiscretETag( Owner, name + " авто. меньше", tagPrefix + ".implMore");
  impDown = new InDiscretETag( Owner, name + " авто. больше", tagPrefix + ".implLess");
}

//------------------------------------------------------------------------------
PIDcont::PIDcont(Unit *Owner, QString Name, QString TagPrefix, char Option )
  :PID(Owner, Name, TagPrefix, Option)
{
  manImp = new OutETag( Owner, Prom::TpOut, Prom::PreSet, name + " руч. воздей-е",  tagPrefix + ".manImpl", false, false, false, true);
  impLimUpLim = new OutETag( Owner, Prom::TpOut, Prom::PreSet, name + " макс. воздей-е",  tagPrefix + ".implMax", false, false, false, true);
  impLimDown = new OutETag( Owner, Prom::TpOut, Prom::PreSet, name + " мин. воздей-е",  tagPrefix + ".implMin", false, false, false, true);
}

//------------------------------------------------------------------------------
