
//#include <qdebug.h>
#include <qsettings.h>
#include <qdebug.h>
#include "unit.h"
#include "InDiscretETag.h"
#include "OutDiscretETag.h"
#include "electroengine.h"

using namespace Prom;


ElectroEngine::ElectroEngine(Unit *Owner, std::vector<QString> SensorsNames, QString MultiEnginePrefix)
    :QObject(Owner), _owner(Owner)
{
    setObjectName(Owner->objectName() + MultiEnginePrefix + " эл.двигатель");

    if(SensorsNames.size() > 0) {
        if(SensorsNames[1] == "") { //---------------------------------------------- НЕ реверсивный двигатель
            //            bool B = SensorsNames[0].section('|', 2, 1) == "i";
            //            QString S = SensorsNames[0].section('|', 2, 1);
            startForward = new OutDiscretETag(_owner, Prom::PreSet, SensorsNames[0].section('|', 0, 0),
                    MultiEnginePrefix + SensorsNames[0].section('|', 1, 1),
                    true, false, false, true, false, false, false,false,
                    SensorsNames[0].section('|', 2, 2) == "i"); //,

            KMforward = new InDiscretETag(_owner, SensorsNames[2].section('|', 0, 0),
                    MultiEnginePrefix + SensorsNames[2].section('|', 1, 1),
                    true, false, SensorsNames[2].section('|', 2, 2).toInt()); //

            connect(startForward, &OutDiscretETag::s_valueChd, this,      &ElectroEngine::CheckStateSTART1KM,     Qt::QueuedConnection);
            connect(KMforward,    &InDiscretETag::s_valueChd,  this,      &ElectroEngine::CheckStateSTART1KM,     Qt::QueuedConnection);
            connect(startForward, &OutDiscretETag::s_on,      KMforward, &InDiscretETag::needBeDetectedAlarm,     Qt::QueuedConnection);
            connect(startForward, &OutDiscretETag::s_off,     KMforward, &InDiscretETag::needBeUndetectedNoAlarm, Qt::QueuedConnection);
            connect(_owner,       &Unit::s_connected,         this,      &ElectroEngine::CheckStateSTART1KM,      Qt::QueuedConnection);
            if(startForward->isOn())
                KMforward->needBeDetectedAlarm();
            else
                KMforward->needBeUndetectedNoAlarm();
        }
        else if(SensorsNames[1].contains(".stop")) {   //---------------------------------------------- НЕ реверсивный двигатель с раздельными Старт и Стоп сигналами

            startForward = new OutDiscretETag(_owner, Prom::PreSet,
                                              SensorsNames[0].section('|', 0, 0),
                    MultiEnginePrefix + SensorsNames[0].section('|', 1, 1),
                    true, false, false, true, false, false, false,false,
                    SensorsNames[0].section('|', 2, 2) == "i"); //


            if(SensorsNames[ 2 ] == ""){

                stop = new OutDiscretETag(_owner, Prom::PreSet,
                                          SensorsNames[1].section('|',0, 0),
                        MultiEnginePrefix + SensorsNames[1].section('|', 1, 1)); //,
                connect(startForward, &OutDiscretETag::s_valueChd, this,      &ElectroEngine::CheckStateSartStop,   Qt::QueuedConnection);
                connect(stop,         &OutDiscretETag::s_valueChd, this,      &ElectroEngine::CheckStateSartStop,   Qt::QueuedConnection);
                connect(_owner,       &Unit::s_connected,          this,      &ElectroEngine::CheckStateSartStop,   Qt::QueuedConnection);
            }
            else{
                stop = new OutDiscretETag(_owner, Prom::PreSet,
                                          SensorsNames[1].section('|',0, 0),
                        MultiEnginePrefix + SensorsNames[1].section('|', 1, 1),
                        true, false, false, true, false, false, false,false,
                        SensorsNames[1].section('|', 2, 2) == "i"); //,
                KMforward = new InDiscretETag(_owner, SensorsNames[2].section('|', 0, 0), MultiEnginePrefix + SensorsNames[2].section('|', 1, 1), 1, 0, SensorsNames[2].section('|', 2, 2).toInt()); //,

                connect(startForward, &OutDiscretETag::s_valueChd, this,      &ElectroEngine::CheckStateSartStop1KM,   Qt::QueuedConnection);
                connect(stop,         &OutDiscretETag::s_valueChd, this,      &ElectroEngine::CheckStateSartStop1KM,   Qt::QueuedConnection);
                connect(KMforward,    &InDiscretETag::s_valueChd,  this,      &ElectroEngine::CheckStateSartStop1KM,   Qt::QueuedConnection);
                connect(startForward, &OutDiscretETag::s_on,       KMforward, &InDiscretETag::needBeDetectedAlarm,     Qt::QueuedConnection);
                if(startForward->tunableImpulseTime)
                    connect(stop, &OutDiscretETag::s_on, KMforward, &InDiscretETag::needBeUndetectedNoAlarm, Qt::QueuedConnection);
                else
                    connect(startForward, &OutDiscretETag::s_off, KMforward, &InDiscretETag::needBeUndetectedNoAlarm, Qt::QueuedConnection);

                connect(_owner,       &Unit::s_connected,         this,      &ElectroEngine::CheckStateSartStop1KM, Qt::QueuedConnection);
                if(startForward->isOn())KMforward->needBeDetectedAlarm();
                else KMforward->needBeUndetectedNoAlarm();

            }
        }
        else if( SensorsNames[1] != "") {   //++++++++++++++++++++++++++++++++++++++ реверсивный двигатель

            startForward = new OutDiscretETag(_owner, Prom::PreSet,
                                              SensorsNames[0].section('|', 0, 0),
                    MultiEnginePrefix + SensorsNames[0].section('|', 1, 1)); //,

            //+++++++++ тег обратного вращения
            startBackward = new OutDiscretETag(_owner, Prom::PreSet,
                                               SensorsNames[1].section('|', 0, 0),
                    MultiEnginePrefix + SensorsNames[1].section('|', 1, 1)); //,

            if(SensorsNames[2] != "" && SensorsNames[3] == "") { // +++++++ если контактор один
                KM = new InDiscretETag(_owner, SensorsNames[2].section('|', 0, 0),
                        MultiEnginePrefix + SensorsNames[2].section('|', 1, 1),
                        true, false, SensorsNames[2].section('|', 2, 2).toInt()); //,



                connect(KM, &InDiscretETag::s_valueChd, this, &ElectroEngine::CheckStateREVERSE1KM, Qt::QueuedConnection);
                connect(startForward, &OutDiscretETag::s_valueChd, this, &ElectroEngine::CheckStateREVERSE1KM, Qt::QueuedConnection);
                connect(startBackward, &OutDiscretETag::s_valueChd,  this, &ElectroEngine::CheckStateREVERSE1KM, Qt::QueuedConnection);

                connect(startForward,  &OutDiscretETag::s_on, KM,  &InDiscretETag::needBeDetectedAlarm, Qt::QueuedConnection);
                connect(startForward,  &OutDiscretETag::s_off,KM,  &InDiscretETag::needBeUndetectedNoAlarm, Qt::QueuedConnection);

                connect(startBackward, &OutDiscretETag::s_on, KM,  &InDiscretETag::needBeDetectedAlarm, Qt::QueuedConnection);
                connect(startBackward, &OutDiscretETag::s_off,KM,  &InDiscretETag::needBeUndetectedNoAlarm, Qt::QueuedConnection);
                connect(_owner, &Unit::s_connected, this, &ElectroEngine::CheckStateREVERSE1KM, Qt::QueuedConnection);

                if(startForward->isOn())KM->needBeDetectedAlarm();
                else KM->needBeUndetectedNoAlarm();
                if(startBackward->isOn())KM->needBeDetectedAlarm();
                else KM->needBeUndetectedNoAlarm();
            }
            else if (SensorsNames[2] != "" && SensorsNames[3] != "") {   //+++++ если контакторов два

                KMforward = new InDiscretETag(_owner,SensorsNames[2].section('|', 0, 0),
                        MultiEnginePrefix + SensorsNames[2].section('|', 1, 1),
                        true, false, SensorsNames[2].section('|', 2, 2).toInt()); //,

                connect(KMforward,    &InDiscretETag::s_valueChd, this,      &ElectroEngine::CheckStateREVERSE2KM, Qt::QueuedConnection);
                connect(startForward, &OutDiscretETag::s_on,  KMforward, &InDiscretETag::needBeDetectedAlarm, Qt::QueuedConnection);
                connect(startForward, &OutDiscretETag::s_off, KMforward, &InDiscretETag::needBeUndetectedNoAlarm, Qt::QueuedConnection);
                if(startForward->isOn())KMforward->needBeDetectedAlarm();
                else KMforward->needBeUndetectedNoAlarm();
                //----
                KMbackward = new InDiscretETag(_owner,  SensorsNames[3].section('|', 0, 0),
                        MultiEnginePrefix + SensorsNames[3].section('|', 1, 1),
                        true, false, SensorsNames[3].section('|', 2, 2).toInt()); //,

                connect(KMbackward,    &InDiscretETag::s_valueChd, this,       &ElectroEngine::CheckStateREVERSE2KM, Qt::QueuedConnection);
                connect(startBackward, &OutDiscretETag::s_on,  KMbackward, &InDiscretETag::needBeDetectedAlarm, Qt::QueuedConnection);
                connect(startBackward, &OutDiscretETag::s_off, KMbackward, &InDiscretETag::needBeUndetectedNoAlarm, Qt::QueuedConnection);

                connect(startForward, &OutDiscretETag::s_valueChd, this, &ElectroEngine::CheckStateREVERSE2KM, Qt::QueuedConnection);
                connect(startBackward, &OutDiscretETag::s_valueChd,  this, &ElectroEngine::CheckStateREVERSE2KM, Qt::QueuedConnection);
                connect(_owner, &Unit::s_connected, this, &ElectroEngine::CheckStateREVERSE2KM, Qt::QueuedConnection);
                if(startBackward->isOn())KMbackward->needBeDetectedAlarm();
                else KMbackward->needBeUndetectedNoAlarm();
            }
            else if (SensorsNames[2] == "" && SensorsNames[3] == "") {   //+++++ если нет контакторов
                connect(startForward, &OutDiscretETag::s_valueChd, this, &ElectroEngine::CheckStateREVERSE0KM, Qt::QueuedConnection);
                connect(startBackward, &OutDiscretETag::s_valueChd,  this, &ElectroEngine::CheckStateREVERSE0KM, Qt::QueuedConnection);
                connect(_owner, &Unit::s_connected, this, &ElectroEngine::CheckStateREVERSE0KM, Qt::QueuedConnection);
            }
        } //---------------------------------------------- реверсивный двигатель
        if(SensorsNames.capacity() > 4 ?  SensorsNames[4] != "" : false) { //++++++ есть QK
            QK = new InDiscretETag(_owner, SensorsNames[4].section('|', 0, 0),
                    MultiEnginePrefix + SensorsNames[4].section('|', 1, 1),
                    true, false, SensorsNames[4].section('|', 2, 2).toInt(), false);
            QK->needBeDetectedAlarmNoTime();
        }
        //tmp = QString::number(SensorsNames.capacity());

        if(SensorsNames.capacity() > 5 ?  SensorsNames[5] != "" : false) { //+++++++ есть стоп кнопка
            AS = new InDiscretETag(_owner,  SensorsNames[5].section('|', 0, 0),
                    MultiEnginePrefix + SensorsNames[5].section('|', 1, 1),
                    true, false, SensorsNames[5].section('|', 2, 2).toInt(), false);
            AS->needBeDetectedAlarmNoTime();
        }
    }
}

