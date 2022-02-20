#include <QSettings>
#include "OutETag.h"
#include "unit.h"
#include <qdebug.h>

using Prom::MessType;

OutETag::OutETag(Unit * Owner,
    Prom::ESTagType Type,
    Prom::OutESTagSetType SetType,
    QString Name,
    QString DBName,
    bool TunableSetTime,
    bool TunablePulseTime,
    bool EgnorableAlarm,
    bool InGUI,
    Prom::ETagValConv Convertion,
    bool Save,
    bool LoadDefault,
    QVariant DefaultValue,
    bool MenuChanged,
    quint8   LimitFlag,
    QVariant HiLimit,
    QVariant LowLimit,
    bool TunableImpulseTime,
    QVariant ChageStep )
    : ETag(Owner,
        Type,
        Name,
        DBName,
        TunableSetTime,
        TunablePulseTime,
        EgnorableAlarm,
        InGUI,
        Convertion,
        ChageStep),
    saveValue(Save), loadDefalt(LoadDefault),
    tunableImpulseTime(TunableImpulseTime), _setType(SetType),
    _defaultValue(DefaultValue), _menuChanged(MenuChanged), _hiLimit(HiLimit),
    _lowLimit(LowLimit), _limitFlag(LimitFlag)
{
    _mayResetAlarm = true;
    _impTimer = new QTimerExt(this);
    _impTimer->setSingleShot(true);
}

//------------------------------------------------------------------------------

bool OutETag::setValue(QVariant Value, bool notImit)
{
    double chkVal = _correctByLimits(Value).toDouble();
    if( chkVal != Value.toDouble() ){
        emit s_valueChd(value());
        return false;
    }

    if(_imit && ! notImit){
        if(_imitVal != Value) {
            _imitVal = Value;
            if(! _pulse)
                _logging(Prom::MessChangeSensor, "установлено значение - " + QString::number(Value.toDouble()), _imit);
            emit s_imitationValueChd(_imitVal);
            emit s_valueChd(_imitVal);
        }
        return true;
    }

    if(!_ok){
        _logging(Prom::MessAlarm, "установить значение - " + QString::number( Value.toDouble()) + " не удалось: сигнал не загружен", false);
        emit s_valueChd(_value);
        return false;
    }
    //_setTimer->stop(); убрал, но боюсь
    if(_tag->readQuality() != Prom::Good){
        _setedValue = Value;
        _logging(Prom::MessAlarm, "установить значение - " + QString::number( Value.toDouble()) + " не удалось: нет связи", false);
        emit s_valueChd(_value);
        return false;
    }

    if( _value == Value ){
        _logging(Prom::MessAlarm, "значение - "+ QString::number(Value.toDouble()) + " уже установлено", false);
        return true;
    }
    //    static QVariant convVal;
    //    switch (_conv) {
    //    case Prom::VCNo :
    //        convVal = Value;
    //        break;
    //    case Prom::VCFloatInIntToDouble :
    //        convVal =  QVariant(Value.toDouble() * 10).toInt();
    //        break;
    //    case Prom::VCFloat3InIntToDouble3 :
    //        convVal =  QVariant(Value.toDouble() * 1000).toInt();
    //        break;
    //    }
    //    if( ! _tag->writeValue(convVal)){
    //        _logging(Prom::MessAlarm, "TSP отказал в установке значения - " + QString::number(Value.toDouble()), _imit);
    //        emit s_valueChd(_value);
    //        return false;
    //    }
    //    setTimerStart();
    //    _setedValue = Value;
    //    if(_setType == Prom::PreSet){
    //        //_value = Value;
    //        if(_preValue != _value){
    //            _preValue = _value;
    //            if(_owner->ini && saveValue){
    //                _owner->ini->setValue(_owner->tagPrefix+ "/" + _DBName + ".value", _value);
    //            }
    //            emit s_valueChd(Value);
    //        }
    //    }
    //    if(! _pulse){
    //        _logging(Prom::MessChangeSensor,"установлено значение - " + QString::number(Value.toDouble()), _imit);
    //    }

    return true;
}

