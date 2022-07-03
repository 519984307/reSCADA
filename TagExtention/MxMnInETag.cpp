#include "MxMnInETag.h"
#include <QDebug>
#include "unit.h"
#include <QSettings>
#include "OutETag.h"

using Prom::MessType;

MxMnInETag::MxMnInETag(Unit * Owner,
    Prom::ESTagType Type,
    QString Name,
    QString DBName,
    QVariant MaxLevel,
    QVariant MinLevel,
    QVariant ChageStep,
    bool InOrOutDetect,
    bool TunableSetTime,
    bool TunablePulseTime,
    bool TunabDetectLevel,
    bool EgnorableAlarm,
    bool InGUI,
    Prom::ETagValConv Convertion,
    int TimeMax)
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
    _inOrOutDetect(InOrOutDetect),
    _maxLevel(MaxLevel),
    _minLevel(MinLevel),
    _tunabDetectLevel(TunabDetectLevel)
{
    _timeMax = new QTimerExt(this);
    _timeMax->setInterval(TimeMax * 1000);
    _timeMax->setSingleShot( true );
    connect( _timeMax, &QTimerExt::timeout, this, &MxMnInETag::_timeMaxStep, Qt::QueuedConnection );
}
//------------------------------------------------------------------------------
void MxMnInETag::saveParam()
{
    ETag::saveParam();
    if(_maxLevelSave)_owner->ini->setValue(_owner->tagPrefix+ "/" + _DBName + ".maxLevel", _maxLevel);
    if(_minLevelSave)_owner->ini->setValue(_owner->tagPrefix+ "/" + _DBName + ".minLevel", _minLevel);
}

//------------------------------------------------------------------------------
void MxMnInETag::loadParam()
{
    if(_maxLevelSave)_maxLevel = _owner->ini->value(_owner->tagPrefix+ "/" + _DBName + ".maxLevel", 0).toDouble();
    if(_minLevelSave)_minLevel = _owner->ini->value(_owner->tagPrefix+ "/" + _DBName + ".minLevel", 0).toDouble();
    ETag::loadParam();
}

//------------------------------------------------------------------------------
void MxMnInETag::reInitialise()
{
    ETag::reInitialise();
    emit s_maxLevelChanged(_maxLevel);
    emit s_minLevelChanged(_minLevel);
    if (_detect) emit s_detected();
    else emit s_undetected();

    if(_maxDetect) emit s_maxDetected();
    else if(_minDetect) emit s_minDetected();
}

//------------------------------------------------------------------------------
QVariant MxMnInETag::maxLevel() const
{
    return _maxDetect;
}

//------------------------------------------------------------------------------
QVariant MxMnInETag::minLevel() const
{
    return _minDetect;
}

//------------------------------------------------------------------------------
void MxMnInETag::setMaxLevel(QVariant MaxLevel)
{
    if( _maxLevel != MaxLevel.toDouble() ){
        if(MaxLevel.toString() == "nan")MaxLevel = 0;
        _maxLevel = MaxLevel.toDouble();
        _logging(Prom::MessInfo, "максимальный уровень срабатывания изменён на -" + _maxLevel.toString(), false);
        _checkVal();
        emit s_maxLevelChanged(_maxLevel);
    }
}

//------------------------------------------------------------------------------
void MxMnInETag::setMinLevel(QVariant MinLevel)
{
    if( _minLevel != MinLevel.toDouble() ){
        if(MinLevel.toString() == "nan")MinLevel = 0;
        _minLevel = MinLevel.toDouble();
        _logging(Prom::MessInfo, "максимальный уровень срабатывания изменён на -" + _minLevel.toString(), false);
        _checkVal();
        emit s_minLevelChanged(_minLevel);
    }
}

//------------------------------------------------------------------------------
void MxMnInETag::writeImit(bool setImit)
{
    if(_imit != setImit){
        _imit = setImit;

        if(_imit){
            //            if(_pulse) disconnect(_tag ,SIGNAL(onValueChanged(QVariant)), this, SLOT(pulse()));
            //            else disconnect(_tag ,SIGNAL(onValueChanged(QVariant)), this, SLOT(CheckVal()));
            _logging(Prom::MessInfo, "имитация включена", false);
            emit s_qualityChd(true);
            if(_imitVal != _value)_checkVal();
        }
        else{
            //            if(_pulse) connect(_tag ,SIGNAL(onValueChanged(QVariant)), this, SLOT(pulse()));
            //            else connect(_tag ,SIGNAL(onValueChanged(QVariant)), this, SLOT(CheckVal()));
            _logging(Prom::MessInfo, "имитация отключена", false);
            if(_ok){
                if(_pulse) _pulseTimer->start();
                else if(_imitVal != _value)
                    _checkVal();
                emit s_qualityChd(_tag->readQuality() == Prom::Good);
            }
            else emit s_qualityChd(false);
        }
    }
}

