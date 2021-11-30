#include <qsettings.h>
#include <qdebug.h>

#include "UnitGroup.h"
#include "unit.h"
#include "ETag.h"
#include "InETag.h"
#include "OutETag.h"
#include "InDiscretETag.h"
#include "OutDiscretETag.h"
#include "promobject.h"
extern QSettings * globalUnitINI;
//-----------------------------------------------------------------------------------------------------
UnitGroup::UnitGroup( Unit * owner,
                      QString Name,
                      Prom::UnitModes SaveMode,
                      QSettings * Ini )

    : saveMode( SaveMode ),
      ini( Ini )
{
    setObjectName( Name );
    if( ini == nullptr ){
        if( globalUnitINI != nullptr ) ini = globalUnitINI;
        else{
            ini = new QSettings( owner->tagPrefix + ".ini", QSettings::IniFormat );
        }
    }
    LoadParam();
}

//-----------------------------------------------------------------------------------------------------
Prom::SetModeResp UnitGroup::setMode( UnitModes Mode, bool UserOrSys )
{
    Logging( Prom::MessChangeCommand, QDateTime::currentDateTime(), UserOrSys, objectName(), "поступила команда - '" + Prom::modeToString( Mode ) + "'" );

    switch ( _customSetMode( &Mode, UserOrSys ) ) {
    case RejNoCond :
        Logging( Prom::MessChangeCommand, QDateTime::currentDateTime(), UserOrSys, objectName(), QString( "отклонена из-за отсутствия условий команда - '" )+ Prom::modeToString( Mode ) + "'" );
        return RejNoCond;
    case RejTransPr:
        Logging( Prom::MessChangeCommand, QDateTime::currentDateTime(), UserOrSys, objectName(), QString( "отклонена из-за переходного процесса команда - '" )+ Prom::modeToString( Mode ) + "'" );
        return RejTransPr;
    case RejAlarm:
        Logging( Prom::MessChangeCommand, QDateTime::currentDateTime(), UserOrSys, objectName(), QString( "отклонена по аварии команда - '" )+ Prom::modeToString( Mode ) + "'" );
        return RejAlarm;
    case RejAnnown:
        Logging( Prom::MessChangeCommand, QDateTime::currentDateTime(), UserOrSys, objectName(), QString( "отклонена неизвестная команда - '" )+ Prom::modeToString( Mode ) + "'" );
        return  RejAnnown;
    case DoneAlready:
        return DoneAlready;
    case DoneWhait:
        Logging( Prom::MessChangeCommand, QDateTime::currentDateTime(), UserOrSys, objectName(), QString( "принята к исполнению команда - '" )+ Prom::modeToString( Mode ) + "'" );
        return DoneWhait;
        ;
    }
}

//-----------------------------------------------------------------------------------------------------
void UnitGroup::setBlocked(bool blocked)
{
    _blocked = blocked;
}

//-----------------------------------------------------------------------------------------------------
void UnitGroup::_setCurrentMode( UnitModes currentMode )
{

    if( _setedMode != Prom::UnMdNoDef ){
        Logging( Prom::MessChangeState, QDateTime::currentDateTime(), false, objectName(), QString( ( _currentMode == _setedMode ? "" : "не" ) )
                 + "выполнена команда - '" +  Prom::modeToString( _currentMode ) + "'" );
        _setedMode = Prom::UnMdNoDef;
    }
    if( _currentMode != currentMode ) {
        _prevMode = _currentMode;
        _currentMode = currentMode;
        _doOnModeChange();
        emit ModeChange( this );
    }
}

//-----------------------------------------------------------------------------------------------------
void UnitGroup::_setSetedMode( Prom::UnitModes setedMode )
{
    _setedMode = setedMode;
}

