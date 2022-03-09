#include <QDateTime>
#include "group.h"
#include "SCADAenums.h"
//------------------------------------------------------------------------------
Group::Group(int id,
    int driverId,
    QString name,
    int delay,
    int optimizerRangeInterval,
    int optimizerRangeMax,
    QString comment)
{
    this->id = id;
    this->setObjectName(name);
    this->delay = delay;
    this->lastUpdate = QDateTime::currentDateTime();
    this->optimRangeMax = optimizerRangeMax;
    this->optimRangeInterval = optimizerRangeInterval;
    this->comment = comment;
    this->driverId = driverId;
}
//------------------------------------------------------------------------------
Group::~Group()
{
    foreach (Tag * tag, listOfTags) {
        delete tag;
    }
}
//------------------------------------------------------------------------------
Quality Group::readQuality()
{
    return quality;
}
//------------------------------------------------------------------------------
void Group::update(bool updateTime)
{
    static Quality newQuality;
    if(listOfTags.count()){
        updateFreq = listOfTags[0]->updateFreq;//TODO Дич. Переписать.
        newQuality = listOfTags[0]->readQuality();
        static bool freqUpd;
        freqUpd = false;
        for(int i = 1; i < listOfTags.count(); i++) {

            if (newQuality != Check && listOfTags[i]->readQuality() != newQuality){
                newQuality = Check;
            }

            if(listOfTags[i]->updateFreq > updateFreq){
                updateFreq = listOfTags[i]->updateFreq;
                freqUpd = false;
            }
        }

        setQuality(newQuality);
        if(freqUpd)emit s_onUpdated();
    }
    if(updateTime)
        lastUpdate = QDateTime::currentDateTime();

}
//------------------------------------------------------------------------------
bool Group::insertTag(Tag *tag)
{
    if (tag){
        tag->moveToThread(this->thread());
        if(tag->virtualization == No){
            listOfTags.append(tag);
        }else{
            listOfVirtualTags.append(tag);
        }
        QObject::connect(tag, &Tag::s_onWriteRequested, this, &Group::s_onWriteRequested);
        QObject::connect(tag, &Tag::s_logging, this, &Group::s_logging);
        return true;
    }else{
        emit s_logging(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Group inserting tag error: tag is null");
        return false;
    }
}
//------------------------------------------------------------------------------
void Group::setQuality(Quality quality)
{
    if(this->quality != quality){
        this->quality = quality;
        emit s_logging(MessVerbose, QDateTime::currentDateTime(),
            false, this->objectName(), "Group quality s_valueChd: "
                + Prom::qualityToString(quality));
        emit s_onQualityChanged();
    }
}
//------------------------------------------------------------------------------
