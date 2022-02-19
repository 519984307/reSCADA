#include <QtDebug>
#include <QSound>
#include <QSettings>
#include <QMediaPlayer>
#include <QCoreApplication>//подключаем ядро Simargl SCADA
#include <QThread>
//#include <QQmlContext>

#include "promobject.h"
#include "unit.h"
#include "route.h"
#include "log.h"
#include "TSP/tsp.h"
#include "TSP/gui.h"
#include "ETag.h"
#include "InETag.h"

TSP * g_TSP;
Route * g_currentRoute = nullptr;
QSettings * g_unitINI = nullptr;

//------------------------------------------------------------------------------
PromObject::PromObject(/*QQmlContext *QMLcontext,*/
                       QObject *GUI,
                       QObject *parent)
    : QObject(parent),
      //      _QMLcontext(QMLcontext),
      _gui(GUI)
{
    //_QMLcontext->setContextProperty("promCore", this);
    pthread_mutex_init(&mutex, NULL);
    _alarmSoundF = new QSound(":/alarm.wav", this);

    _logThread = new LogThread();
    _logThread->start();
    connect(this, &PromObject::s_logging, _logThread->LogMaster, &Log::Logging, Qt::QueuedConnection);
    Logging(Prom::MessInfo, QDateTime::currentDateTime(), false, "SCADA", "ВКЛЮЧЕНИЕ");

    _currentRoute = nullptr;
    iniUnit =  new QSettings("settings.ini", QSettings::IniFormat);
    iniUnit->setIniCodec(QTextCodec::codecForName("UTF-8"));

    g_unitINI = iniUnit;
    iniRoute =  new QSettings("routs.ini", QSettings::IniFormat);
    iniRoute->setIniCodec(QTextCodec::codecForName("UTF-8"));

    _unitsThread = new QThread();
    _unitsThread->setObjectName("Units thread");
    _unitsThread->start();

    _routeThread = new QThread();
    _routeThread->setObjectName("Route thread");
    _routeThread->start();

    tsp = new TSP(nullptr, "TSP");
    g_TSP = tsp;
    _tspWin = new TSPWindow(tsp, nullptr, TSPWindow::DefaultOptions);

    //connect(tsp, SIGNAL(LoggingSig(Prom::MessType, QDateTime, bool, QString, QString)), this,
    //         SLOT(Logging(Prom::MessType, QDateTime, bool, QString, QString)), Qt::QueuedConnection);
    _createUnits();
}

//------------------------------------------------------------------------------
void PromObject::addUnit(Unit * Un)
{
    if(Un){
        //qDebug() << Un->tagPrefix <<" init";
        Logging(Prom::MessInfo, QDateTime::currentDateTime(), false, Un->objectName(), "инициализация");
        _units.append(Un);
        //connect(this, SIGNAL(GlobalRescanSig()),       Un,   SLOT(RescanUnit()),  Qt::QueuedConnection);
        connect(this, SIGNAL(s_globalRestAlarm()),       Un,   SLOT(resetAlarm()),  Qt::QueuedConnection);
        connect(Un, &Unit::s_alarm, this, &PromObject::alarmDo,  Qt::QueuedConnection);
        connect(Un, &Unit::s_loggingSig, this, &PromObject::Logging,  Qt::QueuedConnection);

        if(Un->unitType != Prom::TypePLC
                && Un->unitType != Prom::TypeSilos
                && Un->unitType != Prom::TypeMMC)
            connect(this, SIGNAL(s_globalStop(bool)), Un,   SLOT(freeze(bool)),Qt::QueuedConnection);

        //connectToGUI(Un);
        Un->ini = iniUnit;
        Un->setOwner( this );
        Un->moveToThread(_unitsThread);

        //        if(_QMLcontext != nullptr){
        //            _QMLcontext->setContextProperty(Un->tagPrefix, Un);
        //        }
    }
}

