#ifndef TAG_H
#define TAG_H

#include <QObject>
#include <QVariant>
#include <QDateTime>
#include "enums.h"
using namespace tsp_enums;
class Tag : public QObject
{
    Q_OBJECT
public:
    //constructor
    Tag(int Id,
        QString Name,
        QString Address,
        int GroupId,
        DataType Type = TInt,
        Access Access = RW,
        Virtualization Virtualization = No,
        QString Comment = "",
        TagConfig * Config = nullptr);
    //destructor
    //~Tag();
    //variables
    int id;
    QString address;
    int virtualRand;
    int virtualBit;
    int groupId;
    bool ready = true;
    QList<Tag*> VirtualTags;
    Access access;
    Virtualization virtualization;
    DataType type = TInt;
    QString options;
    QVariant newValue;
    QString comment;
    double updateFreq;
    //methods
    void setValue(QVariant value);
    void setQuality(Quality quality);
    void setError(QString error);
    bool reqestUpdate();
    TagConfig * config{ nullptr };

private:
    //variables
    Quality quality = Unknown;
    QVariant value;
    QVariant updateValue;
    QString error = "";
    QDateTime LastUpdate = QDateTime::currentDateTime();
signals:
    void onValueChanged(QVariant); //for external use
    void onQualityChanged(); //for external use
    void onErrorChanged(QString error); //for external use
    void onWriteRequested(Tag * tag);
    void LoggingSig(MessType MessTypeID,  QDateTime DateTime, bool UserOrSys, QString Source, QString Message);
public slots:
    QVariant ReadValue(); //for external use
    Quality ReadQuality(); //for external use
    QString ReadError(); //for external use
    bool WriteValue(QVariant value); //for external use
};

#endif // TAG_H
