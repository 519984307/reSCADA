#ifndef TSPWINDOW_H
#define TSPWINDOW_H

#include <QMainWindow>
#include "tsp.h"
#include "guiitems.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TSPWindow; }
QT_END_NAMESPACE

class TSPWindow : public QMainWindow, protected IconDir, public OptionsDir
{
  Q_OBJECT

public:
  TSPWindow(TSP * tsp, QWidget *parent, int options = OptionsDir::DefaultOptions);
  ~TSPWindow();
private:
  enum Tabs{tabTags = 0, tabGroups, tabDrivers};
private slots:
  void statusBarLog(MessType MessTypeID,  QDateTime DateTime, bool UserOrSys, QString Source, QString Message);
  void setValTagInCurrRow( QVariant value );

  void on_tabWidget_currentChanged(int index);
  void on_action_TSP_triggered();
  void on_action_resize_triggered();
  void on_action_edit_toggled(bool arg1);
  void on_action_addRows_triggered();
  void on_action_saveCopy_triggered();
  void on_action_showAccess_toggled(bool arg1);
  void on_action_refresh_triggered();
  void on_action_fill_triggered();
  void on_action_inc_triggered();
  void buttonsReorganizer();

  void on_actionTrue_triggered();
  void on_actionFalse_triggered();




private:
  int options;
  TSP * tsp;
  Ui::TSPWindow *ui;
};
#endif // TSPWINDOW_H
