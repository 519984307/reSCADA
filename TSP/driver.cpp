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
bool Driver::insertGroup(Group *group)
{
    if(group){
        Group * twin = getGroupById(group->id);
        if(twin){
            emit s_logging(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Driver inserting group error: group id isn\'t unique");
            return false;
        }else{
            group->moveToThread(thread);
            listOfGroups.append(group);
//            QObject::connect(group, &Group::s_onTagInserted, this, &Driver::TagInserted, Qt::DirectConnection);
            QObject::connect(group, &Group::s_onWriteRequested, this, &Driver::createWriteTask);
            QObject::connect(group, &Group::s_logging, this, &Driver::s_logging);
            return true;
        }
    }else{
        emit s_logging(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Driver inserting group error: group is null");
        return false;
    }
}
//------------------------------------------------------------------------------
Group *Driver::getGroupById(int Id)
{
    foreach (Group * thisGroup, listOfGroups) {
        if(thisGroup->id == Id){
            return thisGroup;
        }
    }
    return nullptr;
}

//------------------------------------------------------------------------------
const QList<Group *> &Driver::getListOfGroups() const
{
  return listOfGroups;
}

//------------------------------------------------------------------------------
void Driver::setListOfGroups(const QList<Group *> &newListOfGroups)
{
  listOfGroups = newListOfGroups;
  foreach (Group *gr, listOfGroups) {
    gr->listOfTags.sort = sortTags;
  }
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
        if (group->readQuality() == Good || group->readQuality() == Check)
            qualityFiller(group->listOfTags, quality);
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
//QList<Tag *> Driver::prepareTagsList(QList<Tag *> listOfTags)
//{
//    QList<Tag *> newTagList = listOfTags;
//    int i = 0;
//    while (i < newTagList.count()){
//        if (newTagList[i]->access != WO && newTagList[i]->access != NA){
//            i++;
//        }else{
//            newTagList.removeAt(i);
//        }
//    }
//    return newTagList;
//    //    return listOfTags;
//}

//------------------------------------------------------------------------------
//void Driver::TagInserted(Group * group)
//{
//    group->listOfTags = sortTags(group->listOfTags);
//}