//------------------------------------------------------------------------------
void OutETag::_checkVal()
{
    if(_imit) return;
    //qDebug()<<this->objectName()<< "CheckVal()" <<_value<<_preValue<<_setedValue;
    if( _value.toDouble() != _preValue.toDouble() ){
        if( _value.toDouble() == _setedValue.toDouble() ){
            if( saveValue && _owner->ini != nullptr )
                _owner->ini->setValue(_owner->tagPrefix+ "/" + _DBName + ".value", _value);
        }
        else{
            //_logging(Prom::MessAlarm,"не удалось установить требуемое значение (получили - " + QString::number( _value.toDouble()) + ", нужно - " + QString::number(_setedValue.toDouble()) + ")", _imit);
        }
        //NOTE может и сломаться!!!_preValue = _value;
        emit s_valueChd(_value);
    }
}

//------------------------------------------------------------------------------
void OutETag::writeImit(bool setImit)
{
    if(_imit != setImit){
        _imit = setImit;
        if(_imit){//++++ вкл. имитации
            _setTimer->stop();
            _logging(Prom::MessInfo, "имитация включена", false);
            emit s_qualityChd(true);
        }
        else {//++++ выкл. имиации
            _logging(Prom::MessInfo, "имитация отключена", false);
            if(_ok){
                emit s_qualityChd(_tag->readQuality() == Prom::Good);
            }
            else emit s_qualityChd(false);
        }
        if(_value != _imitVal) {
            emit s_valueChd(value());
            //_checkVal();
        }
        //emit ChangeIm(_imit);
    }
}

//------------------------------------------------------------------------------
void OutETag::writeImitVal(QVariant setVal)
{
    if(_imitVal != setVal){
        _preValue = _imitVal;
            if(_imit) {
            setValue(setVal);
        }
        else {
            _imitVal = setVal;
            //emit ChangeImVal(_imitVal);
        }
    }
}

//------------------------------------------------------------------------------
void OutETag::_qualityChangedSlot()
{
    ETag::_qualityChangedSlot();
    if(_tag->readQuality() == Prom::Good){
        if(_ferstLoad){
            _ferstLoad = false;
            if( saveValue ){
                _logging(Prom::MessVerbose,
                    "загрузка сохранённого значения -"
                        + QString::number(_iniValue.toDouble()), false);
                _preValue = double(0.0);
                setValue(_iniValue, true);
            }
            else if( loadDefalt ){
                _logging(Prom::MessVerbose,
                    "загрузка значения по умолчанию -"
                        + QString::number(_defaultValue.toDouble()), false);
                _preValue = double(0.0);
                setValue(_defaultValue, true);
            }
            else {
                _logging(Prom::MessVerbose,
                    "подхват выхода при запуске SCADA", false);
                _setedValue = _value;
                _checkVal();
            }
        }
        else setValue(_setedValue, true);
    }
}

//------------------------------------------------------------------------------
void OutETag::setTimerEnd()
{
    if(_value != _setedValue ){
        _logging(Prom::MessAlarm,
            "превышение времени установки выходного тэга "
            "(получили - " + QString::number(_value.toDouble()) +
                ", нужно - " + QString::number(_setedValue.toDouble()) + ")", _imit);
        if(! _pulse) {
            setValue(_setedValue);
        }
    }
}

//------------------------------------------------------------------------------
void OutETag::_checkPulse()
{
    if(_value == _setedValue) {
        _pulseAsyncDelay = _setTimer->interval()
            - _setTimer->remainingTime();
    }
}

//------------------------------------------------------------------------------
void OutETag::loadParam()
{
    if(_owner->ini && saveValue
        && _owner->ini->contains(_owner->tagPrefix+ "/" + _DBName + ".hardInitValue") ){
        _iniValue = _owner->ini->value(_owner->tagPrefix+ "/" + _DBName + ".hardInitValue", (_owner->ini->value(_owner->tagPrefix+ "/" + _DBName + ".value", 0).toDouble())).toDouble();
        _setedValue = _iniValue;
        _value = _iniValue;
    }
    else if( loadDefalt ){
        _setedValue = _defaultValue;
        _value = _defaultValue;
    }
    ETag::loadParam();
}

