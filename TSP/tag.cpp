#include "tag.h"
//#include <qdebug.h>
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
    config(Config)//TODO засунуть конфиг в тег
{
    setObjectName(Name);
    if (virtualization == VValue || virtualization == VRandom){
        quality = Good;
    }
}

//------------------------------------------------------------------------------
Tag::~Tag()
{
    operator delete( speshData );
}

//------------------------------------------------------------------------------
//TODO добавить массив байтов
QVariant Tag::readValue()
{
    if (access != WO && access != NA){
        switch (type) {
        case TInt:
            return value.toInt();
        case TBool:
            return value.toBool();
       case TFloat:
            return value.toFloat();
        default:
            //emit s_logging(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Tag reading error: data type isn\'t valid");
            return 0;
        }
    } else {
        //emit s_logging(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Tag reading error: access denied");
        return 0;
    }
}
//------------------------------------------------------------------------------
Quality Tag::readQuality()
{
    return quality;
}
//------------------------------------------------------------------------------
QString Tag::readError()
{
    return error;
}
//------------------------------------------------------------------------------
bool Tag::writeValue(QVariant value)
{
    if (access != RO && access != NA){
        this->newValue = value;
        ready = false;
        //emit s_logging(MessVerbose, QDateTime::currentDateTime(), false, this->objectName(), "Tag writing: " + value.toString());
        emit s_onWriteRequested(this, value);
        return true;
    } else {
        emit s_logging(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Tag writing error: access denied");
        return false;
    }
}
//------------------------------------------------------------------------------
void Tag::setValue(QVariant value)
{
    if (this->value != value){
        this->value = value;
        //qDebug()<<this->objectName() + "Tag value s_valueChd: " + value.toString();
        //emit s_logging(MessVerbose, QDateTime::currentDateTime(), false, this->objectName(), "Tag value s_valueChd: " + readValue().toString());
        emit s_onValueChanged(value);
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
        //emit s_logging(MessVerbose, QDateTime::currentDateTime(), false, this->objectName(), "Tag quality s_valueChd: " + Prom::qualityToString(quality));
        emit s_onQualityChanged();
    }
}
//------------------------------------------------------------------------------
void Tag::setError(QString error)
{
    if (this->error != error){
        this->error = error;
        if(error == ""){
            emit s_logging(MessVerbose, QDateTime::currentDateTime(), false, this->objectName(), "Tag error reseted");
        }else{
            emit s_logging(MessError, QDateTime::currentDateTime(), false, this->objectName(), "Tag error: " + error);
        }
        emit s_onErrorChanged(error);
    }
}

//------------------------------------------------------------------------------
Quality Tag::getQuality() const
{
    return quality;
}
