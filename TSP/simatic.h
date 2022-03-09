#ifndef SIMATICDRIVER_H
#define SIMATICDRIVER_H

#include <QTimer>
#include "driver.h"
#include "snap7.h"

struct SimAddress{
  Area memArea;
  WordLenght type;
  int DBNumb{0};
  struct {
    int memSlot{-1};//Это может быть байт, слово, двойное слово и т.д.
    int bit{-1};//Номер бита для булевых тэгов, у остальных всегда 0
  }regAddr;
  SimAddress &operator= (const SimAddress& SA);
  bool operator> (SimAddress const& SA);
  bool operator< ( SimAddress const& SA){ return !(*this > SA); };
};
//class SimTag : public Tag, SimAddress{};

class SimaticDriver : public Driver
{
public:
  //constructor
  SimaticDriver(int id, QString name, QString address = "127.0.0.1", int rack = 0, int slot = 2, QString comment = "");
  //destructor
  ~SimaticDriver();
  //enums

  //methods
  void connect() override;
  void disconnect() override;
  static void sortTags(QList<Tag*> &listOfTags);
  static bool strToAddr(QString str, SimAddress * address);
  //static inline bool compare(SimAddress *a1, SimAddress *a2);
  bool insertGroup(Group  * group) override;
  int PDULen {-1};
  struct Task : public SimAddress, CommonTask{
    int endAdr{-1};
    //int buffByteCount{0};
  };
private:
  //structs

  //variables
  QList<Task*> listOfTasks;
  TS7Client * client = nullptr;
  QString address;
  int rack;
  int slot;
  //methods
  void initThread();
  void handleNextTask() override;
  inline void createReadTasks();
  void read(Task * task);
  int readInList(int fstTaskInd, QList<Task*> *taskList = nullptr ); //Читает данные с ПЛК начиная с заданной задачи и далее по списку ListOfTasks сколько влезет в PDU Lengh. Возвращает индекс последней прочитанной задачи
  void write(Task * task);
  //bool check(int res, QString function = "unknown function");
  void scheduleHandler(); //TODO вынести в класс драйвера

public slots:
  void createWriteTask(Tag * tag) override;
};
#endif // SIMATICDRIVER_H
