#ifndef CHART_H
#define CHART_H

#include <QWidget>

namespace Ui {
class chart;
}

class chart : public QWidget
{
    Q_OBJECT

public:
    explicit chart(QWidget *parent = nullptr);
    ~chart();

public:
    void show_spinbox_data();

private slots:
    void on_comboBox_2_currentIndexChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_comboBox_currentTextChanged(const QString &arg1);

signals:
    void signal_show_type_of_spectrum(QString type);

    void signal_show_x(QString,int data);

    void signal_show_y(QString,QString data);

private:
    Ui::chart *ui;

    QString spectral_type_;

public:
    int spinbox_data_=2048;


};

#endif // CHART_H
