#ifndef DRIVER_H
#define DRIVER_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include "group.h"
#include "enums.h"

struct CommonTask
{
    int groupId{0};
    QList<Tag*> listOfTags;
    bool writeTask = false;
    int quantity{0}; //TODO проверит в ModBus и убрать в listOfTags это есть
};

class Driver : public QObject
{
    Q_OBJECT
public:
    //destructor
    virtual ~Driver();
    //variables
    int id;
    QString comment;
    QString options;
    bool started = false;
    QString type;
    //methods
    virtual bool insertGroup(Group  * group);
    Group * getGroupById(int Id);
    virtual void connect() = 0;
    virtual void disconnect() = 0;

    const QList<Group *> &getListOfGroups() const;
    void setListOfGroups(const QList<Group *> &newListOfGroups);

  protected:
    //variables
    QThread * thread = nullptr;
    QTimer * taskTimer;
    bool noError = true;
    int errorCode {0};
    QList<Group*> listOfGroups;
    //methods
    void qualityFiller(QList<Tag*> listOfTags, Quality quality);
    void qualityFiller(QList<Group*> listOfGroups, Quality quality);
    void newValueFiller(QList<Tag*> listOfTags);
    void readyFiller(QList<Tag*> listOfTags, bool ready);
    void errorFiller(QList<Tag*> listOfTags, QString error);
    // каждый драйвер подставляет сюда свою сортировку,
    //TODO но лучше переделать
    static void sortTags(QList<Tag*> &){};
    virtual void handleNextTask() = 0;
signals:
    void s_onStartedChanged();
    void s_connectDriver();
    void s_disconnectDriver();
    void s_logging(MessType MessTypeID,  QDateTime DateTime, bool UserOrSys, QString Source, QString Message);
public slots:
    virtual void createWriteTask(Tag * tag) = 0;
protected slots:
    //virtual void TagInserted(Group * group);
};

#endif // DRIVER_H
