#include "OutDiscretETag.h"
#include "unit.h"
#include <qdebug.h>
#include <QSettings>

using Prom::MessType;

OutDiscretETag::OutDiscretETag(Unit * Owner,
                               Prom::OutESTagSetType SetType,
                               QString Name,
                               QString DBName,
                               QVariant Hi,
                               QVariant Low,
                               bool Save,
                               bool LoadDefault,
                               QVariant DefaultValue,
                               bool MenuChanged,
                               bool TunableSetTime,
                               bool TunablePulseTime,
                               bool EgnorableAlarm,
                               bool InGUI,
                               Prom::ETagValConv Convertion,
                               bool TunableImpulseTime)
    : OutETag(Owner,
              Prom::TpDiscretOut,
              SetType,
              Name,
              DBName,
              TunableSetTime,
              TunablePulseTime,
              EgnorableAlarm,
              InGUI,
              Convertion,
              Save,
              LoadDefault,
              DefaultValue,
              MenuChanged,
              OUT_LIM_NO,
              true,
              false,
              TunableImpulseTime),
      _hi(Hi), _low(Low)
{
    connect(_impTimer, &QTimer::timeout, this, &OutDiscretETag::impilseTimerEnd, Qt::QueuedConnection);
}

//------------------------------------------------------------------------------
bool OutDiscretETag::setValue(QVariant Value, bool notImit)
{
    if(Value == _low || Value == _hi){
        if(OutETag::setValue(Value, notImit)){
            if(_pulse) return true;
            else{
                if(_setType == Prom::PreSet){
                    if(Value == _hi && ! _OnOff) {
                        _OnOff = true;
                        emit s_on();
                    }
                    else if(Value == _low && _OnOff){
                        _OnOff = false;
                        emit s_off();
                    }
                }
                else{
                    if(_imit){
                        _OnOff = (Value == _hi);
                    }
                }
                return  true;
            }
        }
    }
    else{
        _logging(Prom::MessError, QString::number(Value.toDouble())+ " - не допустимое значение для этого дискретного выхода", _imit);
    }
    return  false;

}

//------------------------------------------------------------------------------
bool OutDiscretETag::on()
{
    if (setValue(_hi)){
        if( _impTrigger ){
            _impTrigger = false;
            _pulse = _pulseDuration > 0;
            _pulseTimer->setInterval( _pulseDuration );
            _impTimer->setInterval( _impulseDuration );
        }
        if(_impTimer->interval() > 0){
            _impTimer->start();
            _logging(Prom::MessChangeSensor, "импульс на " + QString::number(_impTimer->interval()) + "сек.", _imit);
        }
        if(_pulse) {
            if(_setTimer->interval() > _pulseTimer->interval()) {
                _setTimer->setInterval(_pulseTimer->interval());
                emit s_setDelayChd(_setTimer->interval()/1000);
            }
            _logging(Prom::MessChangeSensor, "включён в режиме пульсации", _imit);
            _pulseHL = true;
            _pulseTimer->start();
            _OnOff = true;
            emit s_on();
            return  true;
        }
        else _logging(Prom::MessChangeSensor, "включён", _imit);
        return true;
    }
    else return false;
}
//------------------------------------------------------------------------------

bool OutDiscretETag::off()
{
    if( _impTrigger ){
        _impTrigger = false;
        _pulse = _pulseDuration > 0;
        _pulseTimer->setInterval( _pulseDuration );
        _impTimer->setInterval( _impulseDuration );
    }
    _pulseTimer->stop();
    if(_impTimer->isActive())
        _impTimer->stop();
    if(_pulse) {
        if (setValue(_low)) _pulseHL = false;
        _OnOff = false;
        _pulseTimer->stop();
        _logging(Prom::MessChangeSensor, "выключeн в режиме пульсации", _imit);
        emit s_off();
        return true;
    }
    if (setValue(_low)){
        _logging(Prom::MessChangeSensor, "выключeн", _imit);
        return true;
    }
    else return false;
}

//------------------------------------------------------------------------------
void OutDiscretETag::singleImpulse(int Duration, int Pulse)
{
    if( Pulse == 0 ){
        _pulse = false;
    }
    _impTimer->setInterval( Duration );
    _pulseTimer->setInterval( Pulse );
    on();
    _impTrigger = true;
}

//------------------------------------------------------------------------------
//void OutDiscretETag::setPulse(bool set)
//{
//    //pulsOut(set);
//}

//------------------------------------------------------------------------------
bool OutDiscretETag::isOn()
{
    return _OnOff;
}
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
//void OutDiscretETag::WriteImit(bool setImit)
//{

//    if(_imit != setImit){
//        _imit = setImit;
//        if(_imit){//++++ вкл. имитации
//            Logging(Prom::MessInfo, "имитация включена", false);
//            if(_ok){
//                if(_imitVal != _value && ! _pulse){
//                    if(_imitVal == _hi) On();
//                    else Off();
//                }
//            }
//            else if(_imitVal == _hi && ! _pulse) On();
//            if(_ok && _tag->ReadQuality() != Prom::Good)
//                emit QualityChanged(true);
//        }
//        else {//++++ выкл. имиации
//            Logging(Prom::MessInfo, "имитация отключена", false);
//            if(_ok){
//                if(! _pulse)CheckVal();
//            }
//            else Off();
//            if(_ok && _tag->ReadQuality() != Prom::Good)
//                emit QualityChanged(false);
//        }
//        //emit ChangeIm(_imit);
//    }
//}

