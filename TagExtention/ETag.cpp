#include "ETag.h"
#include <qsettings.h>
#include "unit.h"
#include "promobject.h"
#include "TSP/tsp.h"
#include <QDebug>
#include <cmath>

using Prom::MessType;
//using Prom::PropType;
//extern QObject * Ob;

ETag::ETag(Unit * Owner,
    Prom::ESTagType Type,
    QString Name,
    QString DBName,
    bool TunableSetTime,
    bool TunablePulseTime,
    bool EgnorableAlarm,
    bool InGUI,
    Prom::ETagValConv Convertion,
    QVariant ChageStep)
    : QObject(Owner),
    ttype(Type),
    tunableSetTime(TunableSetTime),
    tunablePulseTime (TunablePulseTime),
    ignorableAlarm(EgnorableAlarm),
    inGUI(InGUI),
    _changeStep(ChageStep),
    _owner(Owner),
    _name(Name),
    _DBName(DBName),
    _conv(Convertion)

{
    extern TSP * g_TSP;

    setObjectName(Name);
    _setTimer = new QTimerExt(this);
    _setTimer->setSingleShot(true);
    connect(_setTimer, &QTimer::timeout, this, &ETag::setTimerEnd, Qt::QueuedConnection);

    _pulseTimer = new QTimerExt(this);
    _pulseTimer->setSingleShot(true);
    connect(_pulseTimer, &QTimer::timeout, this, &ETag::pulseTimerEnd);

    connect(this, &ETag::s_logging, _owner, &Unit::logging, Qt::QueuedConnection);
    if(_owner->tagPrefix != "" && _DBName != ""){
        if(g_TSP != nullptr)
            _tag = g_TSP->getTagByName(_owner->tagPrefix + _DBName);
        if(_tag != nullptr){
            connect(_tag, &Tag::s_onQualityChanged, this, &ETag::_qualityChangedSlot, Qt::QueuedConnection);
            connect(_tag, &Tag::s_onValueChanged, this, &ETag::_acceptValue, Qt::QueuedConnection);
            _ok = true;
            _logging (Prom::MessVerbose, "тэг " + _owner->tagPrefix + _DBName + " загружен", false);
        }
        else {
            _owner->setAlarmTag(_owner->tagPrefix + _DBName);
            _alarmStr = _owner->tagPrefix + _DBName + " тэг в TSP отсутствует";
            _logging (Prom::MessAlarm, _alarmStr, false);
        }
    }
    else {
        _owner->setAlarmTag(_name);
        _alarmStr = _name + " пустй адрес тэга";
        _logging (Prom::MessAlarm, _alarmStr, false);
    }
    _owner->addETag(this);
    _timeLog.setInterval(60000);
    connect(&_timeLog, &QTimer::timeout, this, &ETag::_logValChange);
}

//------------------------------------------------------------------------------
int ETag::getSetTime() const
{
    return _setTimer->interval();
}

//------------------------------------------------------------------------------
QVariant ETag::value()
{
    return _imit ? _imitVal : _value;
}

//------------------------------------------------------------------------------
bool ETag::connected()
{
    if(_imit) return true;
    if(_ok )
        return _tag->readQuality() > 0;
    return false;
}

//------------------------------------------------------------------------------
void ETag::reInitialise()
{

    if(_owner && _owner->sensorsConnected() != connected() )
        emit s_qualityChd(connected());
    emit s_valueChd(value());
    emit s_imitationChd(_imit);
    emit s_imitationValueChd(_imitVal);
    emit s_ignorAlarmChd(_ignorAlarm);
    emit s_setDelayChd(_setTimer->interval() / 1000);
    emit s_pulseDelayChd(_pulseTimer->interval() / 1000);
    if(_alarm)
        emit s_alarm("повторный сигнала аварии, при принудительном обновлении состояния");
    else
        emit s_alarmReseted();
    emit s_liveValueChd(_value);
}
//------------------------------------------------------------------------------
QString ETag::fullTagName() {
    if(_ok)
        return _tag->objectName() ;
    else return "";
}

