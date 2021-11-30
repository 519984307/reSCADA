#include "OneEngRouteFClUnit.h"
#include "InDiscretETag.h"
//#include "InETag.h"
#include "OutETag.h"

OneEngRouteFClUnit::OneEngRouteFClUnit( Prom::UnitType Type,
                                      int ID,
                                      QString Name,
                                      QString TagPefix,
                                      bool Mover,
                                      std::vector<QString> EngSensorNames  )

    : OneEngRouteUnit(  Type,
                        ID,
                        Name,
                        TagPefix,
                        Mover,
                        EngSensorNames )
{
    _SetSp = new OutETag( this, Prom::TpOut, Prom::PreSet,
                          "Установка скорости",
                          ".setSpeedLG", false, false, false, true, Prom::VCNo, true );
    //connect( _SetFR, &OutETag::s_valueChd ,this, &OneEngRouteFCUnit::SetSpeed, Qt::QueuedConnection );
    _OutSp = new InETag( this, Prom::TpIn,
                         "Текущая скорость",
                         ".outSpeedLG", true, 0, 2, false, false, false, false, true, Prom::VCNo );
    _OutSp->onlyChange();
}

//-----------------------------------------------------------------------------------
void OneEngRouteFClUnit::GetSpeed( QVariant Value )
{
    _SetSp->setValue( Value );
}

//-----------------------------------------------------------------------------------
//void OneEngRouteFCUnit::_UpdateStateAndMode()
//{
//    OneEngRouteUnit::_UpdateStateAndMode();

//}
//-----------------------------------------------------------------------------------
void OneEngRouteFClUnit::_customConnectToGUI( QObject *guiItem,  QObject *propWin )
{
    EngRouteUnit::_customConnectToGUI( guiItem,  propWin );
    connect( _SetSp,  SIGNAL( s_valueChd( QVariant ) ),       guiItem, SLOT( setSpeed( QVariant ) ), Qt::QueuedConnection );
    connect( guiItem, SIGNAL( speedChanged ( QVariant ) ), _SetSp,  SLOT( setValue( QVariant ) ), Qt::QueuedConnection );
}

//-----------------------------------------------------------------------------------
