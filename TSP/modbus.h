#ifndef MODBUSDRIVER_H
#define MODBUSDRIVER_H

#include <QObject>
#include <QSerialPort>
#include <QModbusDataUnit>
#include <QModbusRtuSerialMaster>
#include <QModbusTcpClient>
#include <QModbusDataUnit>
#include <QTimer>
#include "driver.h"

class Modbusdriver : public Driver
{
public:
    //constructor
    Modbusdriver(int Id, QString name, QString address, int port = 502, int timeout = 30, QString comment = "");
    Modbusdriver(int Id, QString name, QString port, QString baudrate, QSerialPort::DataBits databits = QSerialPort::Data8,
                 QSerialPort::Parity parity = QSerialPort::EvenParity, QSerialPort::StopBits stopbits = QSerialPort::OneStop, int timeout = 30, QString comment = "");
    //destructor
    ~Modbusdriver();
    //structs
    struct MBaddress{
        int devAddr, regAddr;
        QModbusDataUnit::RegisterType regType;
    };
    //methods
    void connect() override;
    void disconnect() override;
private:
    //structs
    struct Task : public MBaddress{
        int quantity = 0, groupId;
        QList<Tag*> listOfTags;
        bool writeTask = false;
    };
    //variables
    QList<Task*> listOfTasks;
    QModbusClient * modbusDevice = nullptr;
    //methods
    void initThread(QModbusClient * modbusDevice);
    void handleNextTask() override;
    void getTask();
    void valueFiller(QList<Tag*> listOfTags, QModbusDataUnit unit);
    void read(Task * task, const std::function<void()> doNext);
    void write(Task * task, const std::function<void()> doNext);
    bool strToAddr(QString str, MBaddress * adress);
    void scheduleHandler();
    QList<Tag*> sortTags(QList<Tag*> listOfTags) override;
    inline bool compare(MBaddress a1, MBaddress a2);
public slots:
    void WriteRequested(Tag * tag) override;
};

#endif // MODBUSDRIVER_H
