#ifndef GROUP_H
#define GROUP_H

#include <QObject>
#include <QDateTime>
#include <QList>
#include "tag.h"

class Group : public QObject
{
    Q_OBJECT
public:
    //constructor
    Group(int id, int driverId, QString name, int delay, int optimizerRangeInterval = 4, int optimizerRangeMax = 70, QString Comment = "");
    //destructor
    ~Group();
    //variables
    int Delay;
    QDateTime LastUpdate;
    QList<Tag*> ListOfTags;
    QList<Tag*> ListOfVirtualTags;
    int Id;
    int OptimizerRangeInterval;
    int OptimizerRangeMax;
    QString Options;
    QString Comment;
    int DriverId;
    double UpdateFreq;
    //methods
    void Update(bool updateTime = true);
    bool InsertTag(Tag *tag);
private:
    //variables
    Quality quality = Unknown;
    //methods
    void setQuality(Quality quality);
signals:
    void onQualityChanged(); //for external use
    void onWriteRequested(Tag * tag);
    void onUpdated();
    void LoggingSig(MessType MessTypeID,  QDateTime DateTime, bool UserOrSys, QString Source, QString Message);
    void onTagInserted(Group *);
public slots:
    Quality ReadQuality(); //for external use
};

#endif // GROUP_H