//-----------------------------------------------------------------------------------------------------
void UnitGroup::_setCurrentState( Prom::UnitStates currentState )
{
    if ( currentState != _currentState ){
        _prevState = _currentState;
        _currentState = currentState;
        Logging( Prom::MessChangeState, QDateTime::currentDateTime(), false, objectName(), QString( "cмена состояния на '" +  Prom::Prom::stateToString( _currentState ) + "'" ) );
        emit StateChange( this );
    }
}

//-----------------------------------------------------------------------------------------------------
void UnitGroup::UpdateState()
{
    //!
//    if( _sensorsConnected )_UpdateStateAndMode();
//    else{
//        _setCurrentState( Prom::UnStNotConnected );
//        _setCurrentMode( Prom::UnMdNoDef );
//    }
}

//-----------------------------------------------------------------------------------------------------
//void UnitGroup::moveToThread( QThread *thread )
//{
//    ownThread = thread;
//    this->QObject::moveToThread( thread );
//    if( ownThread ){
//        foreach ( Unit * unit, _subUnits ) {
//            unit->moveToThread( ownThread );
//            unit->setParent( this );
//        }
//    }

//}

//-----------------------------------------------------------------------------------------------------
void UnitGroup::addUnit( Unit * unit )
{
    _units.append( unit );

    unit->ini = ini;
    connect( unit, &UnitGroup::LoggingSig, this, &UnitGroup::Logging, Qt::QueuedConnection );
    connect( unit, &UnitGroup::AlarmForAnderUnit, this, &UnitGroup::_DetectSubUnitAlarm, Qt::QueuedConnection );
    connect( unit, &UnitGroup::ModeChange, this, &UnitGroup::_updateSubUnitMode, Qt::QueuedConnection );
    connect( unit, &UnitGroup::StateChange, this, &UnitGroup::_updateSubUnitState, Qt::QueuedConnection );
}

//-----------------------------------------------------------------------------------------------------
void UnitGroup::DetectAlarm( QString Description )
{
    if( Description == "" ){
        return;
    }
    _alarm = true;
    if( Prom::icvalModes( _currentMode, saveMode ) && _currentMode != Prom::UnMdNoDef && ! _firstLoad ) {
        emit QuitAlarm( objectName() + " - " +  Description );
        Logging( Prom::MessQuitAlarm, QDateTime::currentDateTime(), false, objectName(), Description );
    }
    else {
        emit Alarm( objectName()  + " - " +  Description );
        Logging( Prom::MessAlarm, QDateTime::currentDateTime(), false, objectName(), Description );
    }
    if( !_firstLoad )
        _AlarmDo();
    emit AlarmForAnderUnit( this, Description );
}

//-----------------------------------------------------------------------------------------------------
void UnitGroup::_DetectSubUnitAlarm( Unit * unit, QString Description )
{
    if( Description == "" ){
        return;
    }
    _alarm = true;
    if( Prom::icvalModes( _currentMode, saveMode ) && _currentMode != Prom::UnMdNoDef && ! _firstLoad ) {
        emit QuitAlarm( objectName() + " - " +  Description );
        Logging( Prom::MessQuitAlarm, QDateTime::currentDateTime(), false, objectName(), "Авария внутреннего юнита "
                 + unit->objectName() );
    }
    else {
        emit Alarm( objectName()  + " - " +  Description );
        Logging( Prom::MessAlarm, QDateTime::currentDateTime(), false, objectName(), "Авария внутреннего юнита "
                 + unit->objectName() );
    }
    if( !_firstLoad )
        _AlarmSubUnitDo( unit );
}

//-----------------------------------------------------------------------------------------------------
void UnitGroup::_AlarmDo()
{
    SetMode( Prom::UnMdStop, false );
}

//-----------------------------------------------------------------------------------------------------
void UnitGroup::_AlarmSubUnitDo(  Unit * unit )
{
    SetMode( Prom::UnMdStop, false );
    foreach (Unit * unit, _subUnits ) {
        unit->SetMode( unit->saveMode, false );
    }
}

