#include "circle_property.h"
#include "ui_circle_property_.h"

circle_property_::circle_property_(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::circle_property_)
{
    ui->setupUi(this);
}

circle_property_::~circle_property_()
{
    delete ui;
}

void circle_property_::on_pushButton_2_clicked()
{
    if(ui->checkBox_2->isChecked()==true)
    {
        emit display_annotation(true);
    }
    else
    {
        emit display_annotation(false);
        ui->checkBox_2->setChecked(false);
    }
    if(ui->checkBox_3->isChecked()==true)
    {
        emit display_time(true);
    }
    else
    {
        emit display_time(false);
        ui->checkBox_3->setChecked(false);
    }
    this->hide();
}
