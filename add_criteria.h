#ifndef ADD_CRITERIA_H
#define ADD_CRITERIA_H

#include <QWidget>
#include <QListWidgetItem>
#include <QTableWidgetItem>
#include "profile.h"

namespace Ui {
class add_criteria;
}

class add_criteria : public QWidget
{
    Q_OBJECT

public:
    explicit add_criteria(QWidget *parent = nullptr);
    ~add_criteria();

private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_listWidget_2_itemClicked(QListWidgetItem *item);

    void on_pushButton_2_clicked();

    void on_confirm_pushButton_clicked();


signals:
    void signal_table_widget(QString str);

    void signal_filter_band(QVector<table_widget_band> );

private:
    Ui::add_criteria *ui;
    QString str_main_criteria_;
    QString str_secondary_criteria_;

public:
     QString criteria_for_selection_;
     QVector<table_widget_band> band_;
};

#endif // ADD_CRITERIA_H
