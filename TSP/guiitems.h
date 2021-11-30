#ifndef GUIITEMS_H
#define GUIITEMS_H

#include "tsp.h"
#include <QTableView>
#include <QDebug>
#include <QAbstractTableModel>
#include <QStandardItem>
#include <QMessageBox>
#include <QInputDialog>
#include <QApplication>
#include <QClipboard>
#include <QHeaderView>
//#include <QSortFilterProxyModel>

//---------------------------------------------------------------------------------------------------------------------------------------------------//
class IconDir
{
public:
    QIcon iconEmpty   = QIcon(":/rsrc/indicator_empty.png");
    QIcon iconFalse   = QIcon(":/rsrc/indicator_false.png");
    QIcon iconTrue    = QIcon(":/rsrc/indicator_true.png");
    QIcon iconUnknown = QIcon(":/rsrc/indicator_unknown.png");
    QIcon iconCheck   = QIcon(":/rsrc/indicator_check.png");
    QIcon iconError   = QIcon(":/rsrc/indicator_error.png");
    QIcon iconFloat   = QIcon(":/rsrc/type_float.png");
    QIcon iconInt     = QIcon(":/rsrc/type_int.png");
    QIcon iconTError  = QIcon(":/rsrc/type_error.png");
    QIcon iconRW      = QIcon(":/rsrc/access_rw.png");
    QIcon iconRO      = QIcon(":/rsrc/access_ro.png");
    QIcon iconWO      = QIcon(":/rsrc/access_wo.png");
    QIcon iconNA      = QIcon(":/rsrc/access_na.png");
    QIcon iconSIMATIC = QIcon(":/rsrc/driver_simatic.png");
    QIcon iconMBRTU   = QIcon(":/rsrc/driver_mbrtu.png");
    QIcon iconMBTCP   = QIcon(":/rsrc/driver_mbtcp.png");
    QIcon iconDError  = QIcon(":/rsrc/driver_error.png");

    QBrush fillModified = QColor(219, 219, 219);
    QBrush fillError    = QColor(203, 28, 28);
    QBrush fillDefault  = QColor(255, 255, 255);
};
//---------------------------------------------------------------------------------------------------------------------------------------------------//
class OptionsDir
{
public:
    enum Options{
        DefaultOptions = 0xFEFF,
        ShowTagAccess = 0x0001,
        ShowDriverTypeLogo = 0x0002,
        ShowSystemLog = 0x0004,
        ShowUserLog = 0x0008,
//        HighlightNATags = 0x0010,
        AllowToWriteTags = 0x0020,
        AllowToManageDrivers = 0x0040,
//        AllowToGetError = 0x0080,
        AllowToEdit = 0x0100,
        AllowToChangeEdit = 0x0200,
    };
};
//---------------------------------------------------------------------------------------------------------------------------------------------------//
class TableDir
{
public:
    enum TagTable              { tagId = 0   , tagGroupId   , tagQuality  , tagName   , tagOptions   ,tagAddress    , tagValue     , tagComment    };
    QStringList tagHeader    = { "id"        , "groupId"    , "quality"   , "name"    , "options"    ,"address"     , "value"      , "comment"     };
    enum GroupTable            { groupId = 0 , groupDriverId, groupQuality, groupName , groupOptions ,groupDelay    , groupComment                 };
    QStringList groupHeader  = { "id"        , "driverId"   , "quality"   , "name"    , "options"    ,"delay"       , "comment"                    };
    enum DriverTable           { driverId = 0, driverStatus , driverType  , driverName, driverOptions, driverComment                               };
    QStringList driverHeader = { "id"        , "status"     , "type"      , "name"    , "options"    , "comment"                                   };
};
//---------------------------------------------------------------------------------------------------------------------------------------------------//
class TSPTableModel : public QAbstractTableModel, public OptionsDir, public TableDir
{
    Q_OBJECT
public:
    explicit TSPTableModel(int & options, IconDir * icons = new IconDir(), QObject *parent = nullptr);
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual int rowCount(const QModelIndex& parent= QModelIndex()) const override = 0;
    void appendHeader(Qt::Orientation orientation, QStringList strings);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
protected:
    bool editing = false;
    int & options;
    const int undoStackSize = 30;
    QStringList vheader;
    QStringList hheader;
    IconDir * icons = nullptr;
    void keyDelete(QModelIndex index, QModelIndexList indexes);
    void keyCtrlC(QModelIndex index, QModelIndexList indexes);
    void keyCtrlV(QModelIndex index, QModelIndexList indexes);
    void keyBackspace(QModelIndex index, QModelIndexList indexes);
public slots:
    void updateRequest();
    virtual void doubleClicked (const QModelIndex & index) = 0;
    void keyPressEvent(QKeyEvent *event, QModelIndex index, QModelIndexList indexes);
    void addRows();
    void fillValues(QModelIndex index, QModelIndexList indexes);
    void incrementValues(QModelIndex index, QModelIndexList indexes);
};
//---------------------------------------------------------------------------------------------------------------------------------------------------//
class TSPTableModelDrivers : public TSPTableModel
{
    Q_OBJECT
public:
    explicit TSPTableModelDrivers(QList<DriverConfig*>& listOfDriverConfigs, int & options, IconDir * icons = new IconDir(), QObject *parent = nullptr);
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    int rowCount(const QModelIndex& parent= QModelIndex()) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int position, int rows, const QModelIndex &index=QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index=QModelIndex()) override;
private:
    QList<DriverConfig*>& listOfDriverConfigs;