//------------------------------------------------------------------------------
//void OutDiscretETag::WriteImitVal(QVariant setVal)
//{
//    if(setVal == _hi || setVal ==_low ){
//        if(_imitVal != setVal && ! _pulse){
//            if(_imit) {
//                if(setVal == _hi) emit on();
//                else emit off();
//                emit s_valueChd(_imitVal);
//            }
//            else {
//                _imitVal = setVal;
//                //emit ChangeImVal(_imitVal);
//            }
//        }
//    }
//}

//------------------------------------------------------------------------------
void OutDiscretETag::_checkVal()
{
    if(! _pulse){
        if(value() == _hi) {
            if(! _OnOff){
                _OnOff = true;
                _logging(Prom::MessChangeSensor, "включён", false);
                emit s_on();
            }
        }
        else if(value() == _low){
            if(_OnOff){
                _OnOff = false;
                _logging(Prom::MessChangeSensor, "выключeн", false);
                emit s_off();
            }
        }
    }
    OutETag::_checkVal();
}
//------------------------------------------------------------------------------
void OutDiscretETag::pulseTimerEnd()
{
    _pulseHL = ! _pulseHL;
    setValue( _pulseHL ? _hi : _low);
    _pulseTimer->start();
}

//------------------------------------------------------------------------------
void OutDiscretETag::impilseTimerEnd()
{
    off();
}
//------------------------------------------------------------------------------
void OutDiscretETag::saveParam()
{
    OutETag::saveParam();
    if(tunableImpulseTime)
        _owner->ini->setValue(_owner->tagPrefix+ "/" + _DBName + ".impTime", _impTimer->interval());
}

//------------------------------------------------------------------------------
void OutDiscretETag::loadParam()
{
    if(tunableImpulseTime){
      if( _owner->ini->contains(_owner->tagPrefix+ "/" + _DBName + ".impTime") )
        _impTimer->setInterval(_owner->ini->value(_owner->tagPrefix+ "/" + _DBName + ".impTime", tunableImpulseTime ? 2000 : 0).toInt());
        //qDebug() << _DBName + " Load " << _impTimer->interval();
    }
    OutETag::loadParam();
}

//------------------------------------------------------------------------------
void OutDiscretETag::reInitialise()
{
    //    if(tunableImpulseTime){
    //        qDebug() << _DBName << _impTimer->interval();
    //    }
    if(_OnOff) emit s_on();
    else emit s_off();
    emit ChangeImpulseDelay(_impTimer->interval() / 1000);
    OutETag::reInitialise();
}


//------------------------------------------------------------------------------
void OutDiscretETag::pulsOut(bool On)
{
    _pulse = On;
    if(_pulse){
        _logging (Prom::MessChangeSensor, "включён режим пульсации", false);
        if(_OnOff) _pulseTimer->start();
    }
    else {
        _logging (Prom::MessChangeSensor, "выключен режим пульсации", false);
    }
}

//------------------------------------------------------------------------------
void OutDiscretETag::_customConnectToGUI(QObject *, QObject *engRow)
{
    QVariant ret;
    QObject * tmpSgSt;
    //!создал в строке главный раздел (с кубиками)
    QMetaObject::invokeMethod(engRow, "addPropertySignal", Qt::DirectConnection,
                              Q_RETURN_ARG(QVariant, ret),
                              Q_ARG(QVariant, this->fullTagName() + "_PrSig"),
                              Q_ARG(QVariant, this->getName()));
    //tmpSgSt = propWin->findChild<QObject*>(est->fullTagName() + "_PrSig");
    tmpSgSt = qvariant_cast< QObject* >(ret);
    //получил указатель на главный раздел
    //-----подключил сигналы к кубикам
    connect(tmpSgSt, SIGNAL(changedIm(     bool    )), this,      SLOT(writeImit(      bool    )), Qt::QueuedConnection);
    connect(tmpSgSt, SIGNAL(changedImVal(  QVariant)), this,      SLOT(writeImitVal(   QVariant)), Qt::QueuedConnection);
    connect(this,      SIGNAL(s_imitationChd(      QVariant)), tmpSgSt, SLOT(changeIm(       QVariant)), Qt::QueuedConnection);
    connect(this,      SIGNAL(s_imitationValueChd(   QVariant)), tmpSgSt, SLOT(changeImVal(    QVariant)), Qt::QueuedConnection);
    connect(this,      SIGNAL(s_liveValueChd(QVariant)), tmpSgSt, SLOT(changeVal(      QVariant)), Qt::QueuedConnection);
    connect(this,      SIGNAL(s_qualityChd(QVariant)), tmpSgSt, SLOT(changeConnected(QVariant)), Qt::QueuedConnection);
    //-----подключил сигналы к кубикам

    OutETag::_customConnectToGUI(nullptr, engRow);
}

//------------------------------------------------------------------------------

