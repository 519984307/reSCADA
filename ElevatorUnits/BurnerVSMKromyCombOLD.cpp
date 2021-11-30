#include <QSettings>
#include "BurnerVSMKromyComb.h"
#include "InDiscretETag.h"
#include "OutETag.h"
#include "OutDiscretETag.h"
#include "QDebug"
//------------------------------------------------------------------------------
BurnerVSMKromyComb::BurnerVSMKromyComb(int ID,
                                       QString Name,
                                       QString TagPefix)
    :BurnerVSM_Kromy(ID,
                     Name,
                     TagPefix)
{
    _combStartDelay = new QTimerExt(this);
    _combStopDelay  = new QTimerExt(this);
    addTimer(_combStartDelay);
    addTimer(_combStopDelay);
    _combStartDelay->setSingleShot(true);
    _combStopDelay->setSingleShot(true);
    connect(_combStartDelay, &QTimer::timeout, this, &BurnerVSMKromyComb::combStartDelayEnd);
    connect(_combStopDelay, &QTimer::timeout, this, &BurnerVSMKromyComb::combStopDelayEnd);
}

//------------------------------------------------------------------------------
void BurnerVSMKromyComb::_updateStateAndMode()
{
    if(_start->isOn()){
        if(_relayKM->isDetected()){
            _combStopDelay->stop();
            _setCurrentState(Prom::UnStStarted);
            _setCurrentMode(Prom::UnMdStart);
        }
        else{
            _setCurrentState(Prom::UnStStartCommand);
            if(_relayKM->isImit() && _start->isImit())
                QTimer::singleShot(100, [ this ]() {
                    if(setedMode() == Prom::UnMdStart){
                        _relayKM->writeImitVal(true);
                        _startSig->writeImitVal(true);
                    }
                });
        }
    }
    else{
        if(_relayKM->isDetected()){
            _setCurrentState(Prom::UnStStopCommand);
            if(_relayKM->isImit() && _start->isImit())
                QTimer::singleShot(100, [ this ]()
                {
                    if(setedMode() == Prom::UnMdStop || setedMode() == Prom::UnMdCollingStop || setedMode() == Prom::UnMdColling){
                        _relayKM->writeImitVal(false);
                        _startSig->writeImitVal(false);
                    }
                });
        }
        else{
            if(_combustor->currentMode() == UnMdStart){
                if(!_combStopDelay->isActive())
                    _combStopDelay->start();
                _setCurrentMode(Prom::UnMdColling, false);
                _setCurrentState(Prom::UnStColling);
            }
            else{
                _setCurrentMode(Prom::UnMdStop);
                _setCurrentState(Prom::UnStStoped);
            }
        }
    }
}
//------------------------------------------------------------------------------
void BurnerVSMKromyComb::setStartCombDelay(QVariant Val)
{
    _combStartDelay->setInterval(Val.toUInt() * 1000);
}

//------------------------------------------------------------------------------
void BurnerVSMKromyComb::setStopCombDelay(QVariant Val)
{
    _combStopDelay->setInterval(Val.toUInt() * 1000);
}

//------------------------------------------------------------------------------
void BurnerVSMKromyComb::combStartDelayEnd()
{
    //    QObject * Ob;
    //    Ob = sender();
    if(isCurrOrSetedMode(UnMdStart)){
        if(! _alarm){
            if(_start->on())
                _combustor->setBlocked(true);
        }
    }
}

//------------------------------------------------------------------------------
void BurnerVSMKromyComb::combStopDelayEnd()
{
    if(isCurrOrSetedMode(UnMdCollingStop)){
        if(_combustor && _combustor->isCurrOrSetedMode(UnMdStart))
            _combustor->setMode(Prom::UnMdStop, false);
    }
    _combustor->setBlocked(false);
}

//------------------------------------------------------------------------------
bool BurnerVSMKromyComb::addSubUnit(Unit * unit)
{
    if(unit->unitType == Prom::TypeCombustor){
        _combustor = unit;
        //connect(_combustor, &Unit::StateChange, this, &Unit::updateState, Qt::QueuedConnection);
    }
    return BurnerVSM_Kromy::addSubUnit(unit);
}

