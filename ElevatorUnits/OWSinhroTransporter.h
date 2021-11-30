#ifndef OWSINHROTRANSPORTER_H
#define OWSINHROTRANSPORTER_H

#include "RouteUnit.h"

class InDiscretETag;

class OWSinhroTransporter : public RouteUnit
{
    Q_OBJECT

public:
    explicit OWSinhroTransporter( int ID,
                                  QString Name,
                                  QString TagPefix );

protected:
    InDiscretETag * _SS = nullptr;

signals:
    void Started();         //для визуализации
    void Stoped();        //для визуализации
    void Cleaning();
    //    void ManualStarted();//для визуализации
    void NoDef();       //для визуализации


protected slots:
    void AddToCurrentRoteStoped()  { _AddToCurrentRoute( Prom::UnMdStop  ); }//для визуализации
    void AddToCurrentRoteStarted() { _AddToCurrentRoute( Prom::UnMdStart ); }//для визуализации
    void _customConnectToGUI( QObject * guiItem,  QObject * propWin ) override;


public slots:


    // Unit interface
protected:
    Prom::SetModeResp _customSetMode(Prom::UnitModes *mode, bool UserOrSys) override;
    void _updateStateAndMode() override;
};

#endif // OWSINHROTRANSPORTER_H
