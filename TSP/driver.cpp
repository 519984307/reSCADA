#include "driver.h"
//------------------------------------------------------------------------------
Driver::~Driver()
{
    thread->exit();
    foreach (Group * group, listOfGroups) {
        delete group;
    }
    delete thread;
}
//------------------------------------------------------------------------------
bool Driver::InsertGroup(Group *group)
{
    if(group){
        Group * twin = getGroupById(group->Id);
        if(twin){
            emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Driver inserting group error: group id isn\'t unique");
            return false;
        }else{
            group->moveToThread(thread);
            listOfGroups.append(group);
            QObject::connect(group, &Group::onTagInserted, this, &Driver::TagInserted, Qt::DirectConnection);
            QObject::connect(group, &Group::onWriteRequested, this, &Driver::WriteRequested);
            QObject::connect(group, &Group::LoggingSig, this, &Driver::LoggingSig);
            return true;
        }
    }else{
        emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Driver inserting group error: group is null");
        return false;
    }
}
//------------------------------------------------------------------------------
Group *Driver::getGroupById(int Id)
{
    foreach (Group * thisGroup, listOfGroups) {
        if(thisGroup->Id == Id){
            return thisGroup;
        }
    }
    return nullptr;
}
//------------------------------------------------------------------------------
void Driver::qualityFiller(QList<Tag *> listOfTags, Quality quality)
{
    foreach (Tag * tag, listOfTags) {
        if(tag)
            tag->setQuality(quality);
    }
}
//------------------------------------------------------------------------------
void Driver::qualityFiller(QList<Group *> listOfGroups, Quality quality)
{
    foreach (Group * group, listOfGroups) {
        if (group->ReadQuality() == Good || group->ReadQuality() == Check)
            qualityFiller(group->ListOfTags, quality);
    }
}
//------------------------------------------------------------------------------
void Driver::newValueFiller(QList<Tag *> listOfTags)
{
    foreach (Tag * tag, listOfTags) {
        if(tag)
            tag->setValue(tag->newValue);
    }
}
//------------------------------------------------------------------------------
void Driver::readyFiller(QList<Tag *> listOfTags, bool ready)
{
    foreach (Tag * tag, listOfTags) {
        if(tag)
            tag->ready = ready;
    }
}
//------------------------------------------------------------------------------
void Driver::errorFiller(QList<Tag *> listOfTags, QString error)
{
    foreach (Tag * tag, listOfTags) {
        if(tag)
            tag->setError(error);
    }
}
//------------------------------------------------------------------------------
QList<Tag *> Driver::prepareTagsList(QList<Tag *> listOfTags)
{
    QList<Tag *> newTagList = listOfTags;
    int i = 0;
    while (i < newTagList.count()){
        if (newTagList[i]->access != WO && newTagList[i]->access != NA){
            i++;
        }else{
            newTagList.removeAt(i);
        }
    }
    return newTagList;
    //    return listOfTags;
}

//------------------------------------------------------------------------------
void Driver::TagInserted(Group * group)
{
    group->ListOfTags = sortTags(group->ListOfTags);
}
