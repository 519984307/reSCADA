#ifndef MCC_H
#define MCC_H

#ifndef UNIT_H
#include "unit.h"
#endif

#include <QObject>

class InDiscretETag;

class MCC : public Unit
{
    Q_OBJECT
public:
    explicit MCC( int ID,
                  QString Name,
                  QString TagPefix,
                  Prom::UnitModes SaveMode = Prom::UnMdStart );

protected:
    InDiscretETag * _MMCmode = nullptr;
    Prom::SetModeResp _customSetMode( Prom::UnitModes *, bool ) override { return Prom::RejAnnown; }
signals:
    void Started();
    void ManualStarted();//для визуализации
    void NoDef();
    //void SendDispName( QString dispName );

public slots:

    /*!для визуализации*/
    void _updateStateAndMode() override;

    void _doOnModeChange() override{};

protected slots:
    void _customConnectToGUI( QObject * guiItem,  QObject * propWin ) override;
};

#endif // MCC_H