//------------------------------------------------------------------------------
void OutETag::saveParam()
{
    ETag::saveParam();
    if(_owner->ini && saveValue)
        _owner->ini->setValue(_owner->tagPrefix+ "/" + _DBName + ".value", _value);
}

//------------------------------------------------------------------------------
void OutETag::setLowLimit(const QVariant &lowLimit)
{
    _lowLimit = lowLimit;
    _limitFlag |= OUT_LIM_MIN;
    _logging( Prom::MessInfo,
        "установлено ограничение минимума "
            + _lowLimit.toString(), false );
    if( value().toDouble() < _lowLimit.toDouble() ){
        setValue( _lowLimit.toDouble() );
    }
}

//------------------------------------------------------------------------------
void OutETag::setHiLimit(const QVariant &hiLimit)
{
    _hiLimit = hiLimit;
    _limitFlag |= OUT_LIM_MAX;
    _logging( Prom::MessInfo,
        "установлено ограничение максимума "
            + _hiLimit.toString(), false );
    if( value().toDouble() > _hiLimit.toDouble() ){
        setValue( _hiLimit.toDouble() );
    }
}

//------------------------------------------------------------------------------
void OutETag::setLimitFlag(const quint8 &limitFlag)
{
    if( limitFlag == OUT_LIM_NO ){
        _logging( Prom::MessInfo,
            "отключены ораничения максимума и минимума ", false );
        _limitFlag = OUT_LIM_NO;
    }
    else{
        if( _limitFlag & OUT_LIM_MAX ){
            if( !(limitFlag & OUT_LIM_MAX) ){
                _logging( Prom::MessInfo,
                    "отключено ораничение максимума ", false );
            }
        }
        else{
            if( limitFlag & OUT_LIM_MAX ){
                _logging( Prom::MessInfo,
                    "включено ораничение максимума ", false );
                if( value().toDouble() > _hiLimit.toDouble() ){
                    setValue( value() );
                }
            }
        }
        if( _limitFlag & OUT_LIM_MIN ){
            if( !(limitFlag & OUT_LIM_MIN) ){
                _logging( Prom::MessInfo,
                    "отключено ораничение минимума ", false );
            }
        }
        else{
            if( limitFlag & OUT_LIM_MIN ){
                _logging( Prom::MessInfo,
                    "включено ораничение минимума ", false );
                if( value().toDouble() < _lowLimit.toDouble() ){
                    setValue( value() );
                }
            }
        }
    }
}

//------------------------------------------------------------------------------
QVariant OutETag::_correctByLimits(QVariant Value)
{
    if( _limitFlag ){
        if( _limitFlag & OUT_LIM_MAX
            && ( Value.toDouble() > _hiLimit.toDouble() )) {
            Value = _hiLimit.toDouble();
            _logging( Prom::MessInfo, "устанавливаемое значение первысило максимум ", _imit );
        }
        if( _limitFlag & OUT_LIM_MIN
            && ( Value.toDouble() > _hiLimit.toDouble() )) {
            Value = _lowLimit.toDouble();
            _logging( Prom::MessInfo, "устанавливаемое значение меньше минимума ", _imit );
        }
    }
    return Value;
}

//------------------------------------------------------------------------------
QVariant OutETag::lowLimit() const
{
    return _lowLimit;
}

//------------------------------------------------------------------------------
QVariant OutETag::hiLimit() const
{
    return _hiLimit;
}
//------------------------------------------------------------------------------
void OutETag::setImpulseDuration(QVariant Delay)
{
    _impulseDuration = Delay.toInt() * 1000;
    _impTimer->setInterval(_impulseDuration);
}

//------------------------------------------------------------------------------
int OutETag::impulseDuration()
{
    if(tunableImpulseTime){
        return _impulseDuration;
    }
    else return 0;
}

