#include "add_criteria.h"
#include "ui_add_criteria.h"
#include <QDebug>

add_criteria::add_criteria(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add_criteria)
{
    ui->setupUi(this);

    ui->groupBox_3->hide();
    ui->groupBox_4->hide();

}

add_criteria::~add_criteria()
{
    delete ui;
}

void add_criteria::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QStringList str_list;

    str_main_criteria_=item->text();

    if(item->text()=="FFT")
    {
        ui->groupBox_3->hide();
        ui->groupBox_4->hide();
        ui->listWidget_2->clear();
        str_list<< "Harm"<<"max1"<< "max2"<<"max3";
        ui->listWidget_2->addItems(str_list);
    }
    else if(item->text()=="LSC")
    {
        ui->groupBox_3->hide();
        ui->groupBox_4->hide();
        ui->listWidget_2->clear();
        str_list<< "△r"<<"fln1"<< "VDp"<<"Vd60"<<"Vd72"<<"Vd90"<<"Vd108"<<"Vd120"<<"VDw"<<"V3D60"<<"V3D90"<<"V3D120";
        ui->listWidget_2->addItems(str_list);
    }
    else if(item->text()=="MIC")
    {
        ui->groupBox_3->hide();
        ui->groupBox_4->hide();
        ui->listWidget_2->clear();
        str_list<< "△r";
        ui->listWidget_2->addItems(str_list);
    }
    else if(item->text()=="MCC")
    {
        ui->groupBox_3->hide();
        ui->groupBox_4->hide();
        ui->listWidget_2->clear();
        str_list<< "△r";
        ui->listWidget_2->addItems(str_list);
    }
    else if(item->text()=="MZC")
    {
        ui->groupBox_3->hide();
        ui->groupBox_4->hide();
        ui->listWidget_2->clear();
        str_list<< "△r";
        ui->listWidget_2->addItems(str_list);
    }
    else if(item->text()=="FLT")
    {
        ui->groupBox_3->show();
        ui->groupBox_4->hide();
        ui->listWidget_2->clear();
        str_list.clear();
        str_list<< "△r"<<"max"<< "Rms"<<"wRms"<<"wMax";
        ui->listWidget_2->addItems(str_list);
    }
    else if(item->text()=="MDi")
    {
        ui->groupBox_3->hide();
        ui->groupBox_4->hide();
        ui->listWidget_2->clear();
        str_list.clear();
        str_list<< "L"<<"L1"<< "L2"<<"L3"<<"M"<<"H"<<"max1"<<"max2"<<"max3"<<"W2"<<"W3"<<"W4"<<"W5"<<"W6"<<"W7"<<"W8"<<"W9"
                <<"WZxx"<<"W+-1"<<"W+-2"<<"W+-3"<<"W+-4"<<"W+-";
        ui->listWidget_2->addItems(str_list);
    }
    else if(item->text()=="Mde")
    {
        ui->groupBox_3->hide();
        ui->groupBox_4->hide();
        ui->listWidget_2->clear();
        str_list.clear();
        str_list<< "L"<<"L1"<< "L2"<<"L3"<<"M"<<"H"<<"max1"<<"max2"<<"max3"<<"W2"<<"W3"<<"W4"<<"W5"<<"W6"<<"W7"<<"W8"<<"W9"
                <<"WZxx"<<"W+-1"<<"W+-2"<<"W+-3"<<"W+-4"<<"W+-";
        ui->listWidget_2->addItems(str_list);
    }
    else if(item->text()=="MDw")
    {
        ui->groupBox_3->hide();
        ui->groupBox_4->hide();
        ui->listWidget_2->clear();
        str_list.clear();
        str_list<< "L"<<"M"<< "M1"<<"M2"<<"H"<<"max1"<<"max2"<<"max3"<<"W2"<<"W3"<<"W4"<<"W5"<<"W6"<<"W7"<<"W8"<<"W9"<<"Le"
                   <<"Lo"<<"M1e"<<"M1o"<<"M2Le"<<"M2Lo"<<"M2e"<<"Me"<<"He"<<"M2o"<<"Mo"<<"Ho";
        ui->listWidget_2->addItems(str_list);
    }
    else if(item->text()=="MS3")
    {
        ui->groupBox_3->hide();
        ui->groupBox_4->hide();
        ui->listWidget_2->clear();
        str_list.clear();
        str_list<<"L"<<"L1"<< "L2"<<"L3"<<"M"<<"H"<<"max1"<<"max2"<<"max3"<<"W2"<<"W3"<<"W4"<<"W5"<<"W6"<<"W7"<<"W8"<<"W9"
                   <<"WZxx"<<"W+-1"<<"W+-2"<<"W+-3"<<"W+-4"<<"W+-";
        ui->listWidget_2->addItems(str_list);
    }
    else if(item->text()=="MRS")
    {
        ui->groupBox_3->hide();
        ui->groupBox_4->hide();
        ui->listWidget_2->clear();
        str_list.clear();
        str_list<< "L"<<"M"<< "M1"<<"M2"<<"H"<<"max1"<<"max2"<<"max3"<<"W2"<<"W3"<<"W4"<<"W5"<<"W6"<<"W7"<<"W8"<<"W9";
        ui->listWidget_2->addItems(str_list);
    }
    else if(item->text()=="TSL")
    {
        ui->groupBox_3->hide();
        ui->groupBox_4->hide();
        ui->listWidget_2->clear();
        str_list.clear();
        str_list<< "L-RS"<<"M-RS"<< "H-RS"<<"maxR"<<"L"<<"M"<<"H"<<"max"<<"NB"<<"FL0"<<"FL1"<<"FL2"<<"FL3"<<"FL4"<<"FL5"
                  <<"FL6"<<"FL7"<<"FL8"<<"FL9"<<"HoRS"<<"mxoR"<<"mxHR"<<"FL0m"<<"FL1m"<<"FL2m"<<"FL3m"<<"FL4m"<<"FL5m"<<"FL6m"
                  <<"FL7m"<<"FL8m"<<"FL9m";
        ui->listWidget_2->addItems(str_list);
    }
    else if(item->text()=="MTH")
    {
        ui->groupBox_3->hide();
        ui->groupBox_4->hide();

        ui->listWidget_2->clear();
        str_list.clear();
        //        str_list<< "△r"<<"fin1"<< "VDp"<<"Vd60"<<"Vd72"<<"Vd90"<<"Vd108"<<"Vd120"<<"VDw"<<"V3D60"<<"V3D90"<<"V3D120";
        //        ui->listWidget_2->addItems(str_list);
    }
}

