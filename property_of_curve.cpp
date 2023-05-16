#include "property_of_curve.h"
#include "ui_property_of_curve.h"

property_of_curve::property_of_curve(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::property_of_curve)
{
    ui->setupUi(this);


}

property_of_curve::~property_of_curve()
{
    delete ui;
}


void property_of_curve::on_pushButton_clicked()
{

    if(ui->checkBox->isChecked()==true)
    {
        emit display_Remarks(true);
    }
    else
    {
        emit display_Remarks(false);
        ui->checkBox->setChecked(false);
    }

    if(ui->checkBox_2->isChecked()==true)
    {
        emit display_Time(true);
    }
    else
    {
        emit display_Time(false);
        ui->checkBox_2->setChecked(false);
    }
    this->hide();
}
