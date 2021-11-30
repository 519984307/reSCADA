#include "UnderGate.h"

UnderGate::UnderGate( int ID,
                      QString Name,
                      QString TagPefix,
                      std::vector<QString> EngSensorNames,
                      bool NOnotNCSensors )
    : Static2posUnit( Prom::TypeUnderGate,
                      ID,
                      Name,
                      TagPefix,
                      Prom::UnMd2pos,
                      EngSensorNames,
                      NOnotNCSensors )
{

}

//-----------------------------------------------------------------------------------
Prom::UnitModes UnderGate::_ModeOfCommand( Prom::RouteCommand *Command )
{
    using namespace Prom;
    switch ( *Command ) {
    case RtCmNo               : return UnMdNoDef;
    case RtCmStop             : return UnMdStop;
    case RtCmToRoute          :
    case RtCmCleanStart       : return _routeMode;
    case RtCmToRoutForClean   :
    case RtCmToSave           :
    case RtCmCleanStop        :
    case RtCmStopOnRouteAlarm : return saveMode;
    default: return UnMdNoDef;
    }
}

//-----------------------------------------------------------------------------------
