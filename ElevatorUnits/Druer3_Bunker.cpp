#include "Druer3_Bunker.h"
#include "InDiscretETag.h"
#include <QSettings>

Druer3_Bunker::Druer3_Bunker(int ID,
                              QString Name,
                              QString TagPefix)
    :RouteUnit(Prom::TypeDryer3_Bunker,
                ID,
                Name,
                TagPefix,
                false,
                Prom::UnMdStop)
{
    _useMayResAlarm = true;
    _HL1 = new InDiscretETag(this, "Датчик верхнего уровня 1", ".HL1", true, false, true, false);
    _HL1->onlyChange();
    connect(_HL1, &InDiscretETag::s_valueChd,    this, &Druer3_Bunker::updateState,  Qt::QueuedConnection);
    connect(_HL1, &InDiscretETag::s_detected,   this, &Druer3_Bunker::ActivateHL1,   Qt::QueuedConnection);
    connect(_HL1, &InDiscretETag::s_undetected, this, &Druer3_Bunker::DeactivateHL1, Qt::QueuedConnection);

    _ML1 = new InDiscretETag(this, "Датчик среднего уровня 1", ".ML1", true, false, true, false);
    _ML1->onlyChange();
    connect(_ML1, &InDiscretETag::s_valueChd,    this, &Druer3_Bunker::updateState,  Qt::QueuedConnection);
    connect(_ML1, &InDiscretETag::s_detected,   this, &Druer3_Bunker::ActivateML1,   Qt::QueuedConnection);
    connect(_ML1, &InDiscretETag::s_undetected, this, &Druer3_Bunker::DeactivateML1, Qt::QueuedConnection);

    _LL1 = new InDiscretETag(this, "Датчик нижнего уровня 1", ".LL1", true, false, true, false);
    _LL1->onlyChange();
    connect(_LL1, &InDiscretETag::s_valueChd,    this, &Druer3_Bunker::updateState,  Qt::QueuedConnection);
    connect(_LL1, &InDiscretETag::s_detected,   this, &Druer3_Bunker::ActivateLL1,   Qt::QueuedConnection);
    connect(_LL1, &InDiscretETag::s_undetected, this, &Druer3_Bunker::DeactivateLL1, Qt::QueuedConnection);

    //----
    _HL2 = new InDiscretETag(this, "Датчик верхнего уровня 2", ".HL2", true, false, true, false);
    _HL2->onlyChange();
    connect(_HL2, &InDiscretETag::s_valueChd,    this, &Druer3_Bunker::updateState,  Qt::QueuedConnection);
    connect(_HL2, &InDiscretETag::s_detected,   this, &Druer3_Bunker::ActivateHL2,   Qt::QueuedConnection);
    connect(_HL2, &InDiscretETag::s_undetected, this, &Druer3_Bunker::DeactivateHL2, Qt::QueuedConnection);

    _ML2 = new InDiscretETag(this, "Датчик среднего уровня 2", ".ML2", true, false, true, false);
    _ML2->onlyChange();
    connect(_ML2, &InDiscretETag::s_valueChd,    this, &Druer3_Bunker::updateState,  Qt::QueuedConnection);
    connect(_ML2, &InDiscretETag::s_detected,   this, &Druer3_Bunker::ActivateML2,   Qt::QueuedConnection);
    connect(_ML2, &InDiscretETag::s_undetected, this, &Druer3_Bunker::DeactivateML2, Qt::QueuedConnection);

    _LL2 = new InDiscretETag(this, "Датчик нижнего уровня 2", ".LL2", true, false, true, false);
    _LL2->onlyChange();
    connect(_LL2, &InDiscretETag::s_valueChd,    this, &Druer3_Bunker::updateState,  Qt::QueuedConnection);
    connect(_LL2, &InDiscretETag::s_detected,   this, &Druer3_Bunker::ActivateLL2,   Qt::QueuedConnection);
    connect(_LL2, &InDiscretETag::s_undetected, this, &Druer3_Bunker::DeactivateLL2, Qt::QueuedConnection);

    //----
    _HL3 = new InDiscretETag(this, "Датчик верхнего уровня 3", ".HL3", true, false, true, false);
    _HL3->onlyChange();
    connect(_HL3, &InDiscretETag::s_valueChd,    this, &Druer3_Bunker::updateState,  Qt::QueuedConnection);
    connect(_HL3, &InDiscretETag::s_detected,   this, &Druer3_Bunker::ActivateHL3,   Qt::QueuedConnection);
    connect(_HL3, &InDiscretETag::s_undetected, this, &Druer3_Bunker::DeactivateHL3, Qt::QueuedConnection);

    _ML3 = new InDiscretETag(this, "Датчик среднего уровня 3", ".ML3", true, false, true, false);
    _ML3->onlyChange();
    connect(_ML3, &InDiscretETag::s_valueChd,    this, &Druer3_Bunker::updateState,  Qt::QueuedConnection);
    connect(_ML3, &InDiscretETag::s_detected,   this, &Druer3_Bunker::ActivateML3,   Qt::QueuedConnection);
    connect(_ML3, &InDiscretETag::s_undetected, this, &Druer3_Bunker::DeactivateML3, Qt::QueuedConnection);

    _LL3 = new InDiscretETag(this, "Датчик нижнего уровня 3", ".LL3", true, false, true, false);
    _LL3->onlyChange();
    connect(_LL3, &InDiscretETag::s_valueChd,    this, &Druer3_Bunker::updateState,  Qt::QueuedConnection);
    connect(_LL3, &InDiscretETag::s_detected,   this, &Druer3_Bunker::ActivateLL3,   Qt::QueuedConnection);
    connect(_LL3, &InDiscretETag::s_undetected, this, &Druer3_Bunker::DeactivateLL3, Qt::QueuedConnection);
}
//------------------------------------------------------------------------------
void Druer3_Bunker::_updateStateAndMode()
{
    bool alarmLvl = false;
    //    if(_LL1->isDetected()){
    //        if(! _ML1->isDetected())
    //            if(_HL1->isDetected())
    //                alarmLvl = true;
    //    }
    //    else {
    //        if(_ML1->isDetected() || _HL1->isDetected())
    //            alarmLvl = true;
    //    }

    //    if(_LL2->isDetected()){
    //        if(! _ML2->isDetected())
    //            if(_HL2->isDetected())
    //                alarmLvl = true;
    //    }
    //    else {
    //        if(_ML2->isDetected() || _HL2->isDetected())
    //            alarmLvl = true;
    //    }

    //    if(_LL3->isDetected()){
    //        if(! _ML3->isDetected())
    //            if(_HL3->isDetected())
    //                alarmLvl = true;
    //    }
    //    else {
    //        if(_ML3->isDetected() || _HL3->isDetected())
    //            alarmLvl = true;
    //    }

    static unsigned short int H {0};

    if(_ML1->isDetected() && _ML2->isDetected() && _ML3->isDetected()) {
        H = _HL1->isDetected() + _HL2->isDetected() + _HL3->isDetected();
        if(H >= 2) {
            if( H == 3 ) _setCurrentState(Prom::UnStHiLvl);
            else _setCurrentState(Prom::UnStMidLvl);
            _setCurrentMode(Prom::UnMdNeedUnload);
        }
        else _setCurrentState(Prom::UnStMidLvl);
    }
    else{
        if(_LL1->isDetected() && _LL2->isDetected() && _LL3->isDetected()) {
            _setCurrentState(Prom::UnStLowLvl);
        }
        else{
            _setCurrentState(Prom::UnStEmpty);
            emit s_belowLL();
        }
        _setCurrentMode(Prom::UnMdNeedLoad);
    }



//    if(_LL1->isDetected() && _LL2->isDetected() && _LL3->isDetected()) {

//        if(_ML1->isDetected() && _ML2->isDetected() && _ML3->isDetected()) {

//            H = _HL1->isDetected() + _HL2->isDetected() + _HL3->isDetected();
//            if(H > 1) {
//                _setCurrentState(Prom::UnStHiLvl);
//                _setCurrentMode(Prom::UnMdNeedUnload);
//            }
//            else {
//                _setCurrentState(Prom::UnStMidLvl);
//                //_setCurrentMode(Prom::UnMdNeedLoad);
//            }
//        }
//        else{
//            _setCurrentState(Prom::UnStLowLvl);
//            _setCurrentMode(Prom::UnMdNeedLoad);
//        }
//    }
//    else{
//        //if(! (_HL1->isDetected() || _HL2->isDetected() || _HL3->isDetected())) {
//        _setCurrentState(Prom::UnStEmpty);
//        _setCurrentMode(Prom::UnMdNeedLoad);
//        // }
//    }
    if(alarmLvl && ! _lvlAlarm){
        _lvlAlarm = true;
        _setCurrentMode(Prom::UnMdNeedLoad);
        detectAlarm("отказ одного из датчиков уровня, либо затор продукта");
    }
    else {
        _lvlAlarm = false;
    }
}