//------------------------------------------------------------------------------
void MxMnInETag::writeImitVal(QVariant setVal)
{
    if(_imitVal != setVal){
        _preValue = _imitVal;
        _imitVal = setVal;
        _logging(Prom::MessInfo, "имитация значения - " +  setVal.toString(), false);
        if(_imit){
            if(_pulse) _checkPulse();
            else _checkVal();
        }
        emit s_imitationValueChd(_imitVal);
    }
}

//------------------------------------------------------------------------------
void MxMnInETag::_qualityChangedSlot()
{
    ETag::_qualityChangedSlot();
    if(_tag->readQuality() == Prom::Good && ! _imit) {
        if(_pulse){
            _pulseTimer->start();
        }
        else{
            _checkVal();
        }
    }
}

//------------------------------------------------------------------------------
void MxMnInETag::needBe(bool DtctOrNot, bool AlarmOn, bool SetTimer)
{
    if(_DnotU != DtctOrNot || _alarmOn != AlarmOn){
        _logging (Prom::MessVerbose, QString("установлен режим обработки сигнала - необходтимо быть ")
                + QString(DtctOrNot ? "" : "не")
                + "сработавшим (больше " + _maxLevel.toString() + "/меньше "
                + _minLevel.toString() + ")"
                +  QString(_alarmOn ? " с генерацией":" без генерации ") + " аварии,"
                + QString(SetTimer ? " с таймером":" без таймера") + " на установку нужного состояния"
            , false);
    }
    _DnotU = DtctOrNot;
    _alarmOn = AlarmOn;
    _onlyChange = false;
    _setTimer->stop();

    if(SetTimer && connected()){
        _mayResetAlarm = true;
        if(_setTimer->interval() > 0){
            setTimerStart();
        }
        else _logging(Prom::MessWarning,  "установлено бесконечное время на принятие сигналом нужного состояния", false);
    }
    else {
        _detect = _checkDetect();
        _preDetect = _detect;
        if(_detect == _DnotU){
            _mayResetAlarm = true;
        }
        else{
            if((_detect != _DnotU && _alarmOn)){
                if(_ignorAlarm) {
                    _mayResetAlarm = true;
                    emit s_alarm("");
                    _logging(Prom::MessInfo,  "проигнорирована авария недупустимого состояния, при смене режима обработки сигнала", false);
                }
                else {
                    _mayResetAlarm = false;
                    _alarm = true;
                    emit s_alarm(_name + " - авария недупустимого состояния, при смене режима обработки сигнала");
                }
            }
        }
    }
}

//------------------------------------------------------------------------------
void MxMnInETag::onlyChange()
{
    if(! _onlyChange)
        _logging (Prom::MessVerbose, "установлен режим 'только информироввание об изменениях'", false);
    _onlyChange = true;
    _mayResetAlarm = true;
    _alarmOn = false;
    _setTimer->stop();
}

//------------------------------------------------------------------------------
void MxMnInETag::pulsSensor(bool On)
{
    _pulse = On;
    if(_pulse){
        if(_setTimer->interval() < _pulseTimer->interval()){
            _setTimer->setInterval(_pulseTimer->interval() + 1000);
            emit s_pulseDelayChd(_setTimer->interval()/1000);
        }
        //if(! _imit) {
        disconnect(_tag ,SIGNAL(onValueChanged(QVariant)), this, SLOT(_checkVal()));
        connect(_tag ,SIGNAL(onValueChanged(QVariant)), this, SLOT(_checkPulse()));
        //}
        _trig = true;
        _detectPulse = false;
        _detect = false;
        _setTimer->start();
        _logging (Prom::MessInfo, "установлен режим детектирования пульсации тэга", false);
    }
    else {
        disconnect(_tag, SIGNAL(onValueChanged(QVariant)), this, SLOT(_checkPulse()));
        connect(_tag, SIGNAL(onValueChanged(QVariant)), this, SLOT(_checkVal()));
        if(_detect != _checkDetect())
            _checkVal();
        _logging (Prom::MessInfo, "выключен режим детектирования пульсации тэга", false);
    }
}

