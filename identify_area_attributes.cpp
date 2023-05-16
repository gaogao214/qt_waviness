#include "identify_area_attributes.h"
#include "ui_identify_area_attributes.h"
#include <QGridLayout>
#include <QCheckBox>
identify_area_attributes::identify_area_attributes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::identify_area_attributes)
{
    ui->setupUi(this);

}

identify_area_attributes::~identify_area_attributes()
{
    delete ui;
}

void identify_area_attributes::on_ok_pushButton_clicked()
{
    if(ui->part_type_checkBox->isChecked()==true)
    {
        emit signal_type_of_part(true);
    }
    else
    {
         emit signal_type_of_part(false);
         ui->part_type_checkBox->setChecked(false);
    }

    if(ui->attribute_checkBox->isChecked()==true)
    {
        emit signal_attribute(true);
    }else
    {
        emit signal_attribute(false);
        ui->attribute_checkBox->setChecked(false);
    }

    if(ui->amount_checkBox->isChecked()==true)
    {
        emit signal_amount(true);
    }else
    {
        emit signal_amount(false);
        ui->amount_checkBox->setChecked(false);
    }

    if(ui->order_checkBox->isChecked()==true)
    {
        emit signal_order(true);
    }else
    {
        emit signal_order(false);
        ui->order_checkBox->setChecked(false);
    }

    if(ui->client_checkBox->isChecked()==true)
    {
        emit signal_client(true);
    }else
    {
        emit signal_client(false);
        ui->client_checkBox->setChecked(false);
    }

    if(ui->channel_checkBox->isChecked()==true)
    {
        emit signal_channel(true);
    }
    else
    {
        emit signal_channel(false);
        ui->channel_checkBox->setChecked(false);
    }

    if(ui->operator_checkBox->isChecked()==true)
    {
        emit signal_operator(true);
    }else
    {
        emit signal_operator(false);
        ui->operator_checkBox->setChecked(false);
    }

    if(ui->tor_numcheckBox->isChecked()==true)
    {
        emit signal_tor_number(true);
    }else
    {
        emit signal_tor_number(false);
        ui->tor_numcheckBox->setChecked(false);
    }

    if(ui->factory_checkBox->isChecked()==true)
    {
        emit signal_factory(true);
    }else
    {
        emit signal_factory(false);
        ui->factory_checkBox->setChecked(false);
    }

    if(ui->branch_checkBox->isChecked()==true)
    {
        emit signal_branch(true);
    }else
    {
        emit signal_branch(false);
        ui->branch_checkBox->setChecked(false);
    }

    if(ui->machine_checkBox->isChecked()==true)
    {
        emit signal_machine(true);
    }else
    {
        emit signal_machine(false);
        ui->machine_checkBox->setChecked(false);
    }

    if(ui->operate_checkBox->isChecked()==true)
    {
        emit signal_operate(true);
    }else
    {
        emit signal_operate(false);
        ui->operate_checkBox->setChecked(false);
    }

    this->hide();
}

void identify_area_attributes::on_cancel_pushButton_clicked()
{
    this->hide();
}
