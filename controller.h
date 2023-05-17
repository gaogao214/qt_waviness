#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "get_msxe3711_data.h"

class controller: public QObject
{
    Q_OBJECT

public:
    controller(QObject* parent = nullptr);
    ~controller();

public slots:
    void handleResults(const QString &des);

    void getMSXe3711Data(QVector<double> data);

signals:
    void operate(const QString &cmd);

    void operate_msx_data();

    void signal_msx_data(QVector<double> data);
private:
    std::unique_ptr<QThread> thread_;

    std::unique_ptr<get_msxe3711_data> get_msxe3711_data_=nullptr;

};

#endif // CONTROLLER_H
