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
        if(_combustor->currentMode() != UnMdStart){
            detectAlarm( "Включение горелки без топочного блока!" );
        }
        _combStopDelay->stop();
        if(_relayKM->isDetected()){
            _combStopDelay->stop();
            _setCurrentState(UnStStarted);
            _setCurrentMode(UnMdStart);
        }
        else{
            _setCurrentState(UnStStartCommand);
            if(_relayKM->isImit() && _start->isImit())
                QTimer::singleShot(100, [ this ]() {
                    if(setedMode() == UnMdStart){
                        _relayKM->writeImitVal(true);
                        _startSig->writeImitVal(true);
                    }
                });
        }
    }
    else{
        if(_relayKM->isDetected()){
            _setCurrentState(UnStStopCommand);
            if(_relayKM->isImit() && _start->isImit())
                QTimer::singleShot(100, [ this ]()
                {
                    if(setedMode() == UnMdStop || setedMode() == UnMdCollingStop || setedMode() == UnMdColling){
                        _relayKM->writeImitVal(false);
                        _startSig->writeImitVal(false);
                    }
                });
        }
        else{
            if( _combustor->currentMode() == UnMdStart ){
                if(!_combStopDelay->isActive())
                    _combStopDelay->start();
                if( setedMode() == UnMdStart){
                    if(!_combStartDelay->isActive())
                        _combStartDelay->start();
                    _setCurrentMode(UnMdColling, false);
                }
                else if(setedMode() == UnMdColling){
                    _setCurrentMode(UnMdColling);
                    _setCurrentState(UnStColling);
                }
                else if( setedMode() == UnMdCollingStop ){
                    _setCurrentMode(UnMdColling, false);
                    _setCurrentState(UnStCollingStopCommand);
                }
                else {
                    _setCurrentMode(UnMdColling);
                    _setCurrentState(UnStColling);
                }
            }
            else if(_combustor->currentMode() == UnMdStop){
                if( _combStopDelay->isActive() ){
                    _setCurrentMode( UnMdStop );
                    _setCurrentState( UnStStoped );
                }
                else{
                    _setCurrentMode( UnMdCollingStop );
                    _setCurrentState( UnStCollingStoped );
                }
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
    if(setedMode() == UnMdCollingStop){
        if(_combustor && _combustor->isCurrOrSetedMode(UnMdStart))
            _combustor->setMode(UnMdStop, false);
    }
    _combustor->setBlocked(false);
}

//------------------------------------------------------------------------------
bool BurnerVSMKromyComb::addSubUnit(Unit * unit)
{
    if(unit->unitType == TypeCombustor){
        _combustor = unit;
        connect(_combustor, &Unit::s_modeChange, this, &BurnerVSMKromyComb::_updateStateAndMode, Qt::QueuedConnection);
    }
    return BurnerVSM_Kromy::addSubUnit(unit);
}

//------------------------------------------------------------------------------
void BurnerVSMKromyComb::_updateSubUnitState(Unit * unit)
{

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
        return  RejNoCond;
    }

    switch (*mode) {
    case UnMdStart :
        if(_blocked)
            return RejNoCond;
        if(_alarm)
            return RejAlarm;
        if(currentState() == UnStStarted)
            return DoneAlready;
        //        if(setedMode() == UnMdStart)
        //            return DoneWhait;
        allTimerStop();
        if(_combustor){
            switch (_combustor->setMode(UnMdStart, UserOrSys)) {
            case DoneAlready:
                if(_combStartDelay->isActive())
                    return DoneWhait;
                else if(_start->on()){
                    _combustor->setBlocked(true);
                    return DoneWhait;
                }
                else {
                    return RejNoCond;
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

    case UnMdCollingStop:
        if( currentMode() == UnMdColling ){
            if(_combStopDelay->isActive()){
                return DoneWhait;
            }
            else{
                return _combustor->setMode( UnMdStop, UserOrSys );
            }
        }
    case UnMdColling:
        _combStartDelay->stop();
        if( _start->isOn() ){
            _start->off();
            return DoneWhait;
        }
        else
            return _combustor->setMode( UnMdStart, UserOrSys );
        break;

    case UnMdStop :
        _combStartDelay->stop();
        _start->off();
        if(_combustor) _combustor->setMode(UnMdStop, UserOrSys);
        if(currentState() == UnStStoped)
            return DoneAlready;
        if(currentState() == UnStStopCommand)
            return DoneWhait;
        return DoneWhait;
        break;

    default : return RejAnnown;

    }
    return RejAnnown;
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
    if( currentModeIn({ UnMdStart })){
        setMode(UnMdColling, false);
    }
}

//------------------------------------------------------------------------------
void BurnerVSMKromyComb::reInitialise()
{
    emit SetStartCombDelay(_combStartDelay->interval() / 1000);
    emit SetStopCombDelay(_combStopDelay->interval() / 1000);

    Unit::reInitialise();
}

//------------------------------------------------------------------------------
