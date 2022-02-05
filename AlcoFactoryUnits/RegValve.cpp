#include <QSettings>
#include "RegValve.h"
#include "InDiscretETag.h"
#include "OutDiscretETag.h"
#include "OutETag.h"

RegValve::RegValve( int *Id,
                       QString Name,
                       QString TagPefix )
  :Unit( TypeRectnColumn,
         Id,
         Name,
         TagPefix,
         false,
         Prom::UnMd2pos )
{

  _posSetPt = new OutETag( this, Prom::TpOut, Prom::PreSet, "уставка позиции",  ".posSetPt", false, false, false, true);
  _pos = new InETag( this, Prom::TpIn, "позиция", ".pos", true, 100, 0.5, false, false, false, false, true );
  _pos->onlyChange();
  _rangeTop = new OutETag( this, Prom::TpOut, Prom::PreSet, "макс. открытие",  ".rangeTop", false, false, false, true);
  _rangeBottom = new OutETag( this, Prom::TpOut, Prom::PreSet, "макс. закрытие",  ".rangeBottom", false, false, false, true);
  _openOut = new OutDiscretETag( this, Prom::PreSet, "руч. приоткрыть", ".openOut" );
  _closeOut = new OutDiscretETag( this, Prom::PreSet, "руч. призакрыть", ".closeOut" );

}

RegValve::~RegValve()
{

}
//------------------------------------------------------------------------------
void RegValve::_alarmDo()
{

}
//------------------------------------------------------------------------------
void RegValve::_resetAlarmDo()
{

}

//------------------------------------------------------------------------------
void RegValve::_updateStateAndMode()
{

}
//------------------------------------------------------------------------------
void RegValve::_doOnModeChange()
{

}

//------------------------------------------------------------------------------
Prom::SetModeResp RegValve::_customSetMode( Prom::UnitModes *mode, bool  )
{
  return RejAnnown;
}

//------------------------------------------------------------------------------
void RegValve::_customConnectToGUI( QObject *guiItem, QObject *)
{

}

//------------------------------------------------------------------------------
