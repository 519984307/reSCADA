#include "tag.h"
//------------------------------------------------------------------------------
Tag::Tag(int Id,
         QString Name,
         QString Address,
         int GroupId,
         DataType Type,
         Access Access,
         Virtualization Virtualization,
         QString Comment,
         TagConfig * Config):
    id(Id),
    address(Address),
    groupId(GroupId),
    access(Access),
    virtualization(Virtualization),
    type(Type),
    comment(Comment),
    config(Config)
{
    setObjectName(Name);
    if (virtualization == VValue || virtualization == VRandom){
        quality = Good;
    }
}
//------------------------------------------------------------------------------
//Tag::~Tag()
//{

//}
//------------------------------------------------------------------------------
QVariant Tag::ReadValue()
{
    if (access != WO && access != NA){
        switch (type) {
        case TInt:
            return value.toInt();
        case TBool:
            return value.toBool();
        default:
            emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Tag reading error: data type isn\'t valid");
            return 0;
        }
    } else {
        emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Tag reading error: access denied");
        return 0;
    }
}
//------------------------------------------------------------------------------
Quality Tag::ReadQuality()
{
    return quality;
}
//------------------------------------------------------------------------------
QString Tag::ReadError()
{
    return error;
}
//------------------------------------------------------------------------------
bool Tag::WriteValue(QVariant value)
{
    if (access != RO && access != NA){
        this->newValue = value;
        ready = false;
        emit LoggingSig(MessVerbose, QDateTime::currentDateTime(), false, this->objectName(), "Tag writing: " + value.toString());
        emit onWriteRequested(this);
        return true;
    } else {
        emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Tag writing error: access denied");
        return false;
    }
}
//------------------------------------------------------------------------------
void Tag::setValue(QVariant value)
{
    if (this->value != value){
        this->value = value;
        emit LoggingSig(MessVerbose, QDateTime::currentDateTime(), false, this->objectName(), "Tag value s_valueChd: " + ReadValue().toString());
        emit onValueChanged(value);
    }
    if (ready) newValue = value;
}
//------------------------------------------------------------------------------
void Tag::setQuality(Quality quality)
{
    QDateTime currentDT = QDateTime::currentDateTime();
    double interval = LastUpdate.msecsTo(currentDT);
    updateFreq = interval <= 0 ? -1 : (1000 / interval);
    LastUpdate = currentDT;

    if (this->quality != quality){
        this->quality = quality;
        emit LoggingSig(MessVerbose, QDateTime::currentDateTime(), false, this->objectName(), "Tag quality s_valueChd: " + Prom::qualityToString(quality));
        emit onQualityChanged();
    }
}
//------------------------------------------------------------------------------
void Tag::setError(QString error)
{
    if (this->error != error){
        this->error = error;
        if(error == ""){
            emit LoggingSig(MessVerbose, QDateTime::currentDateTime(), false, this->objectName(), "Tag error reseted");
        }else{
            emit LoggingSig(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Tag error: " + error);
        }
        emit onErrorChanged(error);
    }
}