//------------------------------------------------------------------------------
void ETag::_logging(Prom::MessType MessType, QString Discription, bool imitation)
{
    emit s_logging(MessType, QDateTime::currentDateTime(), false, objectName(), Discription + (imitation ? " (имитация)" : ""));
}

//------------------------------------------------------------------------------
void ETag::_qualityChangedSlot()
{
    if(_tag->readQuality() == Prom::Good){
        emit s_qualityChd(true);
        _logging (Prom::MessAlarm, "соединение восстановлено", false);
        //reInitialise(); незагруженные теги проямляли неопределённость при имитации но пофиг на них: они ж не загруженные
        if(!_imit){
            _acceptValue(_tag->getValue());
            _logValChange();
            _timeLog.start();
        }
    }
    else  {
        emit s_qualityChd(false);
        if(_imit){
            _logging (Prom::MessVerbose, "соединение разорванно", true);
        }
        else{
            _logValChange();
            _timeLog.stop();
            _alarm = true;
            emit s_alarm("соединение разорванно");
        }

    }
    //    Logging (Prom::MessVerbose, "смена состояния соединения на " + QString::number(_tag->ReadQuality()), false);
}

//------------------------------------------------------------------------------
void ETag::_acceptValue(QVariant Val)
{
    //static QVariant Val;
    //Val = _tag->ReadValue();
    switch (_conv) {
    case Prom::VCNo: break;
    case Prom::VCdiv10 :
        Val = Val.toDouble() / 10;
        break;
    case Prom::VCdiv1000 :
        Val = Val.toDouble() / 1000;
        break;
    }

    //if(_value != Val.toDouble()){
    if( ! _imit )
        _preValue = _value;
    _value = Val.toDouble();
    if( std::fabs( _value.toDouble() - _preValue.toDouble() ) >= _changeStep.toDouble() ){
        _logValChange();
    }
    if(! _imit){
        if(_pulse){
            _checkPulse();
        }
        else{
            _checkVal();
        }
    }
    // }
    emit s_liveValueChd(Val);

}
//------------------------------------------------------------------------------
void ETag::_logValChange()
{
    _logging(Prom::MessInfo, "значение " + QString::number(_value.toDouble()), false);
}

//------------------------------------------------------------------------------
void ETag::setTimerStart()
{
    _setTimer->stop();
    if(_setTimer->interval() > 0){
        _setTimer->start();
        _logging(Prom::MessVerbose, "старт таймера установки требуемого состояния ("
                + QString::number(_setTimer->interval() / 1000) + "с.)" , false);
    }
}

