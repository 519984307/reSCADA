#ifndef SCADAENUMS_H
#define SCADAENUMS_H
#include <QObject>

namespace Prom {
Q_NAMESPACE

const quint8 OUT_LIM_NO   = 0b0;//0000 0000 Тег не лимитирован
const quint8 OUT_LIM_MIN  = 0b01;//0000 0001 Тег лимитирован на максимум
const quint8 OUT_LIM_MAX  = 0b10;//0000 0010 Тег лимитирован на минимум

const char ALL_8OPT_ON = 0b11111111;

namespace EEopt {
const  char KM_ALARM_ON =     0b00000001; // шестнадцатеричный литерал для 0000 0001
//const  char kI =     0b00000010;
//const  char kD =     0b00000100;
//const  char allImp = 0b00111000;
//const  char kPimp  = 0b00001000;
//const  char kIimp  = 0b00010000;
//const  char kDimp  = 0b00100000;
//const  char manImp = 0b01000000;
//const  char impIn  = 0b10000000;
//  option8 = 0b10000000
//};
}

enum UnitModes {
    UnMdNoDef = 0,
    UnMdCantHaveMode,
    UnMdStop,
    UnMdCollingStop,
    UnMdStart,
    UnMdCleanStop,
    UnMdForward,
    UnMdBackward,
    UnMd1pos,
    UnMd2pos,
    UnMdManualStarted,
    UnMdFull,
    UnMdMidle,
    UnMdLow,
    UnMdEmpty,
    UnMdSaveMode,
    UnMdNoriaPropStart,
    UnMdCleanStart,
    UnMdFreeze,
    UnMdColling,
    UnMdSmoke,
    UnMdFire,
    UnMdNeedLoad,
    UnMdNeedUnload,
    UnMdStorage,
    UnMdStopAndWait,
    UnMdPartStarted,
    UnMdAutoStart,
    UnMdWarmingUp,
    UnMdCollingStoped

};
Q_ENUM_NS( UnitModes )

enum UnitStates {
    UnStNoDef = 0,
    UnStNotConnected,

    UnStManualStarted,
    UnStStopCommand,
    UnStStartCommand,
    UnStStarted,
    UnStCleanStarted,
    UnStCleanStoped,

    UnStForwardStopCommand,
    UnStBackwardStopCommand,
    UnStStoped,
    UnStForwardCommand,
    UnStForward,
    UnStManualForward,
    UnStBackwardCommand,
    UnStBackward,
    UnStManualBackward,

    UnStManualMove, //реверсивный двигатель с 1 тэгом от 2х контакторов,
    //когда тэг от контакторов говорит о замыкании и куда что крутится не понять

    UnSt1posCommand,
    UnSt1posMove,
    UnSt1posManual,
    UnSt1pos,
    UnSt1posStopCommand,
    UnSt2posCommand,
    UnSt2posMove,
    UnSt2posManual,
    UnSt2pos,
    UnSt2posStopCommand,
    UnStMidlle,

    UnStClean,
    UnStCleaned,
    UnStCollingCommand,
    UnStColling,
    UnStCollingStoped,
    UnStCollingStopCommand,