//------------------------------------------------------------------------------
void MxMnInETag::_setTimerEnd()
{
    _setTimer->stop(); //Для сторонних сигналов
    if(! _onlyChange){
        if((_DnotU && ! _detect) || (! _DnotU && _detect)){
            if(_ignorAlarm) {
                _logging (Prom::MessAlarm, "проигнорирована авария, превышения времени на установку необходимого состояния", _imit);
                emit s_alarm("");
            }
            else {
                _alarm = true;
                _alarmStr = "превышение времени на установку необходимого состояния";
                emit s_alarm( _name + " " + _alarmStr );
            }
        }
    }
}

//------------------------------------------------------------------------------
bool MxMnInETag::_checkDetect()
{
    _maxDetect = value().toDouble() >= _maxLevel.toDouble();
    _minDetect = value().toDouble() <= _minLevel.toDouble();

    return _inOrOutDetect ? !(_maxDetect || _minDetect) : _maxDetect || _minDetect;
}
//------------------------------------------------------------------------------
void MxMnInETag::setTimeMax( int TimeMax )
{
    _timeMax->setInterval(TimeMax*1000);
}

//------------------------------------------------------------------------------
int MxMnInETag::timeMax()
{
    if( _timeMax != nullptr ){
        return _timeMax->interval()/1000;
    }
    else{
        return 0;
    }
}

//------------------------------------------------------------------------------
bool MxMnInETag::connectTagToMaxLevel(OutETag *Tag)
{
    bool res = true;
    if( Tag == nullptr ) return false;
    if( !Tag->isOk() ) return false;
    setMaxLevel( Tag->value() );
    res = res && connect( this, SIGNAL(s_maxLevelChanged(QVariant)), Tag,  SLOT(setValue(QVariant)), Qt::QueuedConnection );
    res = res && connect( Tag,  &OutETag::s_valueChd,           this, &MxMnInETag::setMaxLevel, Qt::QueuedConnection );
    if( !res ) {
        disconnect( this, SIGNAL(s_maxLevelChanged(QVariant)), Tag,  SLOT(setValue(QVariant)) );
        disconnect( Tag,  &OutETag::s_valueChd,           this, &MxMnInETag::setMaxLevel );
    }
    else{
        _maxLevelSave = false;
    }
    return res;
}

//------------------------------------------------------------------------------
bool MxMnInETag::connectTagToMinLevel(OutETag *Tag)
{
    bool res = true;
    if( Tag == nullptr ) return false;
    if( !Tag->isOk() ) return false;
    setMinLevel( Tag->value() );
    res = res && connect( this, SIGNAL(s_minLevelChanged(QVariant)), Tag,  SLOT(setValue(QVariant)), Qt::QueuedConnection);
    res = res && connect( Tag,  &OutETag::s_valueChd,           this, &MxMnInETag::setMinLevel, Qt::QueuedConnection);
    if( !res ) {
        disconnect( this, SIGNAL(s_minLevelChanged(QVariant)), Tag,  SLOT(setValue(QVariant)) );
        disconnect( Tag,  &OutETag::s_valueChd,           this, &MxMnInETag::setMinLevel );
    }
    else{
        _minLevelSave = false;
    }
    return res;
}

//------------------------------------------------------------------------------
bool MxMnInETag::findMaxMinTags()
{
    bool res = true;
    res &= connectTagToMaxLevel( new OutETag(_owner, Prom::TpOut, Prom::PreSet, _name + " макс.", _DBName + ".max",false,false,false));
    res &= connectTagToMinLevel( new OutETag(_owner, Prom::TpOut, Prom::PreSet, _name + " мин.", _DBName + ".min",false,false,false));
    _tunabDetectLevel = !res;
    return res;
}