void add_criteria::on_listWidget_2_itemClicked(QListWidgetItem *item)
{
    str_secondary_criteria_=item->text();

    if(item->text()=="Harm")
    {
        ui->groupBox_4->show();
        ui->groupBox_3->hide();
    }


}

void add_criteria::on_pushButton_2_clicked()
{
    this->close();
}

void add_criteria::on_confirm_pushButton_clicked()
{
    QString str_value=" ";
    table_widget_band single_band;

    if(str_secondary_criteria_=="Harm")
    {
        int value=ui->spinBox->value();
        str_value=QString::number(value);
    }

    for(int i=0;i<ui->tableWidget->rowCount();i++)
    {
        if(ui->tableWidget->item(i,0)==nullptr || ui->tableWidget->item(i,0)->text().isEmpty())
        {

        }
        else
        {
            single_band.header=ui->tableWidget->verticalHeaderItem(i)->text();

            single_band.begin_num=ui->tableWidget->item(i,0)->text().toInt();
            single_band.end_num=ui->tableWidget->item(i,1)->text().toInt();

            band_.push_back(single_band);
        }

    }


    if(str_main_criteria_!=nullptr || str_secondary_criteria_!=nullptr)
    {
        if(str_main_criteria_=="FLT")
        {
            if(!band_.empty())
            {
                for(auto item:band_)
                {
                    criteria_for_selection_=str_main_criteria_+"["+item.header+"]"+" "+str_secondary_criteria_+" "+str_value;
                    emit signal_table_widget(criteria_for_selection_);
                }

                emit signal_filter_band(band_);

            }
        }
        else
        {
            auto items=ui->listWidget_2->selectedItems();  //多选
            if(!items.empty())
            {
                for(auto item:items)
                {
                   str_secondary_criteria_=item->text();

                   criteria_for_selection_=str_main_criteria_+" "+str_secondary_criteria_+" "+str_value;
                   emit signal_table_widget(criteria_for_selection_);
                   str_value="";
                }
            }
        }
    }
    this->hide();

    str_secondary_criteria_=nullptr;
}


