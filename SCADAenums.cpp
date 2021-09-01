#include "SCADAenums.h"

//using namespace Prom;
//------------------------------------------------------------------------------
QString Prom::messToString (MessType MessTypeInt)
{
    switch (MessTypeInt) {
    case MessAlarm         : return "Авария";
    case MessQuitAlarm     : return "Авария во время работы";
    case MessChangeCommand : return "Команда";
    case MessChangeState   : return "Изменение состояния";
    case MessChangeSensor  : return "Изменение сенсора";
    case MessInfo          : return "Информирование";
    case MessError         : return "Ошибка";
    case MessVerbose       : return "Отладочная информация";
    case MessWarning       : return "Потенциально опасно";
    }
    return "Тип сообщения не опознан";
}
//------------------------------------------------------------------------------
QString Prom::stateToString(UnitStates UnitStateInt)
{
    switch (UnitStateInt) {
    case UnStNoDef               : return "Не определено";
    case UnStNotConnected        : return "Нет соединения";
    case UnStForwardStopCommand  : return "Остановка вращения вперед";
    case UnStBackwardStopCommand : return "Остановка вращения назад";
    case UnStStoped              : return "Остановлен";
    case UnStForwardCommand      : return "Запуск вращения вперед";
    case UnStForward             : return "Запущено вращение вперед";
    case UnStManualForward       : return "Запущено вращение вперед в ручном режиме";
    case UnStBackwardCommand     : return "Запуск вращения назад";
    case UnStBackward            : return "Запущено вращение назад";
    case UnStManualBackward      : return "Запущено вращение назад в ручном режиме";
    case UnStManualMove          : return "Запущено вращение в ручном режиме в неизвестном направлении (т.к. контактор один на 2 упр. тэга)";
    case UnStManualStarted       : return "Запуск в ручном режиме";
    case UnStStopCommand         : return "Остановка";
    case UnStStartCommand        : return "Запуск";
    case UnStStarted             : return "Запущен";
    case UnSt1posCommand         : return "Запуск перемещения в позицию 1 (Открытие)";
    case UnSt1posMove            : return "Перемещение в позицию 1 (Открытие)";
    case UnSt1posManual          : return "Перемещение в позицию 1 (Открытие) вручную";
    case UnSt1pos                : return "Позиция 1 (Открыто)";
    case UnSt1posStopCommand     : return "Остановка перемещения в позицию 1 (Открытие)";
    case UnSt2posCommand         : return "Запуск перемещения в позицию 2 (Закрытие)";
    case UnSt2posMove            : return "Перемещение в позицию 2 (Закрытие)";
    case UnSt2posManual          : return "Перемещение в позицию 2 (Закрытие) вручную";
    case UnSt2pos                : return "Позиция 2 (Закрыто)";
    case UnSt2posStopCommand     : return "Остановка перемещения в позицию 2 (Закрытие)";
    case UnStMidlle              : return "Между 1 и 2 позицией (Среднее положение)";
    case UnStCleanStarted        : return "Запущен без продукта";
    case UnStCleanStoped         : return "Остановлен без продукта";
    case UnStClean               : return "Выполняется чистка";
    case UnStCleaned             : return "Чистка выполнена";
    case UnStCollingCommand      : return "Подготовка к хлаждению";
    case UnStColling             : return "Охлаждение";
    case UnStCollingStoped       : return "Остановлен с хлаждением";
    case UnStCollingStopCommand  : return "Подготовка к становке с хлаждением";
    case UnStHiLvl               : return "Верхний уровень";
    case UnStMidLvl              : return "Средний уровень";
    case UnStLowLvl              : return "Нижний уровень";
    case UnStEmpty               : return "Пусто";
    }
    return "Тип состояния не опознан";
}

//------------------------------------------------------------------------------
QString Prom::modeToString(Prom::UnitModes UnitModeInt)
{
    switch (UnitModeInt) {
    case UnMdNoDef    : return "Не определено";
    case UnMdStop     : return "Остановить";
    case UnMdCollingStop: return "Остановлен с охлаждением";
    case UnMdStart    : return "Запустить";
    case UnMdCleanStart : return "Очистка";
    case UnMdCleanStop: return "Остановить после чистки";
    case UnMdForward  : return "Запустить вперед";
    case UnMdBackward : return "Запустить назад";
    case UnMd1pos     : return "Переместить в поз. 1";
    case UnMd2pos     : return "Переместить в поз. 2";
    case UnMdFull     : return "Верхний уровень";
    case UnMdMidle    : return "Средний уровень";
    case UnMdLow      : return "Нижний уровень";
    case UnMdEmpty    : return "Пустой";
    case UnMdManualStarted : return "Ручной или внешний запуск";
    case UnMdNoriaPropStart : return "Нория в режиме очистки подпора";
    case UnMdSaveMode : return "Безопасный режим";
    case UnMdFreeze   : return "Аварийно остановить";
    case UnMdColling  : return "Охлаждение";
    case UnMdSmoke    : return "Дым";
    case UnMdFire     : return "Пожар";
    case UnMdNeedLoad : return "Нужна загрузка";
    case UnMdNeedUnload : return "Нужна выгрузка";
    case UnMdStorage  : return "Хранение";
    case UnMdStopAndWait  : return "Стоп до разрешения";
    case UnMdPartStarted  : return "Запущен частично";
    case UnMdAutoStart  : return "Запущен в автоматическом режиме";
    }
    return "";
}

