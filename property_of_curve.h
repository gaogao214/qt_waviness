#ifndef PROPERTY_OF_CURVE_H
#define PROPERTY_OF_CURVE_H

#include <QWidget>

namespace Ui {
class property_of_curve;
}

class property_of_curve : public QWidget
{
    Q_OBJECT

public:
    explicit property_of_curve(QWidget *parent = nullptr);
    ~property_of_curve();

signals:
    void display_Remarks(bool);

    void display_Time(bool);


private slots:


    void on_pushButton_clicked();

private:
    Ui::property_of_curve *ui;
};

#endif // PROPERTY_OF_CURVE_H
