#ifndef TAG_H
#define TAG_H

#include <QObject>
#include <QVariant>
#include <QDateTime>
#include "enums.h"
#include <memory>
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
    ~Tag();
    //variables
    int id;
    QVariant  getValue() const {return value;};
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
    Quality getQuality() const;
    //Сюда цепляются структуры с данными
    //TODO Убрать этот костыль
    /*std::unique_ptr<void>*/ void *speshData{nullptr};


private:
    //variables
    Quality quality = Unknown;
    QVariant value;
    QVariant updateValue;
    QString error = "";
    QDateTime LastUpdate = QDateTime::currentDateTime();
signals:
    void s_onValueChanged(QVariant); //for external use
    void s_onQualityChanged(); //for external use
    void s_onErrorChanged(QString error); //for external use
    void s_onWriteRequested(Tag * tag, QVariant NewValue = 0);
    void s_logging(MessType MessTypeID,  QDateTime DateTime, bool UserOrSys, QString Source, QString Message);
public slots:
    QVariant readValue(); //for external use
    Quality readQuality(); //for external use
    QString readError(); //for external use
    bool writeValue(QVariant value); //for external use
};

#endif // TAG_H