//------------------------------------------------------------------------------
void MxMnInETag::_timeMaxStep()
{
    if( _imit ) {
        _imitVal = _timeMaxValue;
        emit s_imitationValueChd( _imitVal );
    }
    else {
        _value = _timeMaxValue;
    }
    _preValue = _timeMaxValue.toDouble() - 1;
    _checkVal();
    if( _timeLastValue.toDouble() < _timeMaxValue.toDouble() ){
        _timeMaxValue = _timeLastValue;
        _timeMax->start();
    }
}

//------------------------------------------------------------------------------
void MxMnInETag::_checkVal()
{
    if( _timeMax->interval() > 0 ){
        if( value().toDouble() < _preValue.toDouble() ){
            if( _timeMax->isActive() ){
                //коплю максимум после падения
                _timeMaxValue = qMax( value(), _timeMaxValue );
                _timeLastValue = value().toDouble();
            }
            else {//запускаю защитный период
                _timeMaxValue = value().toDouble();
                _timeLastValue = value().toDouble();
                _timeMax->start();
            }
            //пока идет защитный период сбрасываю на старый максимум
            if( _imit ) {
                _imitVal = _preValue;
                emit s_imitationValueChd( _imitVal );
            }
            else {
                _value = _preValue;
            }
            return;
        }
        else {
            _timeMax->stop();
        }
    }

    static quint8 ChDt;//чтобы emit s_valueChd и detrct/undetect шли после логов и т.п.
    static bool preMaxDetect, preMinDetect;

    if (_pulse){
        _trig = ! _detectPulse;
        _detect = _detectPulse;
        ChDt = 0;
    }
    else {
        preMaxDetect = _maxDetect;
        preMinDetect = _minDetect;
        _detect = _checkDetect();
        if(value() != _preValue){
            ChDt = 1;
            //_preValue = value();
        }
    }
    if(_preDetect != _detect || preMaxDetect != _maxDetect || preMinDetect != _minDetect) {
        if(_onlyChange){// ---- Обработка без аварий ----
            if(_detect) {
                _logging(Prom::MessChangeSensor, "активирован ", _imit);
            }
            else {
                _logging(Prom::MessChangeSensor, "деактивирован ", _imit);
            }
        }
        else {// ---- Обработка с аварями ----
            _mayResetAlarm = false;
            if(_detect == _DnotU){//значение НЕ вышло за предел
                _mayResetAlarm = true;
                _logging(Prom::MessChangeSensor,"установлено нормальное состояние ", _imit);
            }
            else {//значение вышло за предел
                if(_alarmOn){
                    if(_setTimer->isActive()){
                        _logging(Prom::MessChangeSensor, "состояние допустимо изменено в рамках времени на установку", _imit);
                    }
                    else {
                        if(_ignorAlarm)
                            emit s_alarm("");
                        else{
                            _alarmStr = "выход за предельное значение: значение - "
                                + value().toString() + ", пределы от "
                                + _maxLevel.toString() + " до " + _minLevel.toString();
                            //if( preMaxDetect != _maxDetect || preMinDetect != _minDetect || ! _alarm )

                            if( _alarm
                                && (_inOrOutDetect ? ( ( !preMaxDetect && _minAlarm ) || ( !preMinDetect && _maxAlarm ))
                                                   : ( ( _minDetect   && _minAlarm ) || ( _maxDetect   && _maxAlarm ))) ){
                                //if( ( ( (_maxDetect && _minAlarm)||(_minDetect && _maxAlarm) ) && _inOrOutDetect ) )
                                emit s_alarm("");
                                //_logging(Prom::MessAlarm, _alarmStr, _imit);
                            }
                            else {
                                _alarm = true;
                                _maxAlarm = _inOrOutDetect ?  preMaxDetect : _maxDetect;
                                _minAlarm = _inOrOutDetect ?  preMinDetect : _minDetect;
                                emit s_alarm( _alarmStr );
                            }
                        }
                    }
                }
                else{
                    _mayResetAlarm = true;
                    _logging(Prom::MessChangeSensor, "нормальное состояние допустимо изменено", _imit);
                }
            }
        }
        ChDt += 2;
        _preDetect = _detect;
    }

    switch (ChDt) {
    case 1:
        emit s_valueChd(value());
        break;
    case 2://для режима пульсации
        if( _detect ) emit s_detected();
        else emit s_undetected();
        break;
    case 3://без пульсации
        emit s_valueChd(value());
        if( _detect ) emit s_detected();
        else emit s_undetected();
        if( _maxDetect ) emit s_maxDetected();
        else if( _minDetect ) emit s_minDetected();
        break;
    }
}

