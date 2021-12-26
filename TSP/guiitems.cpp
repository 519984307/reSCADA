#include "guiitems.h"
#include <QSortFilterProxyModel>
//------------------------------------------------------------------------------
TSPTableModel::TSPTableModel(int &options, IconDir *icons, QObject *parent)
    : QAbstractTableModel(parent),
      options(options),
      icons(icons)
{
}
//------------------------------------------------------------------------------

void TSPTableView::setModel(TSPTableModel *model){
    //QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    //proxyModel->setSourceModel( model );
    QTableView::setModel(model);
    connect(this, &TSPTableView::doubleClicked, model, &TSPTableModel::doubleClicked);
    connect(this, &TSPTableView::onKeyPressEvent, model, &TSPTableModel::keyPressEvent);
    connect(this, &TSPTableView::onAddRows, model, &TSPTableModel::addRows);
    connect(this, &TSPTableView::onUpdateRequested, model, &TSPTableModel::updateRequest);
    connect(this, &TSPTableView::onFillValues, model, &TSPTableModel::fillValues);
    connect(this, &TSPTableView::onIncrementValues, model, &TSPTableModel::incrementValues);
    resizeColumnsToContents();
}
//------------------------------------------------------------------------------
int TSPTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return hheader.count();
}
//------------------------------------------------------------------------------
void TSPTableModel::appendHeader(Qt::Orientation orientation, QStringList strings)
{
    foreach (QString string, strings) {
        if (orientation == Qt::Horizontal) hheader.append(string);
        else if (orientation == Qt::Vertical) vheader.append(string);
    }
}
//------------------------------------------------------------------------------
QVariant TSPTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) return QVariant();
    else if (orientation == Qt::Horizontal){
        if (hheader.count()>section) return hheader[section];
        else return QVariant();
    }
    else if (orientation == Qt::Vertical){
        if (vheader.count()>section) return vheader[section];
        else return QVariant();
    }
    else return QVariant();
}
//------------------------------------------------------------------------------
void TSPTableModel::keyDelete(QModelIndex index, QModelIndexList indexes)
{
    QMessageBox msgBox;
    int rows = indexes.count() > 1 ? (rows = indexes.last().row()-indexes.first().row()+1) : 1 ;
    msgBox.setText("Вы уверены, что хотите удалить " + QString::number(rows)
                   + (rows%10==1?" ячейку?":(rows%10>1 && rows%10<5?" ячейки?":" ячеек?")));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int res = msgBox.exec();
    if (res == QMessageBox::Yes){
        removeRows(indexes.count() > 1 ? indexes.first().row() : index.row(), rows);
    }
}
//------------------------------------------------------------------------------
void TSPTableModel::keyCtrlC(QModelIndex index, QModelIndexList indexes)
{
    QClipboard* c = QApplication::clipboard();
    if(indexes.count() > 1){
        QString clipboarddata = "";
        for (int i = 0; i < indexes.count(); i++) {
            clipboarddata += i == 0 ? "" : (indexes[i].row() != indexes[i-1].row()) ? "\n" : "\t";
            clipboarddata += data(indexes[i]).toString();
        }
        c->setText(clipboarddata);
    }else{
        c->setText(data(index).toString());
    }
}
//------------------------------------------------------------------------------
void TSPTableModel::keyCtrlV(QModelIndex index, QModelIndexList indexes)
{
    Q_UNUSED(index);
    QClipboard* c = QApplication::clipboard();
    QString clipboarddata = c->text();
    QStringList cliprows = clipboarddata.split("\n", QString::SkipEmptyParts);
    int row = 0;
    int column = 0;
    for (int i = 0; i < indexes.count(); i++) {
        if(i != 0 && indexes[i].row() != indexes[i-1].row()){
            row++;
            column = 0;
        }
        if (cliprows.count() <= row) continue; //overflow defender
        QStringList clipcolumns = cliprows[row].split("\t");
        if (clipcolumns.count() <= column) continue; //overflow defender
        setData(indexes[i], clipcolumns[column]);
        //qDebug() << indexes[i].row() << indexes[i].column() << clipcolumns[column];
        column++;
    }
}
//------------------------------------------------------------------------------
void TSPTableModel::keyBackspace(QModelIndex index, QModelIndexList indexes)
{
    if (indexes.count() > 1)
        foreach (QModelIndex index, indexes)
            setData(index, "");
    else
        setData(index, "");
}
//------------------------------------------------------------------------------
void TSPTableModel::updateRequest()
{
    emit dataChanged(createIndex(0,0), createIndex(rowCount(), columnCount()));
}
//------------------------------------------------------------------------------
void TSPTableModel::keyPressEvent(QKeyEvent *event, QModelIndex index, QModelIndexList indexes)
{
//    if(!index.isValid())
//        return;
//    else if(event->key() == Qt::Key_Delete && (options & AllowToEdit) != 0){
//        keyDelete(index, indexes);
//    }else if(event->key() == Qt::Key_C && event->modifiers() == Qt::ControlModifier){
//        keyCtrlC(index, indexes);
//    }else if(event->key() == Qt::Key_V && event->modifiers() == Qt::ControlModifier && (options & AllowToEdit) != 0){
//        keyCtrlV(index, indexes);
//    }else if(event->key() == Qt::Key_Backspace && (options & AllowToEdit) != 0){
//        keyBackspace(index, indexes);
//    }
}
//------------------------------------------------------------------------------
void TSPTableModel::addRows()
{
    bool ok = false;
    int count = QInputDialog::getInt(nullptr, "Добавление", "Введите количество строк:", 1, 1, std::numeric_limits<int>::max(), 1, &ok);
    if (ok && options && AllowToEdit != 0){
        insertRows(0, count);
    }
}
//------------------------------------------------------------------------------
void TSPTableModel::fillValues(QModelIndex index, QModelIndexList indexes)
{
    Q_UNUSED(index);
    if(indexes.count() < 2){
        QMessageBox::critical(nullptr, "Ошибка заполнения", "Выделите не менее двух ячеек");
        return;
    }
    bool ok = false;
    QString value = QInputDialog::getText(nullptr, "Добавление",
                                          "Введите значение, которое повторится в выделенных местах:",
                                          QLineEdit::Normal, "", &ok);
    if (ok && options && AllowToEdit != 0){
        foreach (QModelIndex index, indexes) {
            setData(index, value);
        }
    }
}
//------------------------------------------------------------------------------
void TSPTableModel::incrementValues(QModelIndex index, QModelIndexList indexes)
{
    Q_UNUSED(index);
    if(indexes.count() < 2){
        QMessageBox::critical(nullptr, "Ошибка заполнения", "Выделите не менее двух ячеек");
        return;
    }
    bool ok = false;
    int value = QInputDialog::getInt(nullptr, "Добавление",
                                          "Введите значение, которое будет увеличиваться в каждой следующей выделенной ячейке:",
                                          1, 1, std::numeric_limits<int>::max(), 1, &ok);
    if (ok && options && AllowToEdit != 0){
        foreach (QModelIndex index, indexes) {
            setData(index, value++);
        }
    }
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
TSPTableModelDrivers::TSPTableModelDrivers(QList<DriverConfig *> &listOfDriverConfigs, int &options, IconDir *icons, QObject *parent)
    : TSPTableModel(options, icons, parent),
      listOfDriverConfigs(listOfDriverConfigs)
{
    appendHeader(Qt::Horizontal, driverHeader);
    foreach (DriverConfig * config, listOfDriverConfigs) {
        if(config->driver)
            connect(config->driver, &Driver::s_onStartedChanged, this, &TSPTableModel::updateRequest);
    }
}
//------------------------------------------------------------------------------
QVariant TSPTableModelDrivers::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    else if(role == Qt::DisplayRole || role == Qt::EditRole){
        DriverConfig * config = listOfDriverConfigs[index.row()];
        switch(index.column()){
        case driverId:      return config->id;
        case driverStatus:  return config->driver?(config->driver->started?"Run":"Stop"):"Error";
        case driverType:    return config->type;
        case driverName:    return config->name;
        case driverOptions: return config->options;
        case driverComment: return config->comment;
        default: return QVariant();
        }
    }
    else if(role == Qt::BackgroundRole){
        DriverConfig * config = listOfDriverConfigs[index.row()];
        if (config->modified)               return icons->fillModified;
        else if(config->errorString != "")  return icons->fillError;
        else                                return icons->fillDefault;
    }
    else if(role == Qt::ToolTipRole){
        DriverConfig * config = listOfDriverConfigs[index.row()];
        if (config->errorString != "")
            return config->errorString;
        else
            return QVariant();
    }
    else if(role == Qt::DecorationRole){
        switch(index.column()){
        case driverStatus: {
            Driver * driver = listOfDriverConfigs[index.row()]->driver;
            if(driver)
                return driver->started ? icons->iconTrue : icons->iconFalse;
            else
                return icons->iconError;
        }
        case driverType: {
            if((options & ShowDriverTypeLogo) != 0){
                DriverConfig * config = listOfDriverConfigs[index.row()];
                if      (config->type == "simatic")     return icons->iconSIMATIC;
                else if (config->type == "modbusRTU")   return icons->iconMBRTU;
                else if (config->type == "modbusTCP")   return icons->iconMBTCP;
                else                                    return icons->iconDError;
            }else
                return QVariant();
        }
        default: return QVariant();
        }
    }
    else
        return QVariant();
}
//------------------------------------------------------------------------------
Qt::ItemFlags TSPTableModelDrivers::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    else if(index.column() != driverStatus)
        return QAbstractTableModel::flags(index) | ((options & AllowToEdit) != 0 && editing ? Qt::ItemIsEditable : Qt::ItemIsSelectable);
    //        else if((options & AllowToManageDrivers) != 0 && index.column() == driverStatus)
    //            return QAbstractTableModel::flags(index) | Qt::ItemIsSelectable;
    else
        return Qt::ItemIsEnabled;
}
//------------------------------------------------------------------------------
int TSPTableModelDrivers::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return listOfDriverConfigs.count();
}
//------------------------------------------------------------------------------
bool TSPTableModelDrivers::setData(const QModelIndex &index, const QVariant &value, int role)
{
    editing = false;
    if (index.isValid() && role == Qt::EditRole && (options & AllowToEdit) != 0){
        DriverConfig * config = listOfDriverConfigs[index.row()];
        switch (index.column()) {
        case driverId:      if (config->id      != value.toString().toInt()) { config->id      = value.toString().toInt(); config->modified = true; }; break;
        case driverName:    if (config->name    != value.toString()        ) { config->name    = value.toString();         config->modified = true; }; break;
        case driverType:    if (config->type    != value.toString()        ) { config->type    = value.toString();         config->modified = true; }; break;
        case driverOptions: if (config->options != value.toString()        ) { config->options = value.toString();         config->modified = true; }; break;
        case driverComment: if (config->comment != value.toString()        ) { config->comment = value.toString();         config->modified = true; }; break;
        default: return false;
        }
        emit(dataChanged(index, index));
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool TSPTableModelDrivers::insertRows(int position, int rows, const QModelIndex &index)
{
    beginInsertRows(QModelIndex(), position, position+rows-1);
    Q_UNUSED(index);
    DriverConfig * config;
    int biggestId = std::numeric_limits<int>::min();
    foreach (config, listOfDriverConfigs) {
        if (biggestId < config->id)
            biggestId = config->id;
    }
    for (int i = 0; i < rows; i++) {
        config = new DriverConfig();
        config->id = ++biggestId;
        config->name = "unknownDriver" + QString::number(config->id);
        config->type = "modbusTCP";
        config->options = "address=127.0.0.1:502";
        config->modified = true;
        listOfDriverConfigs.append(config);
    }
    updateRequest();
    endInsertRows();
    return true;
}
//------------------------------------------------------------------------------
bool TSPTableModelDrivers::removeRows(int position, int rows, const QModelIndex &index)
{
    beginRemoveRows(QModelIndex(), position, position+rows-1);
    Q_UNUSED(index);
    for(int i = 0; i < rows; i++){
        DriverConfig * config = listOfDriverConfigs.takeAt(position);
        Q_UNUSED(config);
    }
    updateRequest();
    endRemoveRows();
    return true;
}
//------------------------------------------------------------------------------
void TSPTableModelDrivers::doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;
    else if((options & AllowToManageDrivers) != 0 && index.column() == driverStatus){
        Driver * driver = listOfDriverConfigs[index.row()]->driver;
        if (driver)
            driver->started ? emit driver->s_disconnectDriver() : emit driver->s_connectDriver();
        updateRequest();
    }else if((options & AllowToEdit) != 0 && index.column() != driverStatus)
        editing = true;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
TSPTableModelGroups::TSPTableModelGroups(QList<GroupConfig *> &listOfGroupConfigs, int &options, IconDir *icons, QObject *parent)
    : TSPTableModel(options, icons, parent),
      listOfGroupConfigs(listOfGroupConfigs)
{
    appendHeader(Qt::Horizontal, groupHeader);
    foreach (GroupConfig * config, listOfGroupConfigs) {
        if(config->group)
            connect(config->group, &Group::s_onQualityChanged, this, &TSPTableModel::updateRequest);
    }
}
//------------------------------------------------------------------------------
QVariant TSPTableModelGroups::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    else if(role == Qt::DisplayRole || role == Qt::EditRole){
        GroupConfig * config = listOfGroupConfigs[index.row()];
        switch(index.column()){
        case groupId:       return config->id;
        case groupDriverId: return config->driverId;
        case groupQuality:  return config->group?(Prom::qualityToString(config->group->readQuality())):"Error";
        case groupName:     return config->name;
        case groupOptions:  return config->options;
        case groupDelay:    return config->delay;
        case groupComment:  return config->comment;
        default: return QVariant();
        }
    }
    else if(role == Qt::BackgroundRole){
        GroupConfig * config = listOfGroupConfigs[index.row()];
        if (config->modified)               return icons->fillModified;
        else if(config->errorString != "")  return icons->fillError;
        else                                return icons->fillDefault;
    }
    else if(role == Qt::ToolTipRole){
        GroupConfig * config = listOfGroupConfigs[index.row()];
        if (config->errorString != "")
            return config->errorString;
        else
            return QVariant();
    }
    else if(role == Qt::DecorationRole){
        switch(index.column()){
        case groupQuality: {
            Group * group = listOfGroupConfigs[index.row()]->group;
            if(group)
                switch (group->readQuality()) {
                case tsp_enums::Good:       return icons->iconTrue;
                case tsp_enums::Bad:        return icons->iconFalse;
                case tsp_enums::Check:      return icons->iconCheck;
                case tsp_enums::Unknown:    return icons->iconUnknown;
                }
            else
                return icons->iconError;
        }
        default: return QVariant();
        }
    }
    else
        return QVariant();
}
//------------------------------------------------------------------------------
Qt::ItemFlags TSPTableModelGroups::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    else if(index.column() != groupQuality)
        return QAbstractTableModel::flags(index) | ((options & AllowToEdit) != 0 && editing ? Qt::ItemIsEditable : Qt::ItemIsSelectable);
    else
        return Qt::ItemIsEnabled;
}
//------------------------------------------------------------------------------
int TSPTableModelGroups::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return listOfGroupConfigs.count();
}
//------------------------------------------------------------------------------
bool TSPTableModelGroups::setData(const QModelIndex &index, const QVariant &value, int role)
{
    editing = false;
    if (index.isValid() && role == Qt::EditRole && (options & AllowToEdit) != 0){
        GroupConfig * config = listOfGroupConfigs[index.row()];
        switch (index.column()) {
        case groupId:       if (config->id      != value.toString().toInt()) { config->id      = value.toString().toInt(); config->modified = true; }; break;
        case groupDriverId: if (config->driverId!= value.toString().toInt()) { config->driverId= value.toString().toInt(); config->modified = true; }; break;
        case groupName:     if (config->name    != value.toString()        ) { config->name    = value.toString();         config->modified = true; }; break;
        case groupOptions:  if (config->options != value.toString()        ) { config->options = value.toString();         config->modified = true; }; break;
        case groupDelay:    if (config->delay   != value.toString().toInt()) { config->delay   = value.toString().toInt(); config->modified = true; }; break;
        case groupComment:  if (config->comment != value.toString()        ) { config->comment = value.toString();         config->modified = true; }; break;
        default: return false;
        }
        emit(dataChanged(index, index));
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool TSPTableModelGroups::insertRows(int position, int rows, const QModelIndex &index)
{
    beginInsertRows(QModelIndex(), position, position+rows-1);
    Q_UNUSED(index);
    GroupConfig * config;
    int biggestId = std::numeric_limits<int>::min();
    foreach (config, listOfGroupConfigs) {
        if (biggestId < config->id)
            biggestId = config->id;
    }
    for (int i = 0; i < rows; i++) {
        config = new GroupConfig();
        config->id = ++biggestId;
        config->driverId = 0;
        config->name = "unknownGroup" + QString::number(config->id);
        config->delay = 100;
        config->options = "ri=10, rm=200";
        config->modified = true;
        listOfGroupConfigs.append(config);
    }
    updateRequest();
    endInsertRows();
    return true;
}
//------------------------------------------------------------------------------
bool TSPTableModelGroups::removeRows(int position, int rows, const QModelIndex &index)
{
    beginRemoveRows(QModelIndex(), position, position+rows-1);
    Q_UNUSED(index);
    for(int i = 0; i < rows; i++){
        GroupConfig * config = listOfGroupConfigs.takeAt(position);
        Q_UNUSED(config);
    }
    updateRequest();
    endRemoveRows();
    return true;
}
//------------------------------------------------------------------------------
void TSPTableModelGroups::doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;
    else if((options & AllowToEdit) != 0 && index.column() != groupQuality)
        editing = true;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
TSPTableModelTags::TSPTableModelTags(QList<TagConfig *> &listOfTagConfigs, int &options, IconDir *icons, QObject *parent)
    : TSPTableModel(options, icons, parent),
      listOfTagConfigs(listOfTagConfigs)
{
    appendHeader(Qt::Horizontal, tagHeader);
    foreach (TagConfig * config, listOfTagConfigs) {
        if(config->tag){
            connect(config->tag, &Tag::s_onQualityChanged, this, &TSPTableModel::updateRequest);
            connect(config->tag, &Tag::s_onValueChanged, this, &TSPTableModel::updateRequest);
        }
    }
}
//------------------------------------------------------------------------------
QVariant TSPTableModelTags::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    else if(role == Qt::DisplayRole || role == Qt::EditRole){
        TagConfig * config = listOfTagConfigs[index.row()];
        switch(index.column()){
        case tagId:       return config->id;
        case tagGroupId:  return config->groupId;
        case tagQuality:  return config->tag?(Prom::qualityToString(config->tag->readQuality())):"Error";
        case tagName:     return config->name;
        case tagOptions:  return config->options;
        case tagAddress:  return config->address;
        case tagValue:    {
            if (config->tag){
                QVariant value = config->tag->readValue();
                switch (config->tag->type) {
                case TInt: return value.toInt();
                case TFloat: return value.toDouble();
                case TBool: return value.toBool() ? "True" : "False";
                }
            }else
                return "Error";
        }
        case tagComment:  return config->comment;
        default: return QVariant();
        }
    }
    else if(role == Qt::BackgroundRole){
        TagConfig * config = listOfTagConfigs[index.row()];
        if (config->modified)               return icons->fillModified;
        else if(config->errorString != "")  return icons->fillError;
        else                                return icons->fillDefault;
    }
    else if(role == Qt::ToolTipRole){
        TagConfig * config = listOfTagConfigs[index.row()];
        if (config->errorString != "")
            return config->errorString;
        else
            return QVariant();
    }
    else if(role == Qt::DecorationRole){
        switch(index.column()){
        case tagQuality: {
            Tag * tag = listOfTagConfigs[index.row()]->tag;
            if(tag)
                switch (tag->readQuality()) {
                case tsp_enums::Good:       return icons->iconTrue;
                case tsp_enums::Bad:        return icons->iconFalse;
                case tsp_enums::Check:      return icons->iconCheck;
                case tsp_enums::Unknown:    return icons->iconUnknown;
                }
            else
                return icons->iconError;
        }
        case tagValue: {
            Tag * tag = listOfTagConfigs[index.row()]->tag;
            if(tag)
                switch (tag->type) {
                case TInt: return icons->iconInt;
                case TFloat: return icons->iconFloat;
                case TBool: return tag->readValue().toBool() ? icons->iconTrue : icons->iconFalse;
                }
            else
                return icons->iconError;
        }
        case tagAddress: {
            if((options & ShowTagAccess) == 0)
                return QVariant();
            Tag * tag = listOfTagConfigs[index.row()]->tag;
            if(tag)
                switch (tag->access) {
                case RW: return icons->iconRW;
                case RO: return icons->iconRO;
                case WO: return icons->iconWO;
                case NA: return icons->iconNA;
                }
            else
                return icons->iconError;
        }
        default: return QVariant();
        }
    }
    else
        return QVariant();
}
//------------------------------------------------------------------------------
Qt::ItemFlags TSPTableModelTags::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    else if(index.column() != tagQuality && index.column() != tagValue)
        return QAbstractTableModel::flags(index) | ((options & AllowToEdit) != 0 && editing ? Qt::ItemIsEditable : Qt::ItemIsSelectable);
    else
        return Qt::ItemIsEnabled;
}
//------------------------------------------------------------------------------
int TSPTableModelTags::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return listOfTagConfigs.count();
}
//------------------------------------------------------------------------------
bool TSPTableModelTags::setData(const QModelIndex &index, const QVariant &value, int role)
{
    editing = false;
    if (index.isValid() && role == Qt::EditRole && (options & AllowToEdit) != 0){
        TagConfig * config = listOfTagConfigs[index.row()];
        switch (index.column()) {
        case tagId:       if (config->id      != value.toString().toInt()) { config->id      = value.toString().toInt(); config->modified = true; }; break;
        case tagGroupId:  if (config->groupId != value.toString().toInt()) { config->groupId = value.toString().toInt(); config->modified = true; }; break;
        case tagName:     if (config->name    != value.toString()        ) { config->name    = value.toString();         config->modified = true; }; break;
        case tagOptions:  if (config->name    != value.toString()        ) { config->options = value.toString();         config->modified = true; }; break;
        case tagAddress:  if (config->address != value.toString()        ) { config->address = value.toString();         config->modified = true; }; break;
        case tagComment:  if (config->comment != value.toString()        ) { config->comment = value.toString();         config->modified = true; }; break;
        default: return false;
        }
        emit(dataChanged(index, index));
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool TSPTableModelTags::insertRows(int position, int rows, const QModelIndex &index)
{
    beginInsertRows(QModelIndex(), position, position+rows-1);
    Q_UNUSED(index);
    TagConfig * config;
    int biggestId = std::numeric_limits<int>::min();
    foreach (config, listOfTagConfigs) {
        if (biggestId < config->id)
            biggestId = config->id;
    }
    for (int i = 0; i < rows; i++) {
        config = new TagConfig();
        config->id = ++biggestId;
        config->groupId = 0;
        config->name = "unknownTag" + QString::number(config->id);
        config->options = "bool";
        config->modified = true;
        listOfTagConfigs.append(config);
    }
    updateRequest();
    endInsertRows();
    return true;
}
//------------------------------------------------------------------------------
bool TSPTableModelTags::removeRows(int position, int rows, const QModelIndex &index)
{
    beginRemoveRows(QModelIndex(), position, position+rows-1);
    Q_UNUSED(index);
    for(int i = 0; i < rows; i++){
         TagConfig * config = listOfTagConfigs.takeAt(position);
         Q_UNUSED(config);
    }
    updateRequest();
    endRemoveRows();
    return true;
}
//------------------------------------------------------------------------------
void TSPTableModelTags::doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;
    else if((options & AllowToEdit) != 0 && (index.column() != tagValue || index.column() != tagQuality))
        editing = true;
    else if((options & AllowToWriteTags) != 0 && (index.column() == tagValue)){
        Tag * tag = listOfTagConfigs[index.row()]->tag;
        if(tag){
            switch(tag->type){
            case TInt: {
                bool ok = false;
                int value = QInputDialog::getInt(nullptr, QString::fromUtf8("Изменение"), QString::fromUtf8("Введите новое значение:"),
                                                tag->readValue().toInt(), std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), 1, &ok);
                if (ok){
                    tag->writeValue(value);
                }
                return;
            }
            case TFloat: {
                bool ok = false;
                double value = QInputDialog::getDouble(nullptr, QString::fromUtf8("Изменение"), QString::fromUtf8("Введите новое значение:"),
                                                tag->readValue().toDouble(), std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), 1, &ok);
                if (ok){
                    tag->writeValue(value);
                }
            }
            case TBool: {
                tag->writeValue(!tag->readValue().toBool());
                return;
            }
            default: return;
            }
        }
    }
}
//------------------------------------------------------------------------------