//------------------------------------------------------------------------------
void OutETag::_customConnectToGUI(QObject *, QObject *engRow)
{
    QVariant ret;
    QObject * tmpSgSt;

    if(ttype == Prom::TpOut) { //Для аналоговых ВЫходных сигналов
        //!создал в строке главный раздел
        QMetaObject::invokeMethod(engRow, "addPropertyValue", Qt::DirectConnection,
            Q_RETURN_ARG(QVariant, ret),
            Q_ARG(QVariant, this->fullTagName() + "_PrVal"),
            Q_ARG(QVariant, this->getName()));

        //tmpSgSt = propWin->findChild<QObject*>(est->fullTagName() + "_PrSig");
        tmpSgSt = qvariant_cast< QObject* >(ret);
        //получил указатель на главный раздел
        //-----подключил сигналы к значению и имитации
        connect(tmpSgSt, SIGNAL(changedIm(bool)), this,      SLOT(writeImit(bool)), Qt::QueuedConnection);
        connect(tmpSgSt, SIGNAL(changedImVal(QVariant)), this,      SLOT(writeImitVal(QVariant)), Qt::QueuedConnection);
        connect(this,    SIGNAL(s_imitationChd(QVariant)), tmpSgSt, SLOT(changeIm(QVariant)), Qt::QueuedConnection);
        connect(this,    SIGNAL(s_imitationValueChd(QVariant)), tmpSgSt, SLOT(changeImVal(QVariant)), Qt::QueuedConnection);
        connect(this,    SIGNAL(s_liveValueChd(QVariant)), tmpSgSt, SLOT(changeVal(QVariant)), Qt::QueuedConnection);
        connect(this,    SIGNAL(s_qualityChd(QVariant)), tmpSgSt, SLOT(changeConnected(QVariant)), Qt::QueuedConnection);
        //-----подключил сигналы к значению и имитации
        if(_menuChanged){
            QMetaObject::invokeMethod(engRow, "addPropertySetting", Qt::DirectConnection,
                Q_RETURN_ARG(QVariant, ret),
                Q_ARG(QVariant, this->getDBName() + "_value" ),
                Q_ARG(QVariant, " - задать значение"),
                Q_ARG(QVariant, -1));
            //tmpSgSt = guiItem->findChild<QObject*>(est->getDBName() + "_delay");
            tmpSgSt = qvariant_cast< QObject* >(ret);//получаю указатель на уровень срабатывания
            //подключаю сигналы к уровням срабатывания
            connect(tmpSgSt, SIGNAL(changedVal(QVariant)), this,  SLOT(setValue(QVariant)),    Qt::QueuedConnection);
            connect(this,    SIGNAL(s_valueChd(QVariant)),    tmpSgSt, SLOT(changeVal(QVariant)), Qt::QueuedConnection);
            //подключаю сигналы к уровням срабатывания
        }
    }        //!добавляю время импульса
    if(tunableImpulseTime){
        QMetaObject::invokeMethod(engRow, "addPropertySetting", Qt::DirectConnection,
            Q_RETURN_ARG(QVariant, ret),
            Q_ARG(QVariant, this->getDBName() + "_impulseTime" ),
            Q_ARG(QVariant, "сек. - время импульса"),
            Q_ARG(QVariant, 1));
        //tmpSgSt = guiItem->findChild<QObject*>(est->getDBName() + "_delay");
        tmpSgSt = qvariant_cast< QObject* >(ret);//получаю указатель на задержку
        //подключаю сигналы к задержке
        connect(tmpSgSt, SIGNAL(changedVal(    QVariant)),     this,     SLOT(setImpulseDuration(QVariant)), Qt::QueuedConnection);
        connect(this,    SIGNAL(ChangeImpulseDelay(QVariant)), tmpSgSt, SLOT(changeVal(QVariant)    ), Qt::QueuedConnection);
        connect(this->_impTimer,  SIGNAL(StartSig(QVariant)),                 tmpSgSt, SLOT(startCountdown(QVariant)),       Qt::QueuedConnection);
        connect(this->_impTimer,  SIGNAL(StopSig() ), tmpSgSt, SLOT(stopCountdown()),       Qt::QueuedConnection);
        connect(this->_impTimer,  SIGNAL(timeout() ), tmpSgSt, SLOT(stopCountdown()),       Qt::QueuedConnection);
        //подключаю сигналы к задержке
    }
}
//------------------------------------------------------------------------------