//------------------------------------------------------------------------------
void MxMnInETag::_checkPulse()
{
    emit s_valueChd(value());
    if(_trig){
        if(_detectPulse)
            _checkVal();
        else {
            _detectPulse = true;
        }
    }
    _pulseTimer->start();
}

//------------------------------------------------------------------------------
void MxMnInETag::pulseTimerEnd()
{
    _detectPulse = false;
    if(! _trig) _checkVal();
}

//------------------------------------------------------------------------------

void MxMnInETag::_customConnectToGUI(QObject *, QObject *engRow)
{
    QVariant ret;
    QObject * tmpSgSt;

    //!создал в строке главный раздел
    QMetaObject::invokeMethod(engRow, "addPropertyValue", Qt::DirectConnection,
        Q_RETURN_ARG(QVariant, ret),
        Q_ARG(QVariant, this->fullTagName() + "_PrVal"),
        Q_ARG(QVariant, this->getName()));

    //tmpSgSt = propWin->findChild<QObject*>(est->fullTagName() + "_PrSig");
    tmpSgSt = qvariant_cast< QObject* >(ret);
    //получил указатель на главный раздел
    //-----подключил сигналы к значению и имитации
    connect(tmpSgSt, SIGNAL(s_imChanged(bool)),               this,    SLOT(writeImit(bool)),           Qt::QueuedConnection);
    connect(tmpSgSt, SIGNAL(s_imValChanged(QVariant)),        this,    SLOT(writeImitVal(QVariant)),    Qt::QueuedConnection);
    connect(this,    SIGNAL(s_imitationChd(QVariant)),      tmpSgSt, SLOT(setIm(QVariant)),        Qt::QueuedConnection);
    connect(this,    SIGNAL(s_imitationValueChd(QVariant)), tmpSgSt, SLOT(setImVal(QVariant)),     Qt::QueuedConnection);
    connect(this,    SIGNAL(s_liveValueChd(QVariant)),      tmpSgSt, SLOT(setVal(QVariant)),       Qt::QueuedConnection);
    connect(this,    SIGNAL(s_qualityChd(QVariant)),        tmpSgSt, SLOT(setConnected(QVariant)), Qt::QueuedConnection);
    //-----подключил сигналы к значению и имитации

    //!добавляю уровень срабатывания
    if(_tunabDetectLevel){

        QMetaObject::invokeMethod(engRow, "addPropertySetting", Qt::DirectConnection,
            Q_RETURN_ARG(QVariant, ret),
            Q_ARG(QVariant, this->getDBName() + "_delay"),
            Q_ARG(QVariant, " - макс. уровень срабатывания"),
            Q_ARG(QVariant, -1));
        //tmpSgSt = guiItem->findChild<QObject*>(est->getDBName() + "_delay");
        tmpSgSt = qvariant_cast< QObject* >(ret);//получаю указатель на уровень срабатывания
        //подключаю сигналы к уровням срабатывания
        connect(tmpSgSt, SIGNAL(s_valChanged(QVariant)),        this,    SLOT(setMaxLevel(QVariant)), Qt::QueuedConnection);
        connect(this,    SIGNAL(s_maxLevelChanged(QVariant)), tmpSgSt, SLOT(setVal(QVariant)),      Qt::QueuedConnection);

        QMetaObject::invokeMethod(engRow, "addPropertySetting", Qt::DirectConnection,
            Q_RETURN_ARG(QVariant, ret),
            Q_ARG(QVariant, this->getDBName() + "_delay"),
            Q_ARG(QVariant, " - мин. уровень срабатывания"),
            Q_ARG(QVariant, -1));
        //tmpSgSt = guiItem->findChild<QObject*>(est->getDBName() + "_delay");
        tmpSgSt = qvariant_cast< QObject* >(ret);//получаю указатель на уровень срабатывания
        //подключаю сигналы к уровням срабатывания
        connect(tmpSgSt, SIGNAL(s_valChanged(QVariant)),        this,    SLOT(setMinLevel(QVariant)), Qt::QueuedConnection);
        connect(this,    SIGNAL(s_minLevelChanged(QVariant)), tmpSgSt, SLOT(setVal(QVariant)),      Qt::QueuedConnection);
    }
}
//------------------------------------------------------------------------------
