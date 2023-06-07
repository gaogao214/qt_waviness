#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QWidget>
#include "formdoc.h"

namespace Ui {
class configuration;
}

class configuration : public QWidget
{
    Q_OBJECT

public:
    explicit configuration(QWidget *parent = nullptr);
    ~configuration();


public slots:
    void open_mwa_profile(QString filename);

private slots:
    void on_tabWidget_2_tabCloseRequested(int index);

    void on_copy_pushbutton_clicked();

    void on_paste_pushbutton_clicked();

    void on_add_pushButton_clicked();

    void on_clear_pushButton_clicked();

    void on_cancel_pushButton_clicked();

    void on_tabWidget_2_tabBarClicked(int index);

    void on_tabWidget_2_tabBarDoubleClicked(int index);

    void on_confirm_pushButton_clicked();

    void recive_filter_band_formoc(QVector<table_widget_band> band);

    void on_file_pushButton_clicked();



signals:
    void signalResultAreaArribute(QVector<table_config>);
    void signal_recognition_config(recognition_config config);

private:
    Ui::configuration *ui;

    std::shared_ptr<formdoc> ptr_=nullptr;

    QVector<formdoc*> formdoc_vector_;

    std::shared_ptr<formdoc> ptr_copy_=nullptr;
    std::shared_ptr<formdoc> formtable=nullptr;
public:
    QString configuration_filename_mwa_;

    QVector<table_config> tab_str;
    QVector<table_widget_band> tab_band_;



};

#endif // CONFIGURATION_H
