#ifndef PRESENT_H
#define PRESENT_H

#include <QWidget>
#include <QThread>

namespace Ui {
class Present;
}

class Present : public QWidget
{
    Q_OBJECT


public:
    explicit Present(QWidget *parent = nullptr);
    ~Present();

private:
    Ui::Present *ui;
};

class PresentThread : public QThread
{
    Q_OBJECT

public:
    explicit PresentThread (QWidget *parent = nullptr);
    ~PresentThread() override;
    void run() override;
    Present * prW;
};

#endif // PRESENT_H