//-----------------------------------------------------------------------------------------------------
bool UnitGroup::ResetAlarm()
{
    static bool preAlarm = _alarm;
    _alarm = ! _mayResetAlarm;
    bool B;
    foreach( ETag * tag, _tags ){
        B = tag->ResetAlarm();

        _alarm |= ! B;
    }
    foreach (Unit * unit, _subUnits ) {
        _alarm |= ! unit->ResetAlarm();
    }
    _alarm |= _alarmConnection;

    if( _alarm ){
        emit AlarmReseted();
        emit Alarm( "" );
        Logging( Prom::MessAlarm, QDateTime::currentDateTime(), false, objectName(), "аварии не сброшены" );
    }
    else {
        emit AlarmReseted();
        if( preAlarm )Logging( Prom::MessAlarm, QDateTime::currentDateTime(), false, objectName(), "аварии сброшены" );
        _ResetAlarmDo();
    }
    return ! _alarm;
}

//-----------------------------------------------------------------------------------------------------
void UnitGroup::setAlarmTag( QString tagName )
{
    _alarmInit = true;
    Logging( Prom::MessAlarm, QDateTime::currentDateTime(), false, objectName(), "ошибка загрузки тега " + tagName );
    //_DetectAlarm( "ошибка загрузки тега " + tagName );
}

//-----------------------------------------------------------------------------------------------------
QVector<Unit *> UnitGroup::subUnits() const
{
    return _subUnits;
}

//-----------------------------------------------------------------------------------------------------
void UnitGroup::Logging( Prom::MessType MessTypeID, QDateTime DateTime, bool UserOrSys, QString Source, QString Message )
{
    emit LoggingSig( MessTypeID, DateTime, UserOrSys, objectName() + "->" + Source, Message );
}