//------------------------------------------------------------------------------
void ETag::saveParam()
{
    if(_owner->ini){
        _owner->ini->setValue(_owner->tagPrefix+ "/" + _DBName + ".imit", _imit);
        _owner->ini->setValue(_owner->tagPrefix+ "/" + _DBName + ".imitVal", _imitVal.toDouble());
        _owner->ini->setValue(_owner->tagPrefix+ "/" + _DBName + ".ignorAlarm", _ignorAlarm);
        if(_setTimer->interval() > 0)
            _owner->ini->setValue(_owner->tagPrefix+ "/" + _DBName + ".setTime", _setTimer->interval());
        if(_pulseDuration > 0)
            _owner->ini->setValue(_owner->tagPrefix+ "/" + _DBName + ".pulseTime", _pulseDuration);
    }
}
//------------------------------------------------------------------------------
void ETag::loadParam()
{
    //Если ini файл есть и есть запись по тегу, то загружаю
    if( _owner->ini && _owner->ini->contains(_owner->tagPrefix+ "/" + _DBName + ".ignorAlarm")){
        _ignorAlarm =             _owner->ini->value(_owner->tagPrefix+ "/" + _DBName + ".ignorAlarm", false).toBool();
        _setTimer->setInterval(  _owner->ini->value(_owner->tagPrefix+ "/" + _DBName + ".setTime", tunableSetTime ? 2000 : 0).toInt());
        _pulseDuration = _owner->ini->value(_owner->tagPrefix+ "/" + _DBName + ".pulseTime", tunablePulseTime ? 2000 : 0).toInt();
        _pulseTimer->setInterval(_pulseDuration);
        writeImit(_owner->ini->value(_owner->tagPrefix+ "/" + _DBName + ".imit", false).toBool());
        writeImitVal(_owner->ini->value(_owner->tagPrefix+ "/" + _DBName + ".imitVal", 0).toDouble());
    }
    //rescan();
}
//------------------------------------------------------------------------------
bool ETag::resetAlarm()
{
    //_alarmSetTime = false;
    if( _mayResetAlarm ){
        if( _alarm ){
            _logging(Prom::MessAlarm, "сброшена авария - " + _alarmStr, false);
            _alarm = false;
            _alarmStr = "";
        }
        emit s_alarmReseted();
        return true;
    }
    else if(_ignorAlarm) {
        _logging (Prom::MessAlarm, _alarmStr + " - авария сброшена по игнорированию", false);
        emit s_alarm("");
        //emit AlarmReseted();
        return true;
    }
    else {
        _logging(Prom::MessVerbose, _alarmStr + " - авария не сброшена", false);
        emit s_alarm("");
    }
    return false;
}
//------------------------------------------------------------------------------
void ETag::writeSetDelay(QVariant Sec)
{
    _setTimer->setInterval(Sec.toUInt() * 1000);
    _logging(Prom::MessInfo, "установлена задержка срабатывания - " + QString::number(Sec.toUInt()) + "сек.", false);
}

//------------------------------------------------------------------------------
void ETag::writeIgnorAlarm(bool ignor)
{
    if(_ignorAlarm != ignor){
        _ignorAlarm = ignor;
        _logging(Prom::MessAlarm, QString(ignor ? "включено" : "выключено") + "игнорирование аварии", false);
        if(_alarm && ! ignor)
            emit s_alarm(_alarmStr + " - восстановление аварии из-за отключения её игнорирования");
    }
}

//------------------------------------------------------------------------------
void ETag::setPulseDuration(QVariant Sec)
{
    _pulseDuration = Sec.toInt() * 1000;
    _pulseTimer->setInterval(_pulseDuration);
    _logging(Prom::MessInfo,"установлен интервал пульсации - " + QString::number(Sec.toInt()) + "сек.", false);
}

