#ifndef ENUMS_H
#define ENUMS_H

#include <QObject>

#ifndef SCADAENUMS_H
#include "SCADAenums.h"
#endif // SCADAENUMS_H
//enums

namespace tsp_enums {

using namespace Prom;

#ifndef TSP_ENUMS
#define TSP_ENUMS

enum Quality{Check = -2,
             Unknown = -1,
             Bad = 0,
             Good};

enum Access{RW,
            RO,
            WO,
            NA};

enum Virtualization{No,
                    VValue,
                    VRandom,
                    VBitTag,
                    VDWordTag,
                    VQWordTag};

enum DataType{TBool,
              TInt,
              TFloat};

enum MessType {
    MessError,
    MessInfo,
    MessVerbose
};

inline QString qualityToString(Quality quality)
{
    switch(quality){
    case Check:     return "Check";
    case Unknown:   return "Unknown";
    case Bad:       return "Bad";
    case Good:      return "Good";
    default:        return "<quality>";
    }
}
#endif // TSP_ENUMS
}
#ifndef TSP_ENUMS
Q_DECLARE_METATYPE(tsp_enums::MessType);
#endif // TSP_ENUMS

#endif // ENUMS_H
