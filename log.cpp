#include "log.h"
#include "promobject.h"
#include <QMessageBox>
#include <QFile>
#include <QMessageBox>

Log::Log(QObject * parant) : QObject(parant)
{
    //_logFile = new QFile("./LOG/" + QDate::currentDate().toString("yyyy.MM") + "_LOG.csv", this);
//    qRegisterMetaType<Prom::MessType>();
//    qRegisterMetaType<Prom::UnitModes>();
    _sdb = QSqlDatabase::addDatabase("QSQLITE");

    //    _sdb.setDatabaseName("./LOG/log.sqlite");

    //    if(_sdb.isOpen()){
    _query = QSqlQuery(_sdb);
    //        Logging(Prom::MessChangeInfo, QDateTime::currentDateTime(), "SYSTEM","LogSYS", "база данных журнала открыта");
    //    }
}
//------------------------------------------------------------------------------
Log::~Log()
{
    if(timer) {
        delete timer;
    }
}
//------------------------------------------------------------------------------
void Log::Logging(Prom::MessType MessTypeID, QDateTime DateTime, QString User, QString Source, QString Message)
{
    //qDebug()<< "Logging " << Prom::messToString(MessTypeID) << (User) << Source << Message;
    if(_activeBuff)
        _buffer_1.push_back({ MessTypeID, DateTime, User, Source, Message });
    else
        _buffer_0.push_back({ MessTypeID, DateTime, User, Source, Message });
    if(timer){
        if(! timer->isActive()){
            timer->start(200);
        }
    }
    else{
        bufferToDB();
    }
    //if(fin == 1) fin = 2;
}