public slots:
    void doubleClicked (const QModelIndex & index) override;
};
//---------------------------------------------------------------------------------------------------------------------------------------------------//
class TSPTableModelGroups : public TSPTableModel
{
    Q_OBJECT
public:
    explicit TSPTableModelGroups(QList<GroupConfig*>& listOfGroupConfigs, int & options, IconDir * icons = new IconDir(), QObject *parent = nullptr);
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    int rowCount(const QModelIndex& parent= QModelIndex()) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int position, int rows, const QModelIndex &index=QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index=QModelIndex()) override;
private:
    QList<GroupConfig*>& listOfGroupConfigs;
public slots:
    void doubleClicked (const QModelIndex & index) override;
};
//---------------------------------------------------------------------------------------------------------------------------------------------------//
class TSPTableModelTags : public TSPTableModel
{
    Q_OBJECT
public:
    explicit TSPTableModelTags(QList<TagConfig*>& listOfTagConfigs, int & options, IconDir * icons = new IconDir(), QObject *parent = nullptr);
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    int rowCount(const QModelIndex& parent= QModelIndex()) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int position, int rows, const QModelIndex &index=QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index=QModelIndex()) override;
private:
    QList<TagConfig*>& listOfTagConfigs;
public slots:
    void doubleClicked (const QModelIndex & index) override;
};
//---------------------------------------------------------------------------------------------------------------------------------------------------//
class TSPTableView : public QTableView
{
    Q_OBJECT
public:
    explicit TSPTableView(QWidget* parent = Q_NULLPTR)
        : QTableView(parent){
        setEditTriggers(QAbstractItemView::AllEditTriggers);
        verticalHeader()->setDefaultSectionSize(20);
    };
    virtual ~TSPTableView(){};
    using QTableView::setModel;
    void setModel(TSPTableModel * model);
    void fillValues(){
        emit onFillValues(currentIndex(), selectedIndexes());
    };
    void incrementValues(){
        emit onIncrementValues(currentIndex(), selectedIndexes());
    };
protected:
    void keyPressEvent(QKeyEvent *event) override{
        emit onKeyPressEvent(event, currentIndex(), selectedIndexes());
    };
signals:
    void onUpdateRequested();
    void onKeyPressEvent(QKeyEvent *event, QModelIndex index, QModelIndexList indexes);
    void onAddRows();
    void onFillValues(QModelIndex index, QModelIndexList indexes);
    void onIncrementValues(QModelIndex index, QModelIndexList indexes);
};
//---------------------------------------------------------------------------------------------------------------------------------------------------//
#endif // GUIITEMS_H
