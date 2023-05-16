#ifndef CALIBRATION_PASSWORD_H
#define CALIBRATION_PASSWORD_H

#include <QWidget>

namespace Ui {
class calibration_password;
}

class calibration_password : public QWidget
{
    Q_OBJECT

public:
    explicit calibration_password(QWidget *parent = nullptr);
    ~calibration_password();

private slots:

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

signals:
    void signal_password();

private:
    Ui::calibration_password *ui;
};

#endif // CALIBRATION_PASSWORD_H