//------------------------------------------------------------------------------
void Log::bufferToDB()
{
    static bool buffOverload = false;
    static std::vector<message> * tmpBuff = nullptr;
    //delete tmpBuff;
    if(_activeBuff) {
        _activeBuff = 0;
        tmpBuff = &_buffer_1;
    }
    else {
        _activeBuff = 1;
        tmpBuff = &_buffer_0;
    }
    if(! tmpBuff) return;
    if(_sdb.databaseName() != "./LOG/" + QDate::currentDate().toString("yyyy.MM") + "_log.sqlite"
            || ! QFile(_sdb.databaseName()).exists())
        _sdb.close();

    if(!_sdb.isOpen()){

        if(!QFile("./LOG/" + QDate::currentDate().toString("yyyy.MM") + "_log.sqlite").exists()) {

            QSqlQuery query(_sdb);

            if (QFile("db.sql").exists()) {
                QFile * file = new QFile("db.sql");
                QString tmp;
                _sdb.setDatabaseName("./LOG/" + QDate::currentDate().toString("yyyy.MM") + "_log.sqlite");
                _sdb.open();
                file->open(QIODevice::ReadOnly);
                while (! file->atEnd()) {
                    tmp = file->readLine();
                    if(! tmp.startsWith("--") && ! tmp.contains("TRANSACTION", Qt::CaseInsensitive) && tmp.length() > 3){
                        //qDebug()<<tmp;
                        if(! query.exec(tmp)){
                            _sdb.close();
                            //QMessageBox::warning(nullptr, "Oшибка!", "Ошибка sql запроса при создании базы данных журнала событий.", QMessageBox::Ok);
                            Logging(Prom::MessAlarm, QDateTime::currentDateTime(), "SYSTEM","LogSYS", "ошибка sql запроса при создании базы данных журнала событий");
                            timer->stop();
                            QFile::remove("./LOG/" + QDate::currentDate().toString("yyyy.MM") + "_log.sqlite");
                            break;
                        }
                    }
                }
                file->close();
                delete file;
            }
            else {
                //QMessageBox::warning (nullptr, "Критическая ошибка!", "Отсутствует база данных журнала событий и создать её не возможно,\r\n"
                //                                                       "т.к. файл db.sql не найден.", QMessageBox::Ok);
                Logging(Prom::MessAlarm, QDateTime::currentDateTime(), "SYSTEM","LogSYS", "отсутствует база данных журнала событий и создать её не возможно");
                timer->stop();
                QFile::remove("./LOG/" + QDate::currentDate().toString("yyyy.MM") + "_log.sqlite");
            }

            _sdb.close();
        }

        if(QFile("./LOG/" + QDate::currentDate().toString("yyyy.MM") + "_log.sqlite").exists())
        {
            _sdb.setDatabaseName("./LOG/" + QDate::currentDate().toString("yyyy.MM") + "_log.sqlite");
            _sdb.open();
        }
    }
    //qDebug()<< "Start WriteValue " << tmpBuff->size() << QTime::currentTime().toString("mm:ss.zz");

    _logFile = new QFile("./LOG/" + QDate::currentDate().toString("yyyy.MM") + "_LOG.csv", this);
    _logFile->open(QIODevice::Append);

    if(_logFile->isOpen()){

        for (message tmpMess : *tmpBuff) {
            QString tmp = tmpMess.m_dateTime.toString("yyyy.MM.dd hh:mm:ss.z") + ";"
                    + Prom::messToString(tmpMess.m_type) + ";"
                    + tmpMess.m_user + ";"
                    + tmpMess.m_source + ";"
                    + tmpMess.m_text + '\n';
            _logFile->write(tmp.toStdString().c_str());

        }
        _logFile->close();
    }
    if(_logFile != nullptr)delete _logFile;


    if(_sdb.isOpen()){
        _sdb.transaction();
        _query.prepare("INSERT INTO SEL_ALL (TYPE, TIME, USER, UNIT, MESSAGE) "
                        "VALUES (:type, :time, :user, :unit, :mess)");
        //int co = 1;
        if(tmpBuff->size() > 20000){
            buffOverload = true;      
            _query.bindValue(":time",  QDateTime::currentDateTime().toMSecsSinceEpoch());
            _query.bindValue(":type",  Prom::messToString(Prom::MessAlarm));
            _query.bindValue(":user",  "SYSTEM");
            _query.bindValue(":unit",  "LOG Process");
            _query.bindValue(":mess",  "Переполнение буфера. Из " + QString::number(tmpBuff->size())
                              + " записей в БД только важные. Остальное в файле: "
                              + QDate::currentDate().toString("yyyy.MM") + "_LOG.csv.");
            _query.exec();
            buffOverload = true;
        }
        for (message tmpMess : *tmpBuff) {
            if(!(buffOverload && tmpMess.m_type < 0)) {
                //qDebug()<< "Start WriteValue " << co << "/" << tmpBuff->size() << QTime::currentTime().toString("mm:ss.zz");             
                _query.bindValue(":time",  tmpMess.m_dateTime.toMSecsSinceEpoch());
                _query.bindValue(":type",  Prom::messToString(tmpMess.m_type));
                _query.bindValue(":user",  tmpMess.m_user);
                _query.bindValue(":unit",  tmpMess.m_source);
                _query.bindValue(":mess",  tmpMess.m_text);
                _query.exec();
                //co++;
            }
        }
        _sdb.commit();
        //_sdb.close();

    }
    tmpBuff->clear();
    buffOverload = false;
//    if(fin){
//        _sdb.close();
//        emit done();
//    }
}

//------------------------------------------------------------------------------
void Log::finish()
{
//    fin = true;
    timer->stop();
    bufferToDB();
}

//------------------------------------------------------------------------------
LogThread::LogThread(QObject *parant) : QThread(parant)
{
    LogMaster = new Log();
    LogMaster->moveToThread(this);
}

//------------------------------------------------------------------------------
LogThread::~LogThread()
{

}

//------------------------------------------------------------------------------
void LogThread::run()
{
    LogMaster->timer = new QTimer();
    LogMaster->timer->setSingleShot(true);
    LogMaster->timer->setInterval(50);
    connect(LogMaster->timer, &QTimer::timeout, LogMaster, &Log::bufferToDB, Qt::QueuedConnection);
    //connect(this, &LogThread::fin, LogMaster, &Log::finish, Qt::QueuedConnection);
    //connect(LogMaster, &Log::done, this, &LogThread::quit, Qt::DirectConnection);
    exec();
    LogMaster->finish();
    delete LogMaster;

}

//------------------------------------------------------------------------------


