//------------------------------------------------------------------------------
void PromObject::addSubUnit(Unit *Un)
{
    if(Un){
        pthread_mutex_lock(&mutex);
        _units.append(Un);
        Un->setOwner( this );
        pthread_mutex_unlock(&mutex);
        //        if(_QMLcontext != nullptr){
        //            _QMLcontext->setContextProperty(Un->tagPrefix, Un);
        //        }
    }
}

//------------------------------------------------------------------------------
PromObject::~PromObject()
{
    _unitsThread->exit();
    _routeThread->exit();
    saveSettings();
    foreach(Route * Rt, _RouteList){
        delete Rt;
    }
    foreach(Unit * Un, _units){
        delete Un;
    }
    delete iniUnit;
    delete iniRoute;
    delete _unitsThread;
    delete _routeThread;
    tsp->stop();
    _tspWin->close();
    delete tsp;
    delete _tspWin;
    Logging(Prom::MessInfo, QDateTime::currentDateTime(), false, "SCADA", "ВЫКЛЮЧЕНИЕ");
    pthread_mutex_destroy(&mutex);
    _logThread->quit();
    _logThread->deleteLater();
}

//------------------------------------------------------------------------------
bool PromObject::SetCurrentRoute(Route *pRoute)
{
    if (pRoute != nullptr) {
        _currentRoute = pRoute;
        g_currentRoute = pRoute;
        return true;
    }
    return false;

}
//------------------------------------------------------------------------------
void PromObject::MegaRescan()
{
    //qDebug()<<"emit GlobalRescan();";
    emit s_globalRescanSig();
}

//------------------------------------------------------------------------------
void PromObject::alarmDo()
{
    if(_alarmSoundF && _alarmSoundF->isFinished()) {
//        _alarmSoundF->setLoops(3);
//        _alarmSoundF->play();
    }
}

//------------------------------------------------------------------------------
void PromObject::GlobalRescan()
{
    emit s_globalRescanSig();
}

//------------------------------------------------------------------------------
void PromObject::Logging(Prom::MessType MessTypeID,  QDateTime DateTime, bool UserOrSys, QString Source, QString Message)
{
    if(MessTypeID == Prom::MessAlarm /*|| MessTypeID == Prom::MessQuitAlarm  || MessTypeID == Prom::MessWarning*/)
        emit s_addMessage(DateTime.toString("yyyy.MM.dd"), DateTime.toString("hh:mm:ss"), Source, Message);
    emit s_logging(MessTypeID, DateTime, UserOrSys ? "Оператор" : "Система", Source, Message);
    //qDebug()<< DateTime << Source + Message;
}
//------------------------------------------------------------------------------
void PromObject::saveSettings(QString FileName)
{
    //tsp->saveJson();
    if( FileName != "" ){
        if( iniUnit != nullptr ){
            iniUnit->sync();
            delete iniUnit;
        }
        iniUnit =  new QSettings(FileName, QSettings::IniFormat);
        iniUnit->setIniCodec(QTextCodec::codecForName("UTF-8"));
        foreach(Unit * Un, _units){
            Un->saveParam();
        }
        iniUnit->sync();
        delete iniUnit;
        iniUnit =  new QSettings("settings.ini", QSettings::IniFormat);
        iniUnit->setIniCodec(QTextCodec::codecForName("UTF-8"));
    }
    else {
        foreach(Unit * Un, _units){
            Un->saveParam();
        }
    }
}

//------------------------------------------------------------------------------
void PromObject::_createRoute(int ID)
{
    emit s_routeCreated(ID);
    _currentRoute = new Route(this, ID);
    emit s_routeNameUpdate(ID, _currentRoute->objectName());
    _RouteList.append(_currentRoute);
    _currentRoute->moveToThread(_routeThread);
}

//------------------------------------------------------------------------------
void PromObject::CreateRoute()
{
    while(_routeByID(_genRouteID))_genRouteID ++;
    _createRoute(_genRouteID);
    _genRouteID ++;
}