//-----------------------------------------------------------------------------------------------------
void UnitGroup::connectToGUI( const QObject * GUI )
{
    QObject * guiItem = GUI->findChild<QObject*>( this->tagPrefix );

    if( guiItem ){
        //Logging( Prom::MessChangeInfo, QDateTime::currentDateTime(), unit->objectName(), "GUI обнаружен" );

        QObject * propWin =  guiItem->findChild<QObject*>( "prop" );
        connect( this, SIGNAL( Connected()    ), propWin, SLOT( connected()    ), Qt::QueuedConnection );
        connect( this, SIGNAL( Disconnected() ), propWin, SLOT( disconnected() ), Qt::QueuedConnection );
        connect( this, SIGNAL( SetInRoute( QVariant ) ), propWin, SLOT( setRoute( QVariant ) ), Qt::QueuedConnection );

        connect( this,    SIGNAL( Alarm(     QString ) ), propWin, SLOT( alarmSet()        ), Qt::QueuedConnection );
        connect( this,    SIGNAL( QuitAlarm( QString ) ), propWin, SLOT( quitAlarm()    ), Qt::QueuedConnection );
        connect( this,    SIGNAL( AlarmReseted()       ), propWin, SLOT( alarmReseted() ), Qt::QueuedConnection );
        connect( propWin, SIGNAL( resetAlarm()         ), this,    SLOT( ResetAlarm()   ), Qt::QueuedConnection );

        QMetaObject::invokeMethod( propWin, "setLinked", Qt::DirectConnection );
        QVariant ret;
        QObject * tmpSgSt, * engRow;

        if( this->mover ){
            connect( this, SIGNAL( Cleaning()          ), guiItem, SLOT( cleaningWork() ), Qt::QueuedConnection );
            connect( this, SIGNAL( QuitAlarm( QString ) ), this, SLOT( MoverAlarm() ), Qt::QueuedConnection );

            QMetaObject::invokeMethod( propWin, "addEngRow", Qt::DirectConnection, Q_RETURN_ARG( QVariant, ret ), Q_ARG( QVariant, this->objectName() ) ); //создал строку меню задержки
            engRow = qvariant_cast< QObject* >( ret );
            QMetaObject::invokeMethod( engRow, "addPropertySetting", Qt::DirectConnection,
                                       Q_RETURN_ARG( QVariant, ret ),
                                       Q_ARG( QVariant, this->objectName() + "_clean_delay"  ),
                                       Q_ARG( QVariant, "сек. - время зачистки" ) );

            tmpSgSt = qvariant_cast< QObject* >( ret );//получаю указатель на зачистку
            //подключаю сигналы к зачистке
            connect( tmpSgSt, SIGNAL( changedVal(       QVariant ) ), this,      SLOT( WriteCleanDelay( QVariant ) ), Qt::QueuedConnection );
            connect( this,      SIGNAL( ChangeCleanDelay( QVariant ) ), tmpSgSt, SLOT( changeVal      ( QVariant ) ), Qt::QueuedConnection );
        }
        _customConnectToGUI( guiItem, propWin );
        foreach ( ETag * ET , _tags ) {

            QMetaObject::invokeMethod( propWin, "addEngRow", Qt::DirectConnection, Q_RETURN_ARG( QVariant, ret ), Q_ARG( QVariant, ET->fullTagName() ) ); //создал строку меню для тэга
            engRow = qvariant_cast< QObject* >( ret );

            if( ! ET->isOk() ){
                QMetaObject::invokeMethod( propWin, "addAlarm", Qt::DirectConnection, Q_RETURN_ARG( QVariant, ret ),
                                           Q_ARG( QVariant, ET->fullTagName() + "_tag_alarm" ),
                                           Q_ARG( QVariant, "Ошибка загрузки тэга - " + ET->getName() ),
                                           Q_ARG( QVariant, false ) );
            }
            //!создаю строку аварии для тэга
            QMetaObject::invokeMethod( propWin, "addAlarm", Qt::DirectConnection, Q_RETURN_ARG( QVariant, ret ),
                                       Q_ARG( QVariant, ET->fullTagName() + "_alarm" ),
                                       Q_ARG( QVariant, ET->getName() ),
                                       Q_ARG( QVariant, ET->ignorableAlarm ) );

            tmpSgSt = qvariant_cast< QObject* >( ret );
            //QObject * tmpSgSt = propWin->findChild<QObject*>( est->fullTagName() + "_alarm" );                                                //получил указатеть на созданную строку
            //-----подключил сигналы к авирии и игнору
            connect( ET,     SIGNAL( AlarmReseted()             ),   tmpSgSt, SLOT( resetAlarm()                ), Qt::QueuedConnection );
            connect( ET,     SIGNAL( Alarm(            QString  ) ), tmpSgSt, SLOT( setAlarm()                  ), Qt::QueuedConnection );
            connect( tmpSgSt,SIGNAL( changedIgnore(    bool     ) ), ET,      SLOT( WriteIgnorAlarm( bool     ) ), Qt::QueuedConnection );
            connect( ET,     SIGNAL( ChangeIgnorAlarm( QVariant ) ), tmpSgSt, SLOT( changeIgnore(    QVariant ) ), Qt::QueuedConnection );
            connect( ET,     SIGNAL( QualityChanged(   QVariant ) ), tmpSgSt, SLOT( changeConnected( QVariant ) ), Qt::QueuedConnection );
            //-----подключил сигналы к авирии и игнору
            //}


            if( ET->ttype == Prom::TpDiscretIn || ET->ttype == Prom::TpDiscretOut ){ //Для дискретных сигналов
                //!создал в строке главный раздел ( с кубиками )
                QMetaObject::invokeMethod( engRow, "addPropertySignal", Qt::DirectConnection,
                                           Q_RETURN_ARG( QVariant, ret ),
                                           Q_ARG( QVariant, ET->fullTagName() + "_PrSig" ),
                                           Q_ARG( QVariant, ET->getName() ) );
                //tmpSgSt = propWin->findChild<QObject*>( est->fullTagName() + "_PrSig" );
                tmpSgSt = qvariant_cast< QObject* >( ret );
                //получил указатель на главный раздел
                //-----подключил сигналы к кубикам
                connect( tmpSgSt, SIGNAL( changedIm(      bool     ) ), ET,      SLOT( WriteImit(       bool     ) ), Qt::QueuedConnection );
                connect( tmpSgSt, SIGNAL( changedImVal(   QVariant ) ), ET,      SLOT( WriteImitVal(    QVariant ) ), Qt::QueuedConnection );
                connect( ET,      SIGNAL( ChangeIm(       QVariant ) ), tmpSgSt, SLOT( changeIm(        QVariant ) ), Qt::QueuedConnection );
                connect( ET,      SIGNAL( ChangeImVal(    QVariant ) ), tmpSgSt, SLOT( changeImVal(     QVariant ) ), Qt::QueuedConnection );
                connect( ET,      SIGNAL( LiveTagChanged( QVariant ) ), tmpSgSt, SLOT( changeVal(       QVariant ) ), Qt::QueuedConnection );
                connect( ET,      SIGNAL( QualityChanged( QVariant ) ), tmpSgSt, SLOT( changeConnected( QVariant ) ), Qt::QueuedConnection );
                //-----подключил сигналы к кубикам
            }
            else if( ET->ttype == Prom::TpOut ) { //Для аналоговых вЫходных сигналов
                //!создал в строке главный раздел
                QMetaObject::invokeMethod( engRow, "addPropertyValue", Qt::DirectConnection,
                                           Q_RETURN_ARG( QVariant, ret ),
                                           Q_ARG( QVariant, ET->fullTagName() + "_PrVal" ),
                                           Q_ARG( QVariant, ET->getName() ) );

                //tmpSgSt = propWin->findChild<QObject*>( est->fullTagName() + "_PrSig" );
                tmpSgSt = qvariant_cast< QObject* >( ret );
                //получил указатель на главный раздел
                //-----подключил сигналы к значению и имитации
                connect( tmpSgSt, SIGNAL( changedIm(      bool     ) ), ET,      SLOT( WriteImit(       bool     ) ), Qt::QueuedConnection );
                connect( tmpSgSt, SIGNAL( changedImVal(   QVariant ) ), ET,      SLOT( WriteImitVal(    QVariant ) ), Qt::QueuedConnection );
                connect( ET,      SIGNAL( ChangeIm(       QVariant ) ), tmpSgSt, SLOT( changeIm(        QVariant ) ), Qt::QueuedConnection );
                connect( ET,      SIGNAL( ChangeImVal(    QVariant ) ), tmpSgSt, SLOT( changeImVal(     QVariant ) ), Qt::QueuedConnection );
                connect( ET,      SIGNAL( LiveTagChanged( QVariant ) ), tmpSgSt, SLOT( changeVal(       QVariant ) ), Qt::QueuedConnection );
                connect( ET,      SIGNAL( QualityChanged( QVariant ) ), tmpSgSt, SLOT( changeConnected( QVariant ) ), Qt::QueuedConnection );
                //-----подключил сигналы к значению и имитации
            }
            else if( ET->ttype == Prom::TpIn ) { //Для аналоговых входных сигналов
                //!создал в строке главный раздел
                QMetaObject::invokeMethod( engRow, "addPropertyValue", Qt::DirectConnection,
                                           Q_RETURN_ARG( QVariant, ret ),
                                           Q_ARG( QVariant, ET->fullTagName() + "_PrVal" ),
                                           Q_ARG( QVariant, ET->getName() ) );

                //tmpSgSt = propWin->findChild<QObject*>( est->fullTagName() + "_PrSig" );
                tmpSgSt = qvariant_cast< QObject* >( ret );
                //получил указатель на главный раздел
                //-----подключил сигналы к значению и имитации
                connect( tmpSgSt, SIGNAL( changedIm(      bool     ) ), ET,     SLOT( WriteImit(        bool     ) ), Qt::QueuedConnection );
                connect( tmpSgSt, SIGNAL( changedImVal(   QVariant ) ), ET,     SLOT( WriteImitVal(     QVariant ) ), Qt::QueuedConnection );
                connect( ET,      SIGNAL( ChangeIm(       QVariant ) ), tmpSgSt, SLOT( changeIm(        QVariant ) ), Qt::QueuedConnection );
                connect( ET,      SIGNAL( ChangeImVal(    QVariant ) ), tmpSgSt, SLOT( changeImVal(     QVariant ) ), Qt::QueuedConnection );
                connect( ET,      SIGNAL( LiveTagChanged( QVariant ) ), tmpSgSt, SLOT( changeVal(       QVariant ) ), Qt::QueuedConnection );
                connect( ET,      SIGNAL( QualityChanged( QVariant ) ), tmpSgSt, SLOT( changeConnected( QVariant ) ), Qt::QueuedConnection );
                //-----подключил сигналы к значению и имитации
                //!добавляю уровень срабатывания
                if( dynamic_cast< InETag* >( ET )->tunabDetectLevel ){

                    QMetaObject::invokeMethod( engRow, "addPropertySetting", Qt::DirectConnection,
                                               Q_RETURN_ARG( QVariant, ret ),
                                               Q_ARG( QVariant, ET->getDBName() + "_delay"  ),
                                               Q_ARG( QVariant, " - уровень срабатывания" ) );
                    //tmpSgSt = guiItem->findChild<QObject*>( est->getDBName() + "_delay" );
                    tmpSgSt = qvariant_cast< QObject* >( ret );//получаю указатель на уровень срабатывания
                    //подключаю сигналы к уровням срабатывания
                    connect( tmpSgSt, SIGNAL( changedVal( QVariant        ) ),    ET,  SLOT( setDetectLevel( QVariant ) ), Qt::QueuedConnection );
                    connect( ET,      SIGNAL( ChangeDelectLewel( QVariant ) ), tmpSgSt, SLOT( changeVal( QVariant        ) ), Qt::QueuedConnection );
                    //подключаю сигналы к уровням срабатывания
                }
                if( ET->getDBName() == ".current" ){ // Если токомер
                    connect( ET, SIGNAL( s_valueChd( QVariant ) ), propWin, SLOT( setCurrent( QVariant ) ), Qt::QueuedConnection );
                }

            }

            if( ET->tunableSetTime ){
                //!добавляю задержку срабатывания
                QMetaObject::invokeMethod( engRow, "addPropertySetting", Qt::DirectConnection,
                                           Q_RETURN_ARG( QVariant, ret ),
                                           Q_ARG( QVariant, ET->getDBName() + "_delay"  ),
                                           Q_ARG( QVariant, "сек. - задержка установки" ) );
                //tmpSgSt = guiItem->findChild<QObject*>( est->getDBName() + "_delay" );
                tmpSgSt = qvariant_cast< QObject* >( ret );//получаю указатель на задержку
                //подключаю сигналы к задержке
                connect( tmpSgSt, SIGNAL( changedVal(     QVariant ) ), ET,     SLOT( WriteSetDelay( QVariant ) ), Qt::QueuedConnection );
                connect( ET,     SIGNAL( ChangeSetDelay( QVariant ) ), tmpSgSt, SLOT( changeVal( QVariant )     ), Qt::QueuedConnection );
                //подключаю сигналы к задержке
            }

            if( ET->tunablePulseTime ) { //Для входных пульсирующих
                //!добавляю пульс
                QMetaObject::invokeMethod( engRow, "addPropertySetting", Qt::DirectConnection,
                                           Q_RETURN_ARG( QVariant, ret ),
                                           Q_ARG( QVariant, ET->getDBName() + "_pulseDelay"  ),
                                           Q_ARG( QVariant, "сек. - интервал пульсации" ) );
                //tmpSgSt = guiItem->findChild<QObject*>( est->getDBName() + "_pulseDelay" );
                tmpSgSt = qvariant_cast< QObject* >( ret );//получаю указатель на задержку
                connect( tmpSgSt, SIGNAL( changedVal( QVariant       ) ), ET,     SLOT( WritePulseDelay( QVariant ) ), Qt::QueuedConnection );
                connect( ET,     SIGNAL( ChangePulseDelay( QVariant ) ), tmpSgSt, SLOT( changeVal( QVariant ) ),       Qt::QueuedConnection );
            }
        }
        //_customConnectToGUI( guiItem, propWin );
        foreach ( Unit * unit, _subUnits ) {
            unit->connectToGUI( guiItem );
        }
        RescanUnit();
    }
    else{
        Logging( Prom::MessInfo, QDateTime::currentDateTime(), false, this->objectName(), "GUI не обнаружен" );
    }

}

