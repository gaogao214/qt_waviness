#ifndef FUNCTION_KEY_H
#define FUNCTION_KEY_H

#include <QWidget>

namespace Ui {
class function_key;
}

class function_key : public QWidget
{
    Q_OBJECT

public:
    explicit function_key(QWidget *parent = nullptr);
    ~function_key();

private slots:
    void on_connect_pushButton_clicked();

    void on_config_pushButton_clicked();

    void on_pole_diagram_pushButton_clicked();

    void on_graph_pushButton_clicked();

    void on_configuration_Button_clicked();

    void on_pushButton_4_clicked();

    void on_statistical_report_clicked();

signals:
    void signal_F9();

    void signal_config_F8();

    void signal_pole_diagram_F5();

    void signal_graph_F6();

    void signal_F3();

    void signal_F4();

     void signal_F10();

private:
    Ui::function_key *ui;
};

#endif // FUNCTION_KEY_H
