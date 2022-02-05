#ifndef TSP_H
#define TSP_H

#include <QObject>
#include <QSettings>
#include <QKeyEvent>
#include "tag.h"
#include "group.h"
#include "driver.h"
#include "modbus.h"
#include "simatic.h"
#include "enums.h"

#include "SCADAenums.h"

class TSP : public QObject
{
    Q_OBJECT
public:
    //constructor
    explicit TSP(QObject *parent = nullptr, QString name = "");
    //destructor
    ~TSP();
    //variables

    QList<DriverConfig*> listOfDriverConfigs;
    QList<GroupConfig*> listOfGroupConfigs;
    QList<TagConfig*> listOfTagConfigs;

    QList<Driver*> listOfDrivers;
    QList<Group*> listOfGroups;
    QList<Tag*> listOfTags;

    const QString version = "3.0b";
    QSettings * ini = nullptr;
    //methods
    bool addDriver(int id, QString name, QString type, QString options, QString comment); //for external use
    bool addGroup(int id, QString name, QString options, int delay, int driverId, QString comment); //for external use
    bool addTag(int id, QString name, QString address, QString options, int groupId, QString comment); //for external use
    bool addTag(Tag * tag); //for external use
    void init(); //for external use
    void showGUI();
    void start(); //for external use
    void stop(); //for external use
    Tag * getTagById(int id); //for external use
    Tag * getTagByName(QString name); //for external use
    Tag * getTagByAddress(int groupId, QString addr); //for external use
    Driver * getDriverById(int id); //for external use
    Group * getGroupById(int id); //for external use
    void LoadIniFromOld(QString FileName = "");
    void LoadIni(QString FileName = "tagsNEW.ini");
    void SaveIni(QString FileName = "tagsNEW.ini");
    void loadJson(QString FileName = "tags.json");
    void saveJson(QString FileName = "tags.json");
signals:
    void s_logging(MessType MessTypeID,  QDateTime DateTime, bool UserOrSys, QString Source, QString Message);
private:
    //variables
    //int maxId = 0;
    int maxTagId();
    int nextTagId(int preId);
};

#endif // TSP_H
