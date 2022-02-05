#ifndef PLC_H
#define PLC_H

#ifndef UNIT_H
#include "unit.h"
#endif

#include <QObject>

class OutDiscretETag;

class PLC : public Unit
{
    Q_OBJECT
public:
    explicit PLC(int *Id,
                  QString Name,
                  QString TagPefix,
                  Prom::UnitModes SaveMode = Prom::UnMdStart);

protected:
    OutDiscretETag * _heartBeat = nullptr;
    Prom::SetModeResp _customSetMode(Prom::UnitModes *mode, bool UserOrSys) override;
signals:
    void s_started();
    void s_stoped();
    void s_noDef();
    //void SendDispName(QString dispName);

public slots:
    /*!для визуализации*/
    void Start()    { setMode(Prom::UnMdStart, true); }
    /*!для визуализации*/
    void Stop()     { setMode(Prom::UnMdStop, true); }
    void _updateStateAndMode() override;

    void _doOnModeChange() override{};

protected slots:
    void _customConnectToGUI(QObject * guiItem,  QObject * propWin) override;
};

#endif // PLC_H