//------------------------------------------------------------------------------
bool ElectroEngine::StartForward()
{
    if(stop != nullptr)
        stop->off();
    if(_nowState == EngStoped && ! _owner->isAlarm())
        return startForward->on();
    else if(_nowState == EngForvard || _nowState == EngToForvard) return true;
    else return false;
}

//------------------------------------------------------------------------------
bool ElectroEngine::StartBackward()
{
    if(_nowState == EngStoped && ! _owner->isAlarm()) return startBackward->on();
    return false;
}

//------------------------------------------------------------------------------
void ElectroEngine::Stop()
{
    startForward->off();
    if(startBackward != nullptr) {
        startBackward->off();
    }
    if(stop != nullptr) stop->on();
}

//------------------------------------------------------------------------------
void ElectroEngine::CheckStateSTART1KM()
{
    QObject * S =  sender();
    EngineStates tmpSt = _nowState;
    if(startForward->isOn()) {
        if(KMforward->isDetected()) _nowState = EngForvard;
        else {
            _nowState = EngToForvard;
            /*TEST*/if(KMforward->isImit() && startForward->isImit())
                /*TEST*/QTimer::singleShot(1000, [ this ]() {
                this->KMforward->writeImitVal(true);
            });///
        }
    }
    else {
        if(KMforward->isDetected()) {
            if(_nowState == EngForvard || _nowState == EngToForvard) {
                _nowState = EngToStopForward;
                /*TEST*/if(KMforward->isImit() && startForward->isImit())
                    /*TEST*/QTimer::singleShot(1000, [ this ]() {
                    this->KMforward->writeImitVal(false);
                });///
            }
            else _nowState = EngManualForward;
        }
        else _nowState = EngStoped;
    }
    if(tmpSt != _nowState) {
        _prevState = tmpSt;
    }
    emit StateUpdate();
}

