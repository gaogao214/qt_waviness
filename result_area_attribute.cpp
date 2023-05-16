#include "result_area_attribute.h"
#include "ui_result_area_attribute.h"
#include <QDebug>
Result_area_attribute::Result_area_attribute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Result_area_attribute)
{
    ui->setupUi(this);
}

Result_area_attribute::~Result_area_attribute()
{
    delete ui;
}

void Result_area_attribute::on_cancel_pushButton_clicked()
{
    this->hide();
}

void Result_area_attribute::on_confirm_pushButton_clicked()
{
    if(ui->tolerance_checkBox->isChecked()==true)
    {
        emit signal_tolerance(true);
    }
    else{
        emit signal_tolerance(false);
    }

    if(ui->limit_value_checkBox->isChecked()==true)
    {
        emit signal_limit_value(true);
    }else
    {
        emit signal_limit_value(false);
    }

    if(ui->header_checkBox->isChecked()==true)
    {
        emit signal_header(true);
    }else
    {
        emit signal_header(false);
    }

    int value=ui->spinBox->value();
    emit signal_control_decimal_place(value);

    this->hide();
}

void Result_area_attribute::on_use_pushButton_clicked()
{
    if(ui->tolerance_checkBox->isChecked()==true)
    {
        emit signal_tolerance(true);
    }
    else {
        emit signal_tolerance(false);
    }

    if(ui->limit_value_checkBox->isChecked()==true)
    {
        emit signal_limit_value(true);
    }else
    {
        emit signal_limit_value(false);
    }

    if(ui->header_checkBox->isChecked()==true)
    {
        emit signal_header(true);
    }else
    {
        emit signal_header(false);
    }

    int value=ui->spinBox->value();
    emit signal_control_decimal_place(value);
}
