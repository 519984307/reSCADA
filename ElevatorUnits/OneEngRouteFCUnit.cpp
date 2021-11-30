#include "OneEngRouteFCUnit.h"
#include "InDiscretETag.h"
//#include "InETag.h"
#include "OutETag.h"

OneEngRouteFCUnit::OneEngRouteFCUnit(Prom::UnitType Type,
                                      int ID,
                                      QString Name,
                                      QString TagPefix,
                                      bool Mover,
                                      std::vector<QString> EngSensorNames )

    : OneEngRouteUnit( Type,
                        ID,
                        Name,
                        TagPefix,
                        Mover,
                        EngSensorNames)
{
    _SetFR = new OutETag(this, Prom::TpOut, Prom::PreSet,
                          "Установка частоты ЧП",
                          ".SetFR", false, false, false, true, Prom::VCFloatInIntToDouble, true);
    //connect(_SetFR, &OutETag::s_valueChd ,this, &OneEngRouteFCUnit::SetSpeed, Qt::QueuedConnection);
    _OutFR = new InETag(this, Prom::TpIn,
                         "Выходная частота ЧП",
                         ".OutFR", true, 0, 2, false, false, false, false, true, Prom::VCFloatInIntToDouble);
    _OutFR->onlyChange();

    _SetedFR = new InETag(this, Prom::TpIn,
                           "Установленная частота ЧП",
                           ".SetedFR", true, 0, 2, false, false, false, false, true, Prom::VCFloatInIntToDouble );
    _SetedFR->onlyChange();
}

//------------------------------------------------------------------------------
void OneEngRouteFCUnit::GetSpeed(QVariant Value)
{
    _SetFR->setValue(Value);
}

//------------------------------------------------------------------------------
//void OneEngRouteFCUnit::_UpdateStateAndMode()
//{
//    OneEngRouteUnit::_UpdateStateAndMode();

//}
//------------------------------------------------------------------------------
void OneEngRouteFCUnit::_customConnectToGUI(QObject *guiItem,  QObject *propWin)
{
    EngRouteUnit::_customConnectToGUI(guiItem,  propWin);
    connect(_SetFR,  SIGNAL(s_valueChd(QVariant)),       guiItem, SLOT(setSpeed(QVariant)), Qt::QueuedConnection);
    connect(guiItem, SIGNAL(speedChanged (QVariant)), _SetFR,  SLOT(setValue(QVariant)), Qt::QueuedConnection);
}

//------------------------------------------------------------------------------
