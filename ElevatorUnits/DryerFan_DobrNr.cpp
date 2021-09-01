#include "InDiscretETag.h"
#include "DryerFan_DobrNr.h"

DryerFan_DobrNr::DryerFan_DobrNr( int ID,
                                  QString Name,
                                  QString TagPefix,
                                  std::vector<QString> EngSensorNames  )

    : OneEngRouteFCUnit(  Prom::TyteDryerFan_VSMDbr,
                          ID,
                          Name,
                          TagPefix,
                          false,
                          EngSensorNames )
{
    _OnFC = new InDiscretETag( this,
                               "Работа через ЧП",
                               ".OnFC", true, false,
                               true, false, false, false );
    _OnFC->onlyChange();

    _T = new InETag( this, Prom::TpIn, "Датчик t°C", ".T", true, 30, 5, false, false, false, false, true, Prom::VCFloatInIntToDouble );
    _T->needBeUndetectedAlarm();

}

//------------------------------------------------------------------------------
void DryerFan_DobrNr::_customConnectToGUI( QObject *guiItem, QObject *propWin)
{
    OneEngRouteFCUnit::_customConnectToGUI( guiItem, propWin );
    connect( _T,      SIGNAL( s_valueChd( QVariant ) ),           guiItem, SLOT( setTemp( QVariant ) ),        Qt::QueuedConnection );
    connect( guiItem, SIGNAL( limitTempChanged ( QVariant ) ), _T,      SLOT( setDetectLevel( QVariant ) ), Qt::QueuedConnection );
    connect( _T,      SIGNAL( ChangeDelectLewel( QVariant ) ), guiItem, SLOT( setLimitTemp( QVariant ) ),   Qt::QueuedConnection );
}

//------------------------------------------------------------------------------
