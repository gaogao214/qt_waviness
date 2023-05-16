#ifndef START_MEASURING_THE_THREAD_H
#define START_MEASURING_THE_THREAD_H

#include <memory>
#include <QObject>
#include <QTimer>
#include <QThread>

class start_measuring_the_thread:public QObject
{
    Q_OBJECT
public:
    start_measuring_the_thread();
    ~start_measuring_the_thread();

    void show();

public:
    QThread workerthread;

};

#endif // START_MEASURING_THE_THREAD_H
