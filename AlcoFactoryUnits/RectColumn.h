#ifndef RECTCOLUMN_H
#define RECTCOLUMN_H

#ifndef UNIT_H
#endif

#include <QObject>
#include "unit.h"

class InDiscretETag;
class OutDiscretETag;
class InETag;
class OutETag;
class RegValve;

//enum pidOpt{
namespace PIDopt {
const  char allOff = 0;
const  char allOn = 0xFF;
const  char kP = 0b00000001; // шестнадцатеричный литерал для 0000 0001
const  char kI = 0b00000010;
const  char kD = 0b00000100;
//  option4 = 0b00001000,
//  option5 = 0b00010000,
//  option6 = 0b00100000,
//  option7 = 0b01000000,
//  option8 = 0b10000000
//};
}


struct PID{
  PID(Unit *Owner, QString Name, QString TagPrefix, char Option = PIDopt::allOn);
  QString tagPrefix;
  QString name;
  const char opt { PIDopt::allOn };
  OutDiscretETag * manOn { nullptr };
  OutETag * setPt { nullptr };
  InETag * process { nullptr };
  InETag * impIn { nullptr };
  OutETag * impOut { nullptr };
  OutETag * kP { nullptr };
  OutETag * kI { nullptr };
  OutETag * kD { nullptr };
};
struct PIDstep: public PID{
  PIDstep(Unit *Owner, QString Name, QString TagPrefix, char Option = PIDopt::allOn);
  OutDiscretETag * manImpUp { nullptr };
  OutDiscretETag * manImpDown { nullptr };
  InETag * impUp { nullptr };
  InETag * impDown { nullptr };
};
struct PIDcont: public PID{
  PIDcont(Unit *Owner, QString Name, QString TagPrefix, char Option = PIDopt::allOn);
  OutETag * manImp { nullptr };
  OutETag * impLimUpLim { nullptr };
  OutETag * impLimDown { nullptr };
};

class RectColumn : public Unit
{
  Q_OBJECT
public:
  explicit RectColumn( int *Id, QString Name, QString TagPefix );
  ~RectColumn();
protected:
  RegValve * _steamValve{ nullptr };
  RegValve * _clWtrValve{ nullptr };

  InETag * _tempOutWtr { nullptr };
  InETag * _presTop { nullptr };
  InETag * _tempTop { nullptr };
  InETag * _presBottom { nullptr };
  InETag * _tempBottom { nullptr };

  PIDstep *_valveToPresTop;
  PIDstep *_valveToPresBottom;

  void _alarmDo() override;
  void _resetAlarmDo() override;

protected slots:
  Prom::SetModeResp _customSetMode( Prom::UnitModes *mode, bool UserOrSys ) override;
  void _customConnectToGUI( QObject * guiItem,  QObject * propWin ) override;

protected:
  void _updateStateAndMode() override;
  void _doOnModeChange();

};

#endif // DRYER2VSM_DOBRNR_H
