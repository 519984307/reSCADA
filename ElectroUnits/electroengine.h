#ifndef ELECTROENGINE_H
#define ELECTROENGINE_H
#include <QObject>

#ifndef SCADAENUMS_H
#include "../../SCADAenums.h"
#endif // SCADAENUMS_H

#include <QVector>

class Unit;
class OutDiscretETag;
class InDiscretETag;

class ElectroEngine : public QObject
        /*класс электодвигателя
                        входит в большентсво основных и
                        вспомогательных элементов*/
{
    Q_OBJECT

public:
    explicit ElectroEngine(Unit *Owner, std::vector<QString> SensorsNames, QString MultiEnginePrefix = "");

    bool StartForward();
    bool StartBackward();
    void Stop();

    Prom::EngineStates GetNowState(){ return _nowState; }
    Prom::EngineStates GetPrevState(){ return _prevState; }
    OutDiscretETag * startForward = nullptr;
    OutDiscretETag * startBackward = nullptr;
    OutDiscretETag * stop = nullptr;
    InDiscretETag * AS = nullptr;
    InDiscretETag * KM = nullptr;
    InDiscretETag * KMforward = nullptr;
    InDiscretETag * KMbackward = nullptr;
    InDiscretETag * QK = nullptr;
    //bool isOperable() const { return operable; }
    QString AlarmDescription() const { return _alarmDescription; }
    bool isAlarm() const { return _alarm; }

protected:
    Unit * _owner = nullptr;
    Prom::EngineStates _prevState = Prom::EngNoDef;
    Prom::EngineStates _nowState = Prom::EngNoDef;
    //bool operable = true;
    QString _alarmDescription;
    bool _alarm = false;

signals:
    void StateUpdate();
    void Alarm(QString);

//public slots:
    //void CheckState();
public slots:
    void CheckStateSTART1KM();
    void CheckStateSartStop1KM();
    void CheckStateSartStop();
    void CheckStateREVERSE0KM();
    void CheckStateREVERSE1KM();
    void CheckStateREVERSE2KM();
};

#endif // ELECTROENGINE_H