//------------------------------------------------------------------------------
Route * PromObject::_routeByID(int id)
{
    foreach (Route * route, _RouteList){
        if(route->ID == id){
            return route;
        }
    }
    return nullptr;
}

//------------------------------------------------------------------------------
int PromObject::_indOfRouteByID(int id)
{
    for (int i = 0; i < _RouteList.count(); i++){
        if(_RouteList[i]->ID == id){
            return i;
        }
    }
    return -1;
}

//------------------------------------------------------------------------------
int PromObject::getCurrUserN() const
{
    return currUserN;
}

//------------------------------------------------------------------------------
Unit * PromObject::UnitByName(QString Name)
{
    foreach(Unit * unit, _units){
        if(unit->objectName() == Name) return unit;
    }
    return nullptr;
}

//------------------------------------------------------------------------------
void PromObject::DeleteRoute(int id)
{
    Route * r = _RouteList.takeAt(_indOfRouteByID(id));
    Logging(Prom::MessInfo, QDateTime::currentDateTime(), false, r->objectName(), " удален");
    if(_currentRoute == r) _currentRoute = nullptr;
    delete r;
    iniRoute->remove(QString::number(id));
    emit s_routeDeleted(id);

    if(_RouteList.count() != 0){
        SetCurrentRoute(_RouteList[_RouteList.count()-1]);
    }
    else SetCurrentRoute(nullptr);
}

//------------------------------------------------------------------------------
void PromObject::StartRoute(int id)
{
    Route * tmp = _routeByID(id);
    if(tmp != nullptr){
        emit tmp->StartSig();
    }
}
//------------------------------------------------------------------------------
void PromObject::FreezeRoute(int id)
{
    Route * tmp = _routeByID(id);
    if(tmp != nullptr){
        emit tmp->FreezeSig();
    }
}

//------------------------------------------------------------------------------
void PromObject::StopRoute(int id)
{
    Route * tmp = _routeByID(id);
    if(tmp != nullptr){
        emit tmp->StopSig();
    }
}

//------------------------------------------------------------------------------
void PromObject::CleanRoute(int id)
{
    Route * tmp = _routeByID(id);
    if(tmp != nullptr){
        emit tmp->CleanStopSig();
    }
}
//------------------------------------------------------------------------------
void PromObject::DeleteRouteLastUnit(int id)
{
    Route * tmp = _routeByID(id);
    if(tmp != nullptr){
        emit tmp->DeleteLastUnitSig();
    }
}
//------------------------------------------------------------------------------
void PromObject::ClearRoute(int id)
{
    Route * tmp = _routeByID(id);
    if(tmp != nullptr){
        emit tmp->CleanUnitListSig();
    }
}
//------------------------------------------------------------------------------

void PromObject::SetRouteCurrent(int id)
{
    if(SetCurrentRoute(_routeByID(id))){
        emit s_routeSetCurrent(id);
    }
}

//------------------------------------------------------------------------------
void PromObject::UpdateRouteName(int id, QString name)
{
    Route * tmp = _routeByID(id);
    if(tmp != nullptr){
        emit tmp->SetNameSig(name);
    }
}

