#include "start_measuring_the_thread.h"

#include <vector>
#include <QTimer>
#include <QDebug>
start_measuring_the_thread::start_measuring_the_thread()
{
 qDebug()<<"线程\n";
}

start_measuring_the_thread::~start_measuring_the_thread()
{
    workerthread.quit();
    workerthread.wait();
}


void start_measuring_the_thread::show()
{
  qDebug()<<"线程\n";
}
