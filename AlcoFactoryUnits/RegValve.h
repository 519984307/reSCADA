#ifndef REGVALVE_H
#define REGVALVE_H

#ifndef UNIT_H
#endif

#include <QObject>
#include "unit.h"

class InDiscretETag;
class OutDiscretETag;
class InETag;
class OutETag;

class RegValve : public Unit
{
  Q_OBJECT
public:
  explicit RegValve( int *Id, QString Name, QString TagPefix );
  ~RegValve();
  InETag * getPosTag(){ return _pos; }
  OutETag * getPosSetPtTag() { return _posSetPt;}

protected:
  OutETag * _posSetPt { nullptr };
  InETag * _pos { nullptr };
  OutETag * _rangeTop { nullptr };
  OutETag * _rangeBottom { nullptr };
  OutDiscretETag *_openOut { nullptr };
  OutDiscretETag *_closeOut { nullptr };

  void _alarmDo() override;
  void _resetAlarmDo() override;

protected slots:
  Prom::SetModeResp _customSetMode( Prom::UnitModes *mode, bool UserOrSys ) override;
  void _customConnectToGUI( QObject * guiItem,  QObject * propWin ) override;


protected:
  void _updateStateAndMode() override;

  // Unit interface
protected:
  void _doOnModeChange();
};

#endif // DRYER2VSM_DOBRNR_H