//------------------------------------------------------------------------------
Prom::UnitModes Prom::unitModeFromInt(int UnitModeInt)
{
    switch (UnitModeInt) {
    case  UnMdNoDef      : return UnMdNoDef      ;
    case  UnMdStop       : return UnMdStop       ;
    case  UnMdCollingStop : return UnMdCollingStop  ;
    case  UnMdStart      : return UnMdStart      ;
    case  UnMdCleanStart : return UnMdCleanStart ;
    case  UnMdCleanStop  : return UnMdCleanStop  ;
    case  UnMdForward    : return UnMdForward    ;
    case  UnMdBackward   : return UnMdBackward   ;
    case  UnMd1pos       : return UnMd1pos       ;
    case  UnMd2pos       : return UnMd2pos       ;
    case  UnMdFull       : return UnMdFull       ;
    case  UnMdMidle      : return UnMdMidle      ;
    case  UnMdLow        : return UnMdLow        ;
    case  UnMdEmpty      : return UnMdEmpty      ;
    case UnMdManualStarted  : return UnMdManualStarted ;
    case UnMdNoriaPropStart : return UnMdNoriaPropStart;
    case UnMdSaveMode       : return UnMdSaveMode      ;
    case UnMdFreeze         : return UnMdFreeze        ;
    case UnMdColling        : return UnMdColling       ;
    case UnMdSmoke          : return UnMdSmoke         ;
    case UnMdFire           : return UnMdFire          ;
    case UnMdNeedLoad       : return UnMdNeedLoad      ;
    case UnMdNeedUnload     : return UnMdNeedUnload    ;
    case UnMdStorage        : return UnMdStorage       ;
    }
    return UnMdNoDef;
}

//------------------------------------------------------------------------------
QString Prom::driverTypeFromInt(int IntType)
{
    switch (IntType) {
    case  modbusRTU : return "modbusRTU";
    case  modbusTCP : return "modbusTCP";
    case  simatic   : return "simatic";
    default: return "modbusTCP";
    }
}
//------------------------------------------------------------------------------
Prom::Access Prom::accessFromInt(int AccessInt)
{
    switch (AccessInt) {
    case  RW : return RW;
    case  RO : return RO;
    case  WO   : return WO  ;
    default: return RW;
    }
}
//------------------------------------------------------------------------------
Prom::Virtualization Prom::virtualizationFromInt(int VirtInt)
{
    switch (VirtInt) {
    case VValue    : return VValue   ;
    case VRandom   : return VRandom  ;
    case VBitTag   : return VBitTag  ;
    case VDWordTag : return VDWordTag;
    case VQWordTag : return VQWordTag;
    default: return No;

    }
}
//------------------------------------------------------------------------------
QString Prom::dataTypeFromInt(int DataTypeInt)
{
    switch (DataTypeInt) {
    case TBool    : return "bool"   ;
    case TInt     : return "int"    ;
    case TFloat   : return "float"  ;
    default: return "int";
    }
}
//------------------------------------------------------------------------------
bool Prom::icvalModes(Prom::UnitModes TrgMd, Prom::UnitModes ChckMd)
{
    switch (TrgMd) {
    case UnMdStop  : if(ChckMd == UnMdCleanStop || ChckMd == UnMdCollingStop) return true; break;
    case UnMdStart : if(ChckMd == UnMdCleanStart) return true; break;
    case UnMdMidle : if(ChckMd == UnMd1pos) return true; break;
    default: return TrgMd == ChckMd;
    }
    return TrgMd == ChckMd;
}
//------------------------------------------------------------------------------

QString Prom::qualityToString(Quality quality)
{
    switch(quality){
    case Check:     return "Check";
    case Unknown:   return "Unknown";
    case Bad:       return "Bad";
    case Good:      return "Good";
    default:        return "<quality>";
    }
}

//------------------------------------------------------------------------------
bool Prom::modeIn(Prom::UnitModes TrgMd, QVector<Prom::UnitModes> ChckMds)
{
    return ChckMds.indexOf( TrgMd ) >= 0;
}

//------------------------------------------------------------------------------

bool Prom::modeNotIn(Prom::UnitModes TrgMd, QVector<Prom::UnitModes> ChckMds)
{
    return ChckMds.indexOf( TrgMd ) < 0;
}
