#ifndef DRIVER_H
#define DRIVER_H

#include <QObject>
#include <QThread>
#include "group.h"
#include "enums.h"
#include <QTimer>

class Driver : public QObject
{
    Q_OBJECT
public:
    //destructor
    virtual ~Driver();
    //variables
    QList<Group*> listOfGroups;
    int Id;
    QString Comment;
    QString Options;
    bool started = false;
    QString type;
    //methods
    bool InsertGroup(Group  * group);
    Group * getGroupById(int Id);
    virtual void connect() = 0;
    virtual void disconnect() = 0;
protected:

//    struct Task {
//        int quantity = 0, groupId;
//        QList<Tag*> listOfTags;
//        bool writeTask = false;
//        void * customAddr = nullptr;
//    };
    //variables
    QThread * thread = nullptr;
    QTimer * taskTimer;
    bool noError = true;
    //methods
    void qualityFiller(QList<Tag*> listOfTags, Quality quality);
    void qualityFiller(QList<Group*> listOfGroups, Quality quality);
    void newValueFiller(QList<Tag*> listOfTags);
    void readyFiller(QList<Tag*> listOfTags, bool ready);
    void errorFiller(QList<Tag*> listOfTags, QString error);
    QList<Tag*> prepareTagsList(QList<Tag*> listOfTags);
    virtual QList<Tag*> sortTags(QList<Tag*> listOfTags) = 0;
    virtual void handleNextTask() = 0;
signals:
    void onStartedChanged();
    void connectDriver();
    void disconnectDriver();
    void LoggingSig(MessType MessTypeID,  QDateTime DateTime, bool UserOrSys, QString Source, QString Message);
public slots:
    virtual void WriteRequested(Tag * tag) = 0;
protected slots:
    virtual void TagInserted(Group * group);
};

#endif // DRIVER_H