    UnStHiLvl,
    UnStMidLvl,
    UnStLowLvl,
    UnStEmpty
};
Q_ENUM_NS(UnitStates)

enum EngineStates {
    EngAlarm,
    EngStoped,
    EngNoDef,
    EngForvard,
    EngBackward,
    EngToStopForward,
    EngToStopBackward,
    EngToForvard,
    EngToBackward,
    EngManualMove,
    EngManualBackward,
    EngManualForward
};
Q_ENUM_NS(EngineStates)

enum MessType {
    MessChangeSensor = -3,
    MessChangeState = -2,
    MessInfo = -1,
    MessAlarm = 0,
    MessWarning,
    MessQuitAlarm,
    MessChangeCommand,
    MessError,
    MessVerbose
};
Q_ENUM_NS(MessType)

enum UnitType {
    TypeSimpUnit,
    TypeNoDef,
    TypeTransporter,
    TypeNoria,
    TypeShnek, //шнюк
    TypeFan,
    TypeGate,
    TypeUnderGate,
    TypeValve,
    TypeFloodgate,
    TypeLocalFilter,
    TypeSilos,
    TypeSortMachine,
    TypePLC,
    TypeMMC,
    TypeDryer2VSM_DobrNr,
    TypeBurner,
    TyteDryerFan_VSMDbr,
    TypeDryerUnloadTable,
    TypeDryerFan,
    TyteDryerVSM3_Kromy,
    TypeDryer1_Bunker,
    TypeDryer3_Bunker,
    TypeCombustor,
    TypeDruer,
    TypeRectnColumn,
    TypePump,
    TypeCommonTags
};
Q_ENUM_NS(UnitType)

enum ESTagType {
    TpDiscretOut,
    TpDiscretIn,
    TpOut,
    TpIn
};
Q_ENUM_NS(ESTagType)

enum OutESTagSetType {
    PreSet,
    PostSet
};
Q_ENUM_NS(OutESTagSetType)

enum RouteCommand{
    RtCmNo = 0,
    RtCmClearCmList,
    RtCmToRoute,
    RtCmAlarm,
    RtCmToRoutForClean,
    RtCmCleanStart,
    RtCmCleanStop,
    RtCmStop,
    RtCmToSave,
    RtCmStopOnRouteAlarm
};
Q_ENUM_NS(RouteCommand)

enum RouteUnitInfo{
    RtUnNo,
    RtUnReject,
    RtUnDtKnowComm,
    RtUnNotInRoute,
    RtUnAlarm,
    RtUnOutCycle
};
Q_ENUM_NS(RouteUnitInfo)

enum SetModeResp{
    RejNoCond = -3,
    RejTransPr = - 2,
    RejAlarm = -1,
    RejAnnown = 0, //Не менять номер!!
    DoneAlready = 1,
    DoneWhait = 2,
};
Q_ENUM_NS(SetModeResp)

enum ETagValConv{
    VCNo,
    VCdiv10, //Число с плавающей точкой закодировано в целом числе - последний разряд десятые доли (256 = 25,6), преобразуем в обычное с плавающей точкой
    VCdiv100,
    VCdiv1000,
    VCmul100
};
Q_ENUM_NS(ETagValConv)

//Флаги инициализации электродвигателя
//const unsigned char EF_START       = 0x01;/*0000 0001*//*! Двигатель НЕ реверсивный*/
//const unsigned char EF_FWD_BWD     = 0x02;/*0000 0010*//*! Двигатель реверсивный*/
//const unsigned char EF_1KM         = 0x04;/*0000 0100*//*! Двигатель с 1-м тэгом от Контактора (либо не реверс-й двиг, либо реверс-й с объединенным тэгом от 2-х Контакторов)*/
//const unsigned char EF_2KM         = 0x08;/*0000 1000*//*! Ревер-й двигатель с отдельными тэгами от обоих Контакторов*/
//const unsigned char EF_QK          = 0x10;/*0001 0000*//*! У двигателя есть автомат защиты */
//const unsigned char EF_STPBTN      = 0x20;/*0010 0000*//*! У двигателя есть стоповая кнопка */
//const unsigned char EF_INV_STPBTN  = 0x40;/*0100 0000*//*! У двигателя есть стоповая кнопка */
//const unsigned char EF_INV_QK      = 0x80;/*1000 0000*//*! У двигателя есть стоповая кнопка */

const std::vector <QString> EF_S             = { "Пуск|.start",   "",             "",            "" };
const std::vector <QString> EF_S1KM          = { "Пуск|.start",   "",             "Контактор|.KM|1",                    "" };
const std::vector <QString> EF_S1KMplQ       = { "Пуск|.start",   "",             "Контактор + Защитный автомат|.KM|1", "" };
const std::vector <QString> EF_S1KMQ         = { "Пуск|.start",   "",             "Контактор|.KM|1",                    "", "Защитный автомат|.QK|1" };
const std::vector <QString> EF_S1KMQF        = { "Пуск|.start",   "",             "Контактор|.KM|1",                    "", "Защитный автомат|.QF|1" };
const std::vector <QString> EF_S1KMq         = { "Пуск|.start",   "",             "Контактор|.KM|1",                    "", "Защитный автомат|.QK|0" };
const std::vector <QString> EF_S1KMqf        = { "Пуск|.start",   "",             "Контактор|.KM|1",                    "", "Защитный автомат|.QF|0" };
const std::vector <QString> EF_S1KMQS        = { "Пуск|.start",   "",             "Контактор|.KM|1",                    "", "Защитный автомат|.QK|1", "Аварийный стоп|.AS|1" };
const std::vector <QString> EF_S1KMQFS       = { "Пуск|.start",   "",             "Контактор|.KM|1",                    "", "Защитный автомат|.QF|1", "Аварийный стоп|.AS|1" };
const std::vector <QString> EF_Sl1SdlQF      = { "Пуск|.startLG",   "",           "Работа|.startedLG|1",                "", "Защитный автомат|.QF|1" };
const std::vector <QString> EF_S1KMQs        = { "Пуск|.start",   "",             "Контактор|.KM|1",                    "", "Защитный автомат|.QK|1", "Аварийный стоп|.AS|0" };
const std::vector <QString> EF_S1KMS         = { "Пуск|.start",   "",             "Контактор|.KM|1",                    "", "",                       "Аварийный стоп|.AS|1" };
const std::vector <QString> EF_SSp1KMQ       = { "Пуск|.start",   "Стоп|.stop",   "Контактор|.KM|1",                    "", "Защитный автомат|.QK|1" };
const std::vector <QString> EF_SSp1KMQF      = { "Пуск|.start",   "Стоп|.stop",   "Контактор|.KM|1",                    "", "Защитный автомат|.QF|1" };
const std::vector <QString> EF_SSp1KMq       = { "Пуск|.start",   "Стоп|.stop",   "Контактор|.KM|1",                    "", "Защитный автомат|.QK|0" };
const std::vector <QString> EF_SiSpi1KMq     = { "Пуск|.start|i", "Стоп|.stop|i", "Контактор|.KM|1",                    "", "Защитный автомат|.QK|0" };
const std::vector <QString> EF_SSp           = { "Пуск|.start",   "Стоп|.stop", ""};
const std::vector <QString> EF_SSpi          = { "Пуск|.start",   "Стоп|.stop|i",   ""};
const std::vector <QString> EF_SSp1Q         = { "Пуск|.start",   "Стоп|.stop",   "",                                   "", "Защитный автомат|.QK|1" };

const std::vector <QString> EF_REV0KM        = { "Вперёд|.forward",           "Назад|.backward",           "",                              "" };
const std::vector <QString> EF_REV1KM        = { "Вперёд|.forward",           "Назад|.backward",           "Контактор|.KM|1",               "" };
const std::vector <QString> EF_REV2KM        = { "Вперёд|.forward",           "Назад|.backward",           ".KMforward|Контактор вперёд|1", ".KMbackward|Контактор назад|1" };

const std::vector <QString> EF_VLV_VSM_1KMQ  = { "Упр. вых. поз. 1|.pos1",   "Упр. вых. поз. 2|.pos2",    "Контактор|.KM|1",               "",                              "Защитный автомат|.QK|1" };
const std::vector <QString> EF_VLV_VSM_2KMQ  = { "Упр. вых. поз. 1|.pos1",   "Упр. вых. поз. 2|.pos2",    "Контактор поз.1|.KMpos1|1",     "Контактор поз.2|.KMpos2|1",     "Защитный автомат|.QK|1" };
const std::vector <QString> EF_VLV_VSM_2KMQF = { "Упр. вых. поз. 1|.pos1",   "Упр. вых. поз. 2|.pos2",    "Контактор поз.1|.KMpos1|1",     "Контактор поз.2|.KMpos2|1",     "Защитный автомат|.QF|1" };

const std::vector <QString> EF_VLV_VSM_0KMQS = { "Упр. вых. поз. 1|.pos1",   "Упр. вых. поз. 2|.pos2",    "",                              "",                              "Защитный автомат|.QK|1", "Аварийный стоп|.AS|1" };
const std::vector <QString> EF_VLV_VSM_0KMQ  = { "Упр. вых. поз. 1|.pos1",   "Упр. вых. поз. 2|.pos2",    "",                              "",                              "Защитный автомат|.QK|1" };

const std::vector <QString> EF_VLV_OW_0KM    = { "Упр. вых. поз. 1|.pos1",   "Упр. вых. поз. 2|.pos2",    "",                              "" };

const std::vector <QString> EF_GT_VSM_2KMQ   = { "Упр. вых. открытия|.open", "Упр. вых. закрытия|.close", "Контактор открытия|.KMopen|1",  "Контактор закрытия|.KMclose|1", "Защитный автомат|.QK|1" };
const std::vector <QString> EF_GT_VSM_2KMQF  = { "Упр. вых. открытия|.open", "Упр. вых. закрытия|.close", "Контактор открытия|.KMopen|1",  "Контактор закрытия|.KMclose|1", "Защитный автомат|.QF|1" };

const std::vector <QString> EF_GT_VSM_1KMQ   = { "Упр. вых. открытия|.open", "Упр. вых. закрытия|.close", "Контактор|.KM|1",               "",                              "Защитный автомат|.QK|1" };
const std::vector <QString> EF_GT_VSM_0KMQ   = { "Упр. вых. открытия|.open", "Упр. вых. закрытия|.close", "",                              "",                              "Защитный автомат|.QK|1" };
const std::vector <QString> EF_GT_VSM_0KMQS  = { "Упр. вых. открытия|.open", "Упр. вых. закрытия|.close", "",                              "",                              "Защитный автомат|.QK|1", "Аварийный стоп|.AS|1" };
const std::vector <QString> EF_GT_OW_0KMQi   = { "Упр. вых. открытия|.open", "Упр. вых. закрытия|.close", "",                              "",                              "Защитный автомат|.QK|0" };



#define TSP_ENUMS
//TSP ENUMS BEGIN
enum Quality{
    NotValid = -3,
    Check = -2,
    Unknown = -1,
    Bad = 0,
    Good = 1
};

enum Access{
    RW = 0,//read & write
    RO,    //read only
    WO,    //write only
    NA     //not avaible
};

enum Virtualization{
    No = 0,
    VValue,
    VRandom,
    VBitTag,
    VDWordTag,
    VQWordTag
};

enum DataType{
    TBool = 0,
    TInt,
    TFloat
};

enum DriverType {
    modbusTCP,
    modbusRTU,
    simatic
};

QString qualityToString(Quality quality);
QString messToString (Prom::MessType t);
QString stateToString (Prom::UnitStates s);
QString modeToString(Prom::UnitModes m);
Prom::UnitModes unitModeFromInt(int Mode);
QString driverTypeFromInt(int Type);
Prom::Access accessFromInt(int Val);
Prom::Virtualization virtualizationFromInt(int Val);
QString dataTypeFromInt(int Val);
bool icvalModes(UnitModes TrgMd, UnitModes ChckMd);
bool modeIn(UnitModes TrgMd, QVector<UnitModes>ChckMds);
bool modeNotIn(UnitModes TrgMd, QVector<UnitModes>ChckMds);
//void registerSCADAenumsInQML();;
}

class Driver;
class Group;
class Tag;
struct DriverConfig{
    int id;
    QString  name;
    QString type;
    QString options;
    QString comment;
    bool modified = false;
    Driver * driver = nullptr;
    QString errorString = "";
};
struct GroupConfig{
    int id;
    QString name;
    QString options;
    int delay;
    int driverId;
    QString comment;
    bool modified = false;
    Group * group = nullptr;
    QString errorString = "";
};
struct TagConfig{
    int id;
    QString name;
    QString address;
    QString options;
    int groupId;
    QString comment;
    bool modified = false;
    Tag * tag = nullptr;
    QString errorString = "";
};
//TSP ENUMS END


// за счет Q_NAMESPACE Prom отпала надобность в регистрации перечислений по отдельности
//Q_DECLARE_METATYPE(Prom::MessType);
//Q_DECLARE_METATYPE(Prom::UnitModes);
//Q_DECLARE_METATYPE(Prom::RouteCommand);
//Q_DECLARE_METATYPE(Prom::RouteUnitInfo);

//Q_DECLARE_METATYPE(Prom::ETagValConv);
//Q_DECLARE_METATYPE(Prom::ESTagType);
#endif // SCADAENUMS_H