//------------------------------------------------------------------------------
void BurnerVSMKromyComb::_updateSubUnitState(Unit * unit)
{
    //qDebug()<< _combustor->currentState();

    if(_combustor && unit == _combustor){
        switch (_combustor->currentState()) {
        case Prom::UnStStartCommand:
            break;
        case Prom::UnStStarted:
            if(setedMode() == Prom::UnMdStart){
                _combStartDelay->start();
            }
            _setCurrentMode(Prom::UnMdColling, false);
            break;
        case Prom::UnStStopCommand:
            break;
        case Prom::UnStStoped:
            _combustor->setBlocked(false);
            if(stateIn({ Prom::UnStStoped, Prom::UnStColling })){
                if(_combStopDelay->isActive()){
                    _combStopDelay->stop();
                    _setCurrentMode(Prom::UnMdStop);
                }
                else {
                    _setCurrentMode(Prom::UnMdCollingStop);
                }
                _setCurrentState(Prom::UnStStoped);
            }
            else {
                setMode(UnMdStop, false);
            }
            break;
        case Prom::UnStManualStarted:
            break;
        default:
            //qDebug()<<unit->currentState() << unit->currentMode();
            setMode(UnMdStop, false);
            break;
        }
    }
}

//------------------------------------------------------------------------------
void BurnerVSMKromyComb::_updateSubUnitMode(Unit *)
{

}

//------------------------------------------------------------------------------
SetModeResp BurnerVSMKromyComb::_customSetMode(UnitModes * mode, bool UserOrSys)
{
    if(! _sensorsConnected) {
        allTimerStop();
        return  Prom::RejNoCond;
    }

    switch (*mode) {
    case Prom::UnMdStart :
        if(_alarm)
            return Prom::RejAlarm;
        if(currentState() == Prom::UnStStarted)
            return DoneAlready;
        if(setedMode() == Prom::UnMdStart)
            return DoneWhait;
        allTimerStop();
        if(_combustor){
            switch (_combustor->setMode(Prom::UnMdStart, UserOrSys)) {
            case DoneAlready:
                if(_combStartDelay->isActive())
                    return Prom::DoneWhait;
                else if(_start->on()){
                    _combustor->setBlocked(true);
                    return Prom::DoneWhait;
                }
                else {
                    return Prom::RejNoCond;
                }
                break;
            case DoneWhait:
                return DoneWhait;
                break;
            default: return RejNoCond;
            }
        }
        else{
            return RejNoCond;
        }
        break;

    case Prom::UnMdCollingStop:
        _combStartDelay->stop();
        //if(_start->isOn())
        _start->off();
        if(currentMode() == Prom::UnMdStart){
            return DoneWhait;
        }
        else{
            if(_combStopDelay->isActive())
                return DoneWhait;
            else if(_combustor->isCurrOrSetedMode(Prom::UnMdStart)) {
                _combustor->setMode(Prom::UnMdStop, UserOrSys);
                return DoneWhait;
            }
            else return DoneAlready;
        }
        break;
    case Prom::UnMdColling:
        _combStartDelay->stop();
        if(currentMode() == Prom::UnMdStart){
            _start->off();
            if(_combustor->isCurrOrSetedMode(Prom::UnMdStart))
                return DoneWhait;
            else return RejNoCond;
        }
        else if(setedMode() == Prom::UnMdStart){
            _start->off();
            if(_combustor->currentMode() == Prom::UnMdStart)
                return DoneAlready;
            else if(_combustor->setedMode() == Prom::UnMdStart)
                return DoneWhait;
            else return RejNoCond;
        }
        else return RejNoCond;
        break;
    case Prom::UnMdStop :
        _combStartDelay->stop();
        _start->off();
        if(_combustor) _combustor->setMode(Prom::UnMdStop, UserOrSys);
        if(currentState() == Prom::UnStStoped)
            return DoneAlready;
        if(currentState() == Prom::UnStStopCommand)
            return DoneWhait;
        return DoneWhait;
        break;

    default : return RejAnnown;

    }
}

//------------------------------------------------------------------------------
void BurnerVSMKromyComb::setCombustor(Unit * Combustor)
{
    if(Combustor){
        _combustor = Combustor;
        addSubUnit(_combustor);
    }
}

