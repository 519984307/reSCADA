#ifndef SIMATICDRIVER_H
#define SIMATICDRIVER_H

#include <QTimer>
#include "driver.h"
#include "snap7/snap7.h"

class Simaticdriver : public Driver
{
public:
    //constructor
    Simaticdriver(int id, QString name, QString address = "127.0.0.1", int rack = 0, int slot = 2, QString comment = "");
    //destructor
    ~Simaticdriver();
    //enums
    enum Area{
        S7AreaPE = 0x81,
        S7AreaPA = 0x82,
        S7AreaMK = 0x83,
        S7AreaDB = 0x84,
        S7AreaCT = 0x1C,
        S7AreaTM = 0x1D
    };
    enum WordLenght{
        S7WLBit     = 0x01,
        S7WLByte    = 0x02,
        S7WLWord    = 0x04,
        S7WLDWord   = 0x06,
        S7WLReal    = 0x08,
        S7WLCounter = 0x1C,
        S7WLTimer   = 0x1D,
    };
    //structs
    struct SimAddress{
        Area area;
        WordLenght type;
        int regAddr;
    };
    //methods
    void Connect() override;
    void Disconnect() override;
private:
    //structs
    struct Task : public SimAddress{
        int quantity = 0, groupId;
        QList<Tag*> listOfTags;
        bool writeTask = false;
    };
    //variables
    QList<Task*> listOfTasks;
    TS7Client * client = nullptr;
    QString address;
    int rack;
    int slot;
    //methods
    void initThread();
    void handleNextTask() override;
    void getTask();
    template <typename Tarr>
    void valueFiller(QList<Tag*> listOfTags, Tarr data[]);
    void valueFiller(Task * task, uint8_t data[]);
    void read(Task * task, const std::function<void()> doNext);
    void write(Task * task, const std::function<void()> doNext);
    bool check(int res, QString function = "unknown function");
    void scheduleHandler();
    QList<Tag*> sortTags(QList<Tag*> listOfTags) override;
    bool strToAddr(QString str, SimAddress * address);
    inline bool compare(SimAddress a1, SimAddress a2);
public slots:
    void WriteRequested(Tag * tag) override;
};
#endif // SIMATICDRIVER_H
