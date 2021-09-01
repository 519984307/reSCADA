#ifndef SILOS2_H
#define SILOS2_H

//#ifndef UNIT_H
#include "RouteUnit.h"
//#endif

#include <QObject>

class InDiscretETag;

class Silos2 : public RouteUnit
{
    Q_OBJECT
public:
    explicit Silos2( int ID,
                     QString Name,
                     QString TagPefix,
                     bool HLNOnotNC = false,
                     Prom::UnitModes SaveMode = Prom::UnMdEmpty );

protected:
    InDiscretETag * _HL = nullptr;
    InDiscretETag * _LL = nullptr;
    //QString _dispName = "";

signals:
    void ActivateHL();
    void DeactivateHL();
    void ActivateLL();
    void DeactivateLL();
    //void SendDispName( QString dispName );

public slots:
    void AddToCurrentRoteFull()  { _AddToCurrentRoute( Prom::UnMdFull );  }//для визуализации
    void AddToCurrentRoteLow()   { _AddToCurrentRoute( Prom::UnMdLow );   }//для визуализации
    void AddToCurrentRoteEmpty() { _AddToCurrentRoute( Prom::UnMdEmpty ); }//для визуализации

protected slots:
    Prom::SetModeResp _customSetMode( Prom::UnitModes *mode, bool UserOrSys ) override;
    void _customConnectToGUI( QObject *guiItem, QObject *propWin ) override;

protected:
    void _updateStateAndMode() override;

};

#endif // SILOS2_H
