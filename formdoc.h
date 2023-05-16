#ifndef FORMDOC_H
#define FORMDOC_H

#include <QWidget>
#include "add_criteria.h"
#include <memory>
//#include "fft.h"

#include "profile.h"
namespace Ui {
class formdoc;
}


class formdoc : public QWidget
{
    Q_OBJECT

public:
    explicit formdoc(QWidget *parent = nullptr);
    ~formdoc();

public:
    QVector<table_config> getTableWidgetString();

    static bool compare(const table_config &s1, const table_config &s2);

    bool compareBarData(const table_config &barAmount1, const table_config &barAmount2)
    {

//            QString strDiameter1 = barAmount1.main_criteria;
//            QString strDiameter2 = barAmount2.main_criteria;
//            int nCompare = compareDiameterDescription(strDiameter1,strDiameter2);
//            if (nCompare == -1)
//            {
//                return true;
//            }
//            else if (nCompare == 1)
//            {
//                return false;
//            }
//            else
//            {
//                return true;
//            }

    }

    void write_tablewidget(QVector<table_config> );

private slots:
    void table_widget(QString str);

    void on_add_pushbutton_clicked();

    void on_tableWidget_2_cellClicked(int row, int column);

    void on_clear_row_pushButton_clicked();

    void on_change_pushButton_clicked();

    void recive_filter_band(QVector<table_widget_band>);

signals:
    void signal_filter_formdoc_to_configuration(QVector<table_widget_band>);

private:
    Ui::formdoc *ui;

   std::unique_ptr<add_criteria> add_criteria_=nullptr;
//   std::unique_ptr<fft> fft_amplitude_=nullptr;
   QVector<QString> table_string;


   int row_=0;
   int column_=0;

public:
   QVector<table_widget_band> add_criteria_data_;

};

#endif // FORMDOC_H
