#ifndef IDENTIFY_AREA_ATTRIBUTES_H
#define IDENTIFY_AREA_ATTRIBUTES_H

#include <QWidget>

namespace Ui {
class identify_area_attributes;
}

class identify_area_attributes : public QWidget
{
    Q_OBJECT

public:
    explicit identify_area_attributes(QWidget *parent = nullptr);
    ~identify_area_attributes();

private slots:
    void on_ok_pushButton_clicked();

    void on_cancel_pushButton_clicked();

signals:
    void signal_type_of_part(bool);

    void signal_attribute(bool);

    void signal_amount(bool);

    void signal_order(bool);

    void signal_client(bool);

    void signal_channel(bool);

    void signal_operator(bool);

    void signal_tor_number(bool);

    void signal_factory(bool);

    void signal_branch(bool);

    void signal_machine(bool);

    void signal_operate(bool);



private:
    Ui::identify_area_attributes *ui;
};

#endif // IDENTIFY_AREA_ATTRIBUTES_H
