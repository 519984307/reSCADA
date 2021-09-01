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
    bool AddDriver(int id, QString name, QString type, QString options, QString comment); //for external use
    bool AddGroup(int id, QString name, QString options, int delay, int driverId, QString comment); //for external use
    bool addTag(int id, QString name, QString address, QString options, int groupId, QString comment); //for external use
    bool addTag(Tag * tag); //for external use
    void Init(); //for external use
    void ShowGUI();
    void Start(); //for external use
    void Stop(); //for external use
    Tag * GetTagById(int id); //for external use
    Tag * GetTagByName(QString name); //for external use
    Tag * GetTagByAddress(int groupId, QString addr); //for external use
    Driver * GetDriverById(int id); //for external use
    Group * GetGroupById(int id); //for external use
    void LoadIniFromOld(QString FileName = "");
    void LoadIni(QString FileName = "tagsNEW.ini");
    void SaveIni(QString FileName = "tagsNEW.ini");
    void LoadJson(QString FileName = "tags.json");
    void SaveJson(QString FileName = "tags.json");
signals:
    void LoggingSig(MessType MessTypeID,  QDateTime DateTime, bool UserOrSys, QString Source, QString Message);
private:
    //variables
    //int maxId = 0;
    int maxTagId();
    int nextTagId(int preId);
};

#endif // TSP_H
