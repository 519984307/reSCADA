#include <qdebug.h>
#include "Noria.h"
#include "SCADAenums.h"
#include "ETag.h"

Noria::Noria( int ID,
              QString Name,
              QString TagPefix,
              std::vector<QString> EngSensorNames  )

    : OneEngRouteUnit(  Prom::TypeNoria,
                        ID,
                        Name,
                        TagPefix,
                        true,
                        EngSensorNames )
{

}

//------------------------------------------------------------------------------
Prom::UnitModes Noria::_ModeOfCommand(Prom::RouteCommand *Command)
{
    using namespace Prom;
    switch ( *Command ) {
    case RtCmNo               : return UnMdNoDef;
    case RtCmCleanStop        : return UnMdCleanStop;
    case RtCmStop             : return UnMdStop;
    case RtCmToSave           : return saveMode;
    case RtCmToRoute          :
    case RtCmToRoutForClean   :
    case RtCmCleanStart       : return _routeMode;
    case RtCmStopOnRouteAlarm :
        if( currentMode() == Prom::UnMdStart )
            return Prom::UnMdStart;
        else
            return saveMode;
    default: return UnMdNoDef;
    }
}

//------------------------------------------------------------------------------
void Noria::_alarmDo()
{
    foreach (ETag *tg, _tags) {
        if( tg->getDBName() != ".PS" && tg->isAlarm() ){
            setMode( Prom::UnMdStop, false );
        }
    }
    _routeCommand =  Prom::RtCmNo;
    emit InformToRoute( this, Prom::RtCmNo, Prom::RtUnAlarm );
}

//------------------------------------------------------------------------------
void Noria::_customConnectToGUI( QObject *guiItem,  QObject *propWin)
{
    EngRouteUnit::_customConnectToGUI( guiItem, propWin );
}
//-----------------------------------------------------------------------------------



