#include <qsettings.h>
#include <qdebug.h>

#include "SimpUnit.h"
#include "ETag.h"

#include "promobject.h"
#include "QTimerExt.h"

extern QSettings * g_unitINI;
//------------------------------------------------------------------------------
SimpUnit::SimpUnit( Prom::UnitType Type,
    int * Id,
    QString Name,
    QString TagPrefix,
    bool Mover,
    Prom::UnitModes SaveMode,
    QSettings * Ini)

    :Unit(Type,
        Id,
        Name,
        TagPrefix,
        Mover,
        SaveMode,
        Ini)
{

}

//------------------------------------------------------------------------------