//------------------------------------------------------------------------------
void ElectroEngine::CheckStateSartStop1KM()
{
    static EngineStates tmpSt;
    tmpSt = _nowState;
    if(startForward->isOn()) {
        if(stop->isOn()){
            startForward->off();
        }
        else if(KMforward->isDetected()) {
            _nowState = EngForvard;
            //startForward->Off();
        }
        else {
            _nowState = EngToForvard;
            /*TEST*/if(KMforward->isImit() && startForward->isImit())
                /*TEST*/QTimer::singleShot(1000, [ this ]() { this->KMforward->writeImitVal(true); });
        }
    }
    else if(stop->isOn()) {
        if(KMforward->isDetected()) {
            _nowState = EngToStopForward;
            /*TEST*/if(KMforward->isImit() && startForward->isImit())
                /*TEST*/QTimer::singleShot(1000, [ this ](){ this->KMforward->writeImitVal(false); });///
        }
        else {
            _nowState = EngToStopForward;
            stop->off();
        }
    }
    else  {
        if(KMforward->isDetected()) {
            if(_nowState == EngForvard || _nowState == EngToForvard) {
                _nowState = EngForvard;
            }
            else _nowState = EngManualForward;
        }
        else _nowState = EngStoped;
    }

    if(tmpSt != _nowState) {
        _prevState = tmpSt;
    }
    emit StateUpdate();
}

