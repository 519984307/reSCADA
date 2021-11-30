#ifndef STATIC2POSUNIT_H
#define STATIC2POSUNIT_H

//#ifndef UNIT_H
#include "RouteUnit.h"
//#endif
#include <QObject>

class ElectroEngine;
class InDiscretETag;

class Static2posUnit : public RouteUnit
{
    Q_OBJECT

public:
    Static2posUnit( Prom::UnitType Type,
                    int ID,
                    QString Name,
                    QString TagPefix,
                    Prom::UnitModes SaveMode,
                    std::vector<QString> EngSensorNames,
                    bool NOnotNCSensors);



    InDiscretETag * _POS1 = nullptr;
    InDiscretETag * _POS2 = nullptr;
protected:
    ElectroEngine * _engine = nullptr;
     virtual Prom::SetModeResp _customSetMode( Prom::UnitModes *mode, bool UserOrSys ) override;
    bool _midleMode() override;
    Prom::UnitModes _ModeOfCommand( Prom::RouteCommand *Command ) override;
    void _alarmDo() override;

signals:
    void Pos1();
    void Pos1Move();
    void Pos1Command();
    void Pos1Manual();
    void Pos1StopCommand();

    void MiddlePos();
    void NoDefPos();
    void Stoped();

    void Pos2();
    void Pos2Move();
    void Pos2Command();
    void Pos2Manual();
    void Pos2StopComand();
    void NoDefManual();

protected slots:
    //void _CleanTimeEnd() override {;}
    void _customConnectToGUI( QObject * guiItem, QObject * propWin ) override;

public slots:
    void AddToCurrentRoteIn1pos(){  _AddToCurrentRoute( Prom::UnMd1pos  ); }
    void AddToCurrentRoteIn2pos(){  _AddToCurrentRoute( Prom::UnMd2pos  ); }
    void ToPos1(){ if( ! _blocked )setMode( Prom::UnMd1pos, true ); }
    void ToPos2(){ if( unitType == Prom::TypeGate || ! _blocked )setMode( Prom::UnMd2pos, true ); }///Вынести в отдельный класс ЗАДВИЖКИ
    void Stop(){ if( unitType == Prom::TypeGate || ! _blocked ) setMode( Prom::UnMdStop, true ); }///Вынести в отдельный класс ЗАДВИЖКИ

    void _updateStateAndMode() override;
};

#endif // STATIC2POSUNIT_H
