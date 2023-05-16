#ifndef RESULT_AREA_ATTRIBUTE_H
#define RESULT_AREA_ATTRIBUTE_H

#include <QWidget>

namespace Ui {
class Result_area_attribute;
}

class Result_area_attribute : public QWidget
{
    Q_OBJECT

public:
    explicit Result_area_attribute(QWidget *parent = nullptr);
    ~Result_area_attribute();

private slots:
    void on_cancel_pushButton_clicked();

    void on_confirm_pushButton_clicked();

    void on_use_pushButton_clicked();

signals:
    void signal_tolerance(bool);

    void signal_limit_value(bool);

    void signal_header(bool);

    void signal_control_decimal_place(int);

private:
    Ui::Result_area_attribute *ui;
};

#endif // RESULT_AREA_ATTRIBUTE_H