//------------------------------------------------------------------------------
void ElectroEngine::CheckStateSartStop()
{
    static EngineStates tmpSt;
    tmpSt = _nowState;
    if(startForward->isOn()) {
        if(stop->isOn()){
            startForward->off();
        }
        else {
            _nowState = EngForvard;
            //startForward->Off();
        }
    }
    else if(stop->isOn()) {
        //QTimer::singleShot(5000, [ this ]() { this->stop->Off(); });
        _nowState = EngStoped;
        //stop->Off();
    }
    else {
        _nowState = EngStoped;
    }
    if(tmpSt != _nowState) {
        _prevState = tmpSt;
    }
    emit StateUpdate();
}

//else  {
//    if(KMforward->isDetected()) {
//        if(_nowState == EngForvard || _nowState == EngToForvard) {
//            _nowState = EngForvard;
//        }
//        else _nowState = EngManualForward;
//    }
//    else _nowState = EngStoped;
//}
//------------------------------------------------------------------------------
void ElectroEngine::CheckStateREVERSE0KM()
{
    EngineStates tmpSt = _nowState;
    _alarm = false;
    if(startForward->isOn() && ! startBackward->isOn())  _nowState = EngForvard;
    else if(startBackward->isOn() && !startForward->isOn()) _nowState = EngBackward;
    else if((!startBackward->isOn())&& !startForward->isOn()) _nowState = EngStoped;
    else {
        _nowState = EngAlarm;
        _alarm = true;
        _alarmDescription = "включение обоих управляющих тэгов";
    }
    if(tmpSt != _nowState)
        _prevState = tmpSt;

    emit StateUpdate();
}