//-----------------------------------------------------------------------------------------------------
void UnitGroup::SaveParam()
{
    if( mover )ini->setValue( tagPrefix + "/cleanTimeSec", _cleanTimer->interval()/1000 );
    ini->setValue( tagPrefix + "/exName", _exName );
    foreach( ETag * tag, _tags ){
        tag->SaveParam();
    }
    foreach ( Unit * unit, _subUnits ) {
        unit->SaveParam();
    }
    ini->sync();
}

//-----------------------------------------------------------------------------------------------------
void UnitGroup::LoadParam()
{
    if( mover ) {
        _cleanTimer->setInterval( ini->value( tagPrefix + "/cleanTimeSec", 1 ).toInt() * 1000 );
        emit ChangeCleanDelay( _cleanTimer->interval() / 1000 );
    }
    _exName = ini->value( tagPrefix + "/exName", "" ).toString();
    emit ChangeExName( _exName );
    foreach (Unit * unit, _subUnits) {
        unit->LoadParam();
    }
}

//-----------------------------------------------------------------------------------------------------
void UnitGroup::RescanUnit()
{
    if( mover )emit ChangeCleanDelay( _cleanTimer->interval() / 1000 );
    emit ChangeExName( _exName );
    foreach( ETag * tag, _tags ){
        tag->Rescan();
    }
    foreach (Unit * unit, _subUnits) {
        unit->RescanUnit();
    }
    _SensorConnect();
}

