#include "gui.h"
#include "ui_gui.h"
#include <QFileDialog>
TSPWindow::TSPWindow(TSP * tsp, QWidget *parent, int options)
    : QMainWindow(parent),
      options(options),
      tsp(tsp),
      ui(new Ui::TSPWindow)
{
    ui->setupUi(this);
    setWindowTitle("Trofen SCADA platform " + tsp->version);
    if((options & ShowSystemLog) != 0){
        QObject::connect(tsp, &TSP::s_loggingSig, [this](MessType messType, QDateTime DateTime, bool, QString Source, QString Message){
            switch (messType) {
            case MessError:{
                if(ui->statusbar)ui->statusbar->showMessage("<<SYSTEM ERROR>> " + DateTime.toString("hh:mm:ss.zzz ") + Source + " " + Message);
                break;
            }
            case MessInfo:{
                if(ui->statusbar)ui->statusbar->showMessage("<<SYSTEM INFO>> " + DateTime.toString("hh:mm:ss.zzz ") + Source + " " + Message);
                break;
            }
            case MessVerbose:{
                if(ui->statusbar)ui->statusbar->showMessage("<<SYSTEM>> " + DateTime.toString("hh:mm:ss.zzz ") + Source + " " + Message);
                break;
            }
            default:{}
            }
        });
    }
    if((options & ShowUserLog) != 0){
        QObject::connect(tsp, &TSP::s_loggingSig, [this](MessType messType, QDateTime DateTime, bool, QString Source, QString Message){
            switch (messType) {
            case MessError:{
                if(ui)ui->statusbar->showMessage("<<USER ERROR>> " + DateTime.toString("hh:mm:ss.zzz ") + Source + " " + Message);
                break;
            }
            case MessInfo:{
                if(ui)ui->statusbar->showMessage("<<USER INFO>> " + DateTime.toString("hh:mm:ss.zzz ") + Source + " " + Message);
                break;
            }
            case MessVerbose:{
                if(ui)ui->statusbar->showMessage("<<USER>> " + DateTime.toString("hh:mm:ss.zzz ") + Source + " " + Message);
                break;
            }
            default:{}
            }
        });
    }

    IconDir * icons = new IconDir();

    TSPTableModelDrivers * modelDrivers = new TSPTableModelDrivers(tsp->listOfDriverConfigs, this->options, icons, ui->tableDrivers);
    ui->tableDrivers->setModel(modelDrivers);
    TSPTableModelGroups * modelGroups = new TSPTableModelGroups(tsp->listOfGroupConfigs, this->options, icons, ui->tableGroups);
    ui->tableGroups->setModel(modelGroups);
    TSPTableModelTags * modelTags = new TSPTableModelTags(tsp->listOfTagConfigs, this->options, icons, ui->tableTags);
    ui->tableTags->setModel(modelTags);

    buttonsReorganizer();
}

TSPWindow::~TSPWindow()
{
    delete ui;
}

void TSPWindow::on_tabWidget_currentChanged(int index)
{
    Q_UNUSED(index);
    buttonsReorganizer();
}

void TSPWindow::on_action_TSP_triggered()
{
    ui->tableTags->sortByColumn(4, Qt::AscendingOrder );
    emit tsp->s_loggingSig(MessVerbose, QDateTime::currentDateTime(), true, "", "GUI action: open help");
    QMessageBox::about(this, "О программе", "Trofen Scada Platform\nВерсия: " + tsp->version + "\nДата сборки: " +
                       __DATE__ + " " + __TIME__ + "\nАвтор: Никита Плынский\nТелефон: 89192697533\nTelegram: @trofen");
}

void TSPWindow::on_action_resize_triggered()
{
    switch (ui->tabWidget->currentIndex()) {
    case tabTags:    emit ui->tableTags->resizeColumnsToContents();
    case tabGroups:  emit ui->tableGroups->resizeColumnsToContents();
    case tabDrivers: emit ui->tableDrivers->resizeColumnsToContents();
    }
}


void TSPWindow::on_action_edit_toggled(bool arg1)
{
    if(arg1)    options |= AllowToEdit;
    else        options &=!AllowToEdit;
    buttonsReorganizer();
}

void TSPWindow::on_action_addRows_triggered()
{
    switch (ui->tabWidget->currentIndex()) {
    case tabTags: emit ui->tableTags->onAddRows();          break;
    case tabGroups: emit ui->tableGroups->onAddRows();      break;
    case tabDrivers: emit ui->tableDrivers->onAddRows();    break;
    }
}

void TSPWindow::on_action_saveCopy_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить тэги", QDir::currentPath(), "Json tags (*.json);;All files (*.*)");
    if(fileName != "")
        tsp->saveJson(fileName);
}

void TSPWindow::buttonsReorganizer()
{
    ui->action_edit->setEnabled((options & AllowToChangeEdit) != 0);
    ui->action_addRows->setEnabled((options & AllowToEdit) != 0);
    ui->action_fill->setEnabled((options & AllowToEdit) != 0);
    ui->action_inc->setEnabled((options & AllowToEdit) != 0);
    ui->action_showAccess->setChecked((options & ShowTagAccess) != 0);
    ui->action_edit->setChecked((options & AllowToEdit) != 0);
}

void TSPWindow::on_action_showAccess_toggled(bool arg1)
{
    if(arg1)    options |= ShowTagAccess;
    else        options &=!ShowTagAccess;
    emit ui->tableTags->onUpdateRequested();
    buttonsReorganizer();
}

void TSPWindow::on_action_refresh_triggered()
{
    switch (ui->tabWidget->currentIndex()) {
    case tabTags: emit ui->tableTags->onUpdateRequested();          break;
    case tabGroups: emit ui->tableGroups->onUpdateRequested();      break;
    case tabDrivers: emit ui->tableDrivers->onUpdateRequested();    break;
    }
    buttonsReorganizer();
}

void TSPWindow::on_action_fill_triggered()
{
    switch (ui->tabWidget->currentIndex()) {
    case tabTags: ui->tableTags->fillValues();          break;
    case tabGroups: ui->tableGroups->fillValues();      break;
    case tabDrivers: ui->tableDrivers->fillValues();    break;
    }
}

void TSPWindow::on_action_inc_triggered()
{
    switch (ui->tabWidget->currentIndex()) {
    case tabTags: ui->tableTags->incrementValues();          break;
    case tabGroups: ui->tableGroups->incrementValues();      break;
    case tabDrivers: ui->tableDrivers->incrementValues();    break;
    }
}
