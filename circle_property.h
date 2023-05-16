#ifndef CIRCLE_PROPERTY__H
#define CIRCLE_PROPERTY__H

#include <QWidget>

namespace Ui {
class circle_property_;
}

class circle_property_ : public QWidget
{
    Q_OBJECT

public:
    explicit circle_property_(QWidget *parent = nullptr);
    ~circle_property_();

private slots:
    void on_pushButton_2_clicked();

signals:
    void display_annotation(bool);

    void display_time(bool);

private:
    Ui::circle_property_ *ui;
};

#endif // CIRCLE_PROPERTY__H