//-----------------------------------------------------------------------------------------------------
void UnitGroup::WriteCleanDelay( QVariant Mnt)
{
    _cleanTimer->setInterval( Mnt.toInt() * 1000 );
}

//-----------------------------------------------------------------------------------------------------
void UnitGroup::_SensorConnect()
{
    _sensorsConnected = true;
    foreach( ETag * tag, _tags ){
        if( tag->isOk() )
            _sensorsConnected &= tag->connected();
    }
    if( _firstLoad ){
        if( _sensorsConnected ) {
            _alarmConnection = false;
            emit Connected();
            _firstLoad = false;
        }
    }
    else {
        if( _sensorsConnected ){
            _alarmConnection = false;
            emit Connected();
        }
        else{
            if( !_alarmConnection ){
                _alarmConnection = true;
                _DetectAlarm( "обрыв соединеия с ПЛК" );
                _setCurrentState( Prom::UnStNotConnected );
                _setCurrentMode( Prom::UnMdNoDef );
                emit Disconnected();
            }
        }
    }
    //UpdateState();
}

//-----------------------------------------------------------------------------------------------------

Prom::UnitStates UnitGroup::prevState() const
{
    return _prevState;
}

void UnitGroup::setPrevState(const Prom::UnitStates &prevState)
{
    _prevState = prevState;
}
