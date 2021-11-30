#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QtSql>

#ifndef SCADAENUMS_H
#include "../../SCADAenums.h"
#endif // SCADAENUMS_H

class PromObject;
struct message {
    Prom::MessType m_type;
    QDateTime m_dateTime;
    QString m_user;
    QString m_source;
    QString m_text;
};
//wew

class Log : public QObject
{
    Q_OBJECT
public:
    explicit Log(QObject * parant = nullptr);
    ~Log();
signals:
    void done();

public slots:
    void Logging(Prom::MessType MessTypeID, QDateTime DateTime, QString User, QString Source, QString Message);
    void bufferToDB();
    void finish();

protected:
    QTimer * timer = nullptr;
    bool fin = false;
    QSqlDatabase _sdb;

    QSqlQuery _query;
    QFile * _logFile;
    bool _prd =false;
    int _activeBuff = 0;
    std::vector <message> _buffer_0;
    std::vector <message> _buffer_1;

    friend class LogThread;

};

//-------------------------------
class LogThread : public QThread
{
    Q_OBJECT
public:
    explicit LogThread(QObject *parant = nullptr);
    ~LogThread() override;
    Log * LogMaster;
signals:
    void fin();

public slots:
    //void finish();

protected:
    void run() override;
};


#endif // LOG_H
