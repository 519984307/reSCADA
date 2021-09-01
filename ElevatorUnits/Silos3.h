#ifndef SILOS3_H
#define SILOS3_H

//#ifndef UNIT_H
#include "RouteUnit.h"
//#endif

#include <QObject>

class InDiscretETag;

class Silos3 : public RouteUnit
{
    Q_OBJECT
public:
    explicit Silos3( int ID,
                     QString Name,
                     QString TagPefix,
                     bool HLNOnotNC = false,
                     Prom::UnitModes SaveMode = Prom::UnMdEmpty );

protected:
    InDiscretETag * _HL = nullptr;
    InDiscretETag * _ML = nullptr;
    InDiscretETag * _LL = nullptr;
    //QString _dispName = "";
    void _updateStateAndMode() override;
    Prom::SetModeResp _customSetMode(Prom::UnitModes *mode, bool UserOrSys) override;

signals:
    void ActivateHL();
    void DeactivateHL();
    void ActivateML();
    void DeactivateML();
    void ActivateLL();
    void DeactivateLL();
    //void SendDispName( QString dispName );

public slots:
    void AddToCurrentRoteFull()  { _AddToCurrentRoute( Prom::UnMdFull );  }//для визуализации
    void AddToCurrentRoteMidle() { _AddToCurrentRoute( Prom::UnMdMidle ); }//для визуализации
    void AddToCurrentRoteLow()   { _AddToCurrentRoute( Prom::UnMdLow );   }//для визуализации
    void AddToCurrentRoteEmpty() { _AddToCurrentRoute( Prom::UnMdEmpty ); }//для визуализации

    //void _CleanTimeEnd()override{}
protected slots:
    void _customConnectToGUI( QObject * guiItem, QObject * propWin ) override;

};

#endif // SILOS3_H
