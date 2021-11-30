#include "InDiscretETag.h"
#include "OutETag.h"
#include "DryerFan_DobrNrST.h"

DryerFan_DobrNrST::DryerFan_DobrNrST( int ID,
                                      QString Name,
                                      QString TagPefix,
                                      std::vector<QString> EngSensorNames  )

    : DryerFan_DobrNr(  ID,
                        Name,
                        TagPefix,
                        EngSensorNames )
{
    _setT = new OutETag( this, Prom::TpOut, Prom::PreSet, "Уставка порога t°C", ".setT", false, false, false, true, Prom::VCFloatInIntToDouble, true );
    connect( _setT, &OutETag::s_valueChd, _T, &InETag::setDetectLevel, Qt::QueuedConnection );
}

//-----------------------------------------------------------------------------------
void DryerFan_DobrNrST::_customConnectToGUI( QObject *guiItem,  QObject *propWin )
{
    OneEngRouteFCUnit::_customConnectToGUI( guiItem,  propWin );
    connect( _T,      SIGNAL( s_valueChd( QVariant ) ),           guiItem, SLOT( setTemp( QVariant ) ),        Qt::QueuedConnection );
    connect( _setT,   SIGNAL( s_valueChd( QVariant ) ),           guiItem, SLOT( setLimitTemp( QVariant ) ),   Qt::QueuedConnection );
    connect( _setT,   SIGNAL( s_valueChd( QVariant ) ),           _T,      SLOT( setDetectLevel( QVariant ) ), Qt::QueuedConnection );
    connect( guiItem, SIGNAL( limitTempChanged ( QVariant ) ), _setT,   SLOT( setValue( QVariant ) ),       Qt::QueuedConnection );
}

//-----------------------------------------------------------------------------------