//------------------------------------------------------------------------------
void ETag::connectToGUI(QObject *guiItem, QObject *propWin)
{
    QVariant ret;
    QObject * tmpSgSt, * engRow;
    QMetaObject::invokeMethod(propWin, "addEngRow", Qt::DirectConnection, Q_RETURN_ARG(QVariant, ret), Q_ARG(QVariant, this->fullTagName())); //создал строку меню для тэга
    engRow = qvariant_cast< QObject* >(ret);

    if(engRow && guiItem) _customConnectToGUI(guiItem, engRow);

    if(! this->isOk()){
        QMetaObject::invokeMethod(propWin, "addAlarm", Qt::DirectConnection, Q_RETURN_ARG(QVariant, ret),
            Q_ARG(QVariant, this->fullTagName() + "_tag_alarm"),
            Q_ARG(QVariant, this->getName() + " - в TSP нет " + _owner->tagPrefix + _DBName ),
            Q_ARG(QVariant, false));
    }
    else{
        //!создаю строку аварии для тэга
        QMetaObject::invokeMethod(propWin, "addAlarm", Qt::DirectConnection, Q_RETURN_ARG(QVariant, ret),
            Q_ARG(QVariant, this->fullTagName() + "_alarm"),
            Q_ARG(QVariant, this->getName()),
            Q_ARG(QVariant, this->ignorableAlarm));
    }
    tmpSgSt = qvariant_cast< QObject* >(ret);
    //QObject * tmpSgSt = propWin->findChild<QObject*>(est->fullTagName() + "_alarm");                                                //получил указатеть на созданную строку
    //-----подключил сигналы к авирии и игнору
    connect(this,SIGNAL(s_alarmReseted()            ), tmpSgSt, SLOT(alarmReseted()               ), Qt::QueuedConnection);
    connect(this,SIGNAL(s_alarm(           QVariant )), tmpSgSt, SLOT(setAlarm()                 ), Qt::QueuedConnection);
    connect(tmpSgSt,SIGNAL(changedIgnore(   bool    )), this, SLOT(writeIgnorAlarm(bool    )), Qt::QueuedConnection);
    connect(this,SIGNAL(s_ignorAlarmChd(QVariant)), tmpSgSt, SLOT(changeIgnore(   QVariant)), Qt::QueuedConnection);
    connect(this,SIGNAL(s_qualityChd(  QVariant)), tmpSgSt, SLOT(changeConnected(QVariant)), Qt::QueuedConnection);
    //-----подключил сигналы к авирии и игнору
    //}

    if(tunablePulseTime) { //Для пульсирующих
        //!добавляю пульс
        QMetaObject::invokeMethod(engRow, "addPropertySetting", Qt::DirectConnection,
            Q_RETURN_ARG(QVariant, ret),
            Q_ARG(QVariant, this->getDBName() + "_pulseDelay" ),
            Q_ARG(QVariant, "сек. - интервал пульсации"),
            Q_ARG(QVariant, 1));
        //tmpSgSt = guiItem->findChild<QObject*>(est->getDBName() + "_pulseDelay");
        tmpSgSt = qvariant_cast< QObject* >(ret);//получаю указатель на задержку
        connect(tmpSgSt, SIGNAL(changedVal(QVariant     )), this, SLOT(setPulseDuration(QVariant)), Qt::QueuedConnection);
        connect(this, SIGNAL(s_pulseDelayChd(QVariant)), tmpSgSt, SLOT(changeVal(QVariant)), Qt::QueuedConnection);
        connect(this->_pulseTimer, SIGNAL(StartSig(QVariant)),   tmpSgSt, SLOT(startCountdown(QVariant)), Qt::QueuedConnection);
        connect(this->_pulseTimer, SIGNAL(StopSig() ), tmpSgSt, SLOT(stopCountdown()), Qt::QueuedConnection);
        connect(this->_pulseTimer, SIGNAL(timeout() ), tmpSgSt, SLOT(stopCountdown()), Qt::QueuedConnection);
    }

    if(this->tunableSetTime){
        //!добавляю задержку срабатывания
        QMetaObject::invokeMethod(engRow, "addPropertySetting", Qt::DirectConnection,
            Q_RETURN_ARG(QVariant, ret),
            Q_ARG(QVariant, this->getDBName() + "_delay" ),
            Q_ARG(QVariant, "сек. - задержка установки"),
            Q_ARG(QVariant, 1));
        //tmpSgSt = guiItem->findChild<QObject*>(est->getDBName() + "_delay");
        tmpSgSt = qvariant_cast< QObject* >(ret);//получаю указатель на задержку
        //подключаю сигналы к задержке
        connect(tmpSgSt, SIGNAL(changedVal(QVariant)), this,SLOT(writeSetDelay(QVariant)), Qt::QueuedConnection);
        connect(this,SIGNAL(s_setDelayChd(QVariant)), tmpSgSt, SLOT(changeVal(QVariant)    ), Qt::QueuedConnection);
        connect(this->_setTimer, SIGNAL(StartSig(QVariant)),   tmpSgSt, SLOT(startCountdown(QVariant)), Qt::QueuedConnection);
        connect(this->_setTimer, SIGNAL(StopSig() ), tmpSgSt, SLOT(stopCountdown()), Qt::QueuedConnection);
        connect(this->_setTimer, SIGNAL(timeout() ), tmpSgSt, SLOT(stopCountdown()), Qt::QueuedConnection);

        //подключаю сигналы к задержке
    }

    if(this->getDBName() == ".current"){ // Если токомер
        connect(this, SIGNAL(s_valueChd(QVariant)), propWin, SLOT(setCurrent(QVariant)), Qt::QueuedConnection);
    }

}

//------------------------------------------------------------------------------
