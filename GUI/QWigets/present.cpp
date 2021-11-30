#include "present.h"
#include "ui_present.h"

Present::Present(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Present)
{
    ui->setupUi(this);
}

Present::~Present()
{
    delete ui;
}

PresentThread::PresentThread(QWidget *parent):QThread(parent)
{
    prW = new Present();
    prW->setWindowFlags(Qt::WindowType::SplashScreen | Qt::WindowType::WindowStaysOnTopHint);
}

PresentThread::~PresentThread()
{
    prW->hide();
}

void PresentThread::run()
{
    prW->show();
    exec();
}