//------------------------------------------------------------------------------
void ElectroEngine::CheckStateREVERSE1KM()
{
    EngineStates tmpSt = _nowState;
    _alarm = false;
    if(startForward->isOn() && ! startBackward->isOn()) {
        if(KM->isDetected()) _nowState = EngForvard;
        else _nowState = EngToForvard;
    }
    else if((!startForward->isOn()) && startBackward->isOn() ) {
        if(KM->isDetected()) _nowState = EngBackward;
        else _nowState = EngToBackward;
    }
    else if((!startForward->isOn()) && !startBackward->isOn() ) {
        if(KM->isDetected()) {
            if(_nowState == EngForvard || _nowState == EngToForvard) {
                _nowState = EngToStopForward;
            }
            else if(_nowState == EngBackward || _nowState == EngToBackward)
                _nowState = EngToStopBackward;
            else _nowState = EngManualMove;
        }
        else _nowState = EngStoped;
    }
    else {
        _nowState = EngAlarm;
        _alarm = true;
        _alarmDescription = "включение обоих управляющих тэгов";
    }

    if(tmpSt != _nowState) {
        _prevState = tmpSt;
    }
    emit StateUpdate();
}
//------------------------------------------------------------------------------
void ElectroEngine::CheckStateREVERSE2KM()
{
    EngineStates tmpSt = _nowState;
    _alarm = false;
    if(startForward->isOn() && ! startBackward->isOn()) {
        if(KMforward->isDetected()) _nowState = EngForvard;
        else {
            _nowState = EngToForvard;
            /*TEST*/if(KMforward->isImit() && startForward->isImit()) QTimer::singleShot(1000, [ this ]() {
                this->KMforward->writeImitVal(true);
            });///
        }
    }
    else if((!startForward->isOn()) && startBackward->isOn()) {
        if(KMbackward->isDetected()) _nowState = EngBackward;
        else {
            _nowState = EngToBackward;
            /*TEST*/if(KMbackward->isImit() && startBackward->isImit()) QTimer::singleShot(1000, [ this ]() {
                this->KMbackward->writeImitVal(true);
            });///
        }
    }
    else if((!startBackward->isOn()) && !startForward->isOn()) {
        if(KMforward->isDetected() && !KMbackward->isDetected()) {
            if(_nowState == EngForvard || _nowState == EngToForvard) {
                _nowState = EngToStopForward;
                /*TEST*/if(KMforward->isImit() && startForward->isImit()) QTimer::singleShot(1000, [ this ]() {
                    this->KMforward->writeImitVal(false);
                });///
            }
            else _nowState = EngManualForward;
        }
        else if (!KMforward->isDetected() && KMbackward->isDetected()) {
            if(_nowState == EngBackward || _nowState == EngToBackward) {
                _nowState = EngToStopBackward;
                /*TEST*/if(KMbackward->isImit() && startBackward->isImit()) QTimer::singleShot(1000, [ this ]() {
                    this->KMbackward->writeImitVal(false);
                });///
            }
            else _nowState = EngManualBackward;
        }
        else if (!KMforward->isDetected() && !KMbackward->isDetected())_nowState = EngStoped;
        else {
            _nowState = EngAlarm;
            _alarm = true;
            _alarmDescription = "включение обоих контакторов при выключенных управляющих тэгах";
        }
    }
    else {
        _nowState = EngAlarm;
        _alarm = true;
        _alarmDescription = "включение обоих управляющих тэгов";
    }

    if(tmpSt != _nowState)
        _prevState = tmpSt;
    emit StateUpdate();
}
//------------------------------------------------------------------------------

