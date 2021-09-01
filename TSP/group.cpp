#include <QDateTime>
#include "group.h"
#include "SCADAenums.h"
//------------------------------------------------------------------------------
Group::Group(int id, int driverId, QString name, int delay, int optimizerRangeInterval, int optimizerRangeMax, QString comment)
{
    this->Id = id;
    this->setObjectName(name);
    this->Delay = delay;
    this->LastUpdate = QDateTime::currentDateTime();
    this->OptimizerRangeMax = optimizerRangeMax;
    this->OptimizerRangeInterval = optimizerRangeInterval;
    this->Comment = comment;
    this->DriverId = driverId;
}
//------------------------------------------------------------------------------
Group::~Group()
{
    foreach (Tag * tag, ListOfTags) {
        delete tag;
    }
}
//------------------------------------------------------------------------------
Quality Group::ReadQuality()
{
    return quality;
}
//------------------------------------------------------------------------------
void Group::Update(bool updateTime)
{
    static Quality newQuality;

    UpdateFreq = ListOfTags[0]->updateFreq;
    newQuality = ListOfTags[0]->ReadQuality();

    for(int i = 1; i < ListOfTags.count(); i++) {

        if (newQuality != Check && ListOfTags[i]->ReadQuality() != newQuality){
            newQuality = Check;
        }

        if(ListOfTags[i]->updateFreq < UpdateFreq)
            UpdateFreq = ListOfTags[i]->updateFreq;
    }

    setQuality(newQuality);
    if(updateTime)
        LastUpdate = QDateTime::currentDateTime();
    emit onUpdated();
}
//------------------------------------------------------------------------------
bool Group::InsertTag(Tag *tag)
{
    if (tag){
        tag->moveToThread(this->thread());
        if(tag->virtualization == No){
            ListOfTags.append(tag);
        }else{
            ListOfVirtualTags.append(tag);
        }
        QObject::connect(tag, &Tag::onWriteRequested, this, &Group::onWriteRequested);
        QObject::connect(tag, &Tag::LoggingSig, this, &Group::LoggingSig);
        return true;
    }else{
        emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Group inserting tag error: tag is null");
        return false;
    }
}
//------------------------------------------------------------------------------
void Group::setQuality(Quality quality)
{
    if(this->quality != quality){
        this->quality = quality;
        emit LoggingSig(MessVerbose, QDateTime::currentDateTime(),
                        false, this->objectName(), "Group quality s_valueChd: "
                        + Prom::qualityToString(quality));
        emit onQualityChanged();
    }
}
//------------------------------------------------------------------------------