//------------------------------------------------------------------------------
void PromObject::ShowTags()
{
    _tspWin->show();
}
//------------------------------------------------------------------------------
void PromObject::init()
{
    connect(_gui, SIGNAL(test()),             this, SLOT(  alarmDo()),           Qt::QueuedConnection);
    connect(_gui, SIGNAL(stop()),             this, SLOT(  UserGlobalStop()),    Qt::QueuedConnection);
    connect(_gui, SIGNAL(globalResetAlarm()), this, SIGNAL(s_globalRestAlarm()), Qt::QueuedConnection);
    connect(_gui, SIGNAL(showTags()),         this, SLOT(  ShowTags()),          Qt::QueuedConnection);
    connect(this, SIGNAL(s_loaded()),         _gui, SLOT(  checkLoaded()),       Qt::QueuedConnection);

    connect(this, &PromObject::s_setCurrentRoute, this,  &PromObject::SetCurrentRoute);


    QObject* routeGui = _gui->findChild<QObject*>("routeWindow");
    if(routeGui){
        connect(this, SIGNAL(s_routeCreated(QVariant)),routeGui, SLOT(appendRoute(QVariant)) ,  Qt::QueuedConnection);
        connect(this, SIGNAL(s_routeStarted(QVariant)),routeGui, SLOT(startedRoute(QVariant)) , Qt::QueuedConnection);
        connect(this, SIGNAL(s_routeStoped(QVariant)),routeGui,  SLOT(stoppedRoute(QVariant)) , Qt::QueuedConnection);
        //connect(this, SIGNAL(s_routeCleaning(QVariant)),routeGui,SLOT(cleanRoute(QVariant)) ,  Qt::QueuedConnection);

        connect(this, SIGNAL(s_routeUnitUpdate(QVariant, QVariant)),routeGui, SLOT(updateRoute(QVariant, QVariant)),  Qt::QueuedConnection);
        connect(this, SIGNAL(s_routeNameUpdate(QVariant, QVariant)),routeGui, SLOT(setRouteName(QVariant, QVariant)), Qt::QueuedConnection);

        connect(routeGui, SIGNAL(createRoute()),                 this, SLOT(CreateRoute())                , Qt::QueuedConnection);
        connect(routeGui, SIGNAL(startRoute(int)),               this, SLOT(StartRoute(int))              , Qt::QueuedConnection);
        connect(routeGui, SIGNAL(stopRoute(int)),                this, SLOT(StopRoute(int))               , Qt::QueuedConnection);
        connect(routeGui, SIGNAL(cleanRoute(int)),               this, SLOT(CleanRoute(int))              , Qt::QueuedConnection);
        connect(routeGui, SIGNAL(updateRouteName(int, QString)), this, SLOT(UpdateRouteName(int, QString)), Qt::QueuedConnection);
        connect(routeGui, SIGNAL(setCurrentRoute(int)),          this, SLOT(SetRouteCurrent(int))         , Qt::QueuedConnection);
        connect(routeGui, SIGNAL(deleteRouteLastUnit(int)),      this, SLOT(DeleteRouteLastUnit(int))     , Qt::QueuedConnection);
        connect(routeGui, SIGNAL(deleteRoute(int)),              this, SLOT(DeleteRoute(int))             , Qt::QueuedConnection);
    }
    else{
        //qDebug() <<"QML routeWindow was not found!";
        Logging(Prom::MessInfo, QDateTime::currentDateTime(), false, "SCADA", "QML-интерфейс маршрутов отсутствует");
    }

    QObject* logGui = _gui->findChild<QObject*>("log");
    if(logGui){
        connect(this, SIGNAL(s_addMessage(QVariant, QVariant, QVariant, QVariant)), logGui, SLOT(addMessage(QVariant, QVariant, QVariant, QVariant)) ,  Qt::QueuedConnection);
    }
    else{
        //qDebug() <<"QML routeWindow was not found!";
        Logging(Prom::MessInfo, QDateTime::currentDateTime(), false, "SCADA", "Лог-интерфейс пользователя отсутствует");
    }
    foreach (Unit * unit, _units) {
        unit->loadParam();
        unit->connectToGUI(_gui);
    }
    emit s_globalRestAlarm();
    emit s_loaded();
    if(_gui->findChild<QObject*>("routeWindow")){
        if(iniRoute->allKeys().count() > 0){
            int id = 1;
            int count = iniRoute->childGroups().count();
            for(int i = 1; i <= count; i ++ ){
                while(! iniRoute->contains(QString::number(id) + "/Name"))
                    id ++;
                _createRoute(id);
                id ++;
            }
        }
    }
    tsp->start();
}
//------------------------------------------------------------------------------
void PromObject::init(QObject *GUI)
{
    _gui = GUI;
    init();
}

//------------------------------------------------------------------------------