//------------------------------------------------------------------------------
Prom::SetModeResp Druer3_Bunker::_customSetMode(Prom::UnitModes *mode, bool)
{
    if(currentMode() == *mode) return Prom::DoneAlready;
    else{
        switch (*mode) {
        case Prom::UnMdNeedLoad  :
        case Prom::UnMdNeedUnload :
            _setSetedMode(*mode);
            return Prom::DoneWhait;
        default : return RejAnnown;
        }
    }
    return RejAnnown;
}
//------------------------------------------------------------------------------
Prom::UnitModes Druer3_Bunker::_ModeOfCommand(Prom::RouteCommand *Command)
{
    using namespace Prom;
    switch (*Command) {
    case RtCmNo               : return UnMdNoDef;
    case RtCmStop             :
    case RtCmToRoute          :
    case RtCmCleanStart       :
    case RtCmToRoutForClean   : return _routeMode;
    case RtCmCleanStop        :
    case RtCmToSave           :
    case RtCmStopOnRouteAlarm : return currentMode();
    default: return currentMode();
    }
}

//------------------------------------------------------------------------------
bool Druer3_Bunker::_resetAlarm(bool upClassAlarm)
{
    return RouteUnit::_resetAlarm(upClassAlarm || _lvlAlarm );
}
//------------------------------------------------------------------------------
void Druer3_Bunker::_customConnectToGUI(QObject *guiItem,  QObject *)
{
    connect(this, SIGNAL(ActivateHL1()  ), guiItem, SLOT(activateHL1()  ),   Qt::QueuedConnection);
    connect(this, SIGNAL(DeactivateHL1()), guiItem, SLOT(deactivateHL1()),   Qt::QueuedConnection);
    connect(this, SIGNAL(ActivateML1()  ), guiItem, SLOT(activateML1()  ),   Qt::QueuedConnection);
    connect(this, SIGNAL(DeactivateML1()), guiItem, SLOT(deactivateML1()),   Qt::QueuedConnection);
    connect(this, SIGNAL(ActivateLL1()  ), guiItem, SLOT(activateLL1()  ),   Qt::QueuedConnection);
    connect(this, SIGNAL(DeactivateLL1()), guiItem, SLOT(deactivateLL1()),   Qt::QueuedConnection);

    connect(this, SIGNAL(ActivateHL2()  ), guiItem, SLOT(activateHL2()  ),   Qt::QueuedConnection);
    connect(this, SIGNAL(DeactivateHL2()), guiItem, SLOT(deactivateHL2()),   Qt::QueuedConnection);
    connect(this, SIGNAL(ActivateML2()  ), guiItem, SLOT(activateML2()  ),   Qt::QueuedConnection);
    connect(this, SIGNAL(DeactivateML2()), guiItem, SLOT(deactivateML2()),   Qt::QueuedConnection);
    connect(this, SIGNAL(ActivateLL2()  ), guiItem, SLOT(activateLL2()  ),   Qt::QueuedConnection);
    connect(this, SIGNAL(DeactivateLL2()), guiItem, SLOT(deactivateLL2()),   Qt::QueuedConnection);

    connect(this, SIGNAL(  ActivateHL3()), guiItem, SLOT(activateHL3()  ),   Qt::QueuedConnection);
    connect(this, SIGNAL(DeactivateHL3()), guiItem, SLOT(deactivateHL3()),   Qt::QueuedConnection);
    connect(this, SIGNAL(  ActivateML3()), guiItem, SLOT(activateML3()  ),   Qt::QueuedConnection);
    connect(this, SIGNAL(DeactivateML3()), guiItem, SLOT(deactivateML3()),   Qt::QueuedConnection);
    connect(this, SIGNAL(  ActivateLL3()), guiItem, SLOT(activateLL3()  ),   Qt::QueuedConnection);
    connect(this, SIGNAL(DeactivateLL3()), guiItem, SLOT(deactivateLL3()),   Qt::QueuedConnection);

    //connect(guiItem, SIGNAL(addToCurrentRoteNeedUnload()),this, SLOT(AddToCurrentRoteNeedUnload()),   Qt::QueuedConnection);
    //connect(guiItem, SIGNAL(addToCurrentRoteNeedLoad()),  this, SLOT(AddToCurrentRoteNeedLoad()),     Qt::QueuedConnection);
}

//------------------------------------------------------------------------------