//------------------------------------------------------------------------------
void BurnerVSMKromyComb::_customConnectToGUI(QObject *guiItem, QObject *propWin)
{
    QVariant ret;
    QObject * tmpSgSt, * engRow;
    //создал строку меню задержки старта топок
    QMetaObject::invokeMethod(propWin, "addEngRow", Qt::DirectConnection, Q_RETURN_ARG(QVariant, ret), Q_ARG(QVariant, this->objectName()));
    engRow = qvariant_cast< QObject* >(ret);
    QMetaObject::invokeMethod(engRow, "addPropertySetting", Qt::DirectConnection,
                              Q_RETURN_ARG(QVariant, ret),
                              Q_ARG(QVariant, this->objectName() + "_startCombdelay" ),
                              Q_ARG(QVariant, "сек. - задержка запуска горелки"),
                              Q_ARG(QVariant, 1));

    tmpSgSt = qvariant_cast< QObject* >(ret);//получаю указатель
    //подключаю сигналы
    connect(tmpSgSt, SIGNAL(changedVal(      QVariant) ), this,    SLOT(setStartCombDelay(QVariant)), Qt::QueuedConnection);
    connect(this,    SIGNAL(SetStartCombDelay(QVariant)), tmpSgSt, SLOT(changeVal        (QVariant)), Qt::QueuedConnection);
    connect(this->_combStartDelay,  SIGNAL(StartSig(QVariant)), tmpSgSt, SLOT(startCountdown(QVariant)),       Qt::QueuedConnection);
    connect(this->_combStartDelay,  SIGNAL(StopSig() ), tmpSgSt, SLOT(stopCountdown()),       Qt::QueuedConnection);
    connect(this->_combStartDelay,  SIGNAL(timeout() ), tmpSgSt, SLOT(stopCountdown()),       Qt::QueuedConnection);
    //---- Время старта топок ------


    //создал строку меню задержки остановки топок
    QMetaObject::invokeMethod(propWin, "addEngRow", Qt::DirectConnection, Q_RETURN_ARG(QVariant, ret), Q_ARG(QVariant, this->objectName()));
    engRow = qvariant_cast< QObject* >(ret);
    QMetaObject::invokeMethod(engRow, "addPropertySetting", Qt::DirectConnection,
                              Q_RETURN_ARG(QVariant, ret),
                              Q_ARG(QVariant, this->objectName() + "_stopCombDelay" ),
                              Q_ARG(QVariant, "сек. - задержка отключения топочных блоков"),
                              Q_ARG(QVariant, 1));

    tmpSgSt = qvariant_cast< QObject* >(ret);//получаю указатель
    //подключаю сигналы
    connect(tmpSgSt, SIGNAL(changedVal(      QVariant)), this,    SLOT(setStopCombDelay(QVariant)), Qt::QueuedConnection);
    connect(this,    SIGNAL(SetStopCombDelay(QVariant)), tmpSgSt, SLOT(changeVal       (QVariant)), Qt::QueuedConnection);
    connect(this->_combStopDelay,  SIGNAL(StartSig(QVariant)),            tmpSgSt, SLOT(startCountdown(QVariant)),       Qt::QueuedConnection);
    connect(this->_combStopDelay,  SIGNAL(StopSig() ), tmpSgSt, SLOT(stopCountdown()),       Qt::QueuedConnection);
    connect(this->_combStopDelay,  SIGNAL(timeout() ), tmpSgSt, SLOT(stopCountdown()),       Qt::QueuedConnection);
    //---- Время остановки топок ------

    BurnerVSM_Kromy::_customConnectToGUI(guiItem, propWin);
}

//------------------------------------------------------------------------------
void BurnerVSMKromyComb::saveParam()
{
    ini->setValue(tagPrefix + "/combStartDelay", _combStartDelay->interval() / 1000);
    ini->setValue(tagPrefix + "/combStopDelay", _combStopDelay->interval() / 1000);
    Unit::saveParam();
}

//------------------------------------------------------------------------------
void BurnerVSMKromyComb::loadParam()
{
    _combStartDelay->setInterval(ini->value(tagPrefix + "/combStartDelay", 60).toInt() * 1000);
    _combStopDelay->setInterval(ini->value(tagPrefix + "/combStopDelay", 60).toInt() * 1000);
    emit SetStartCombDelay(_combStartDelay->interval() / 1000);
    emit SetStopCombDelay(_combStopDelay->interval() / 1000);
    Unit::loadParam();
}

//------------------------------------------------------------------------------
void BurnerVSMKromyComb::_alarmDo()
{
    setMode(Prom::UnMdCollingStop, false);
}

//------------------------------------------------------------------------------
void BurnerVSMKromyComb::reInitialise()
{
    emit SetStartCombDelay(_combStartDelay->interval() / 1000);
    emit SetStopCombDelay(_combStopDelay->interval() / 1000);

    Unit::reInitialise();
}

//------------------------------------------------------------------------------
