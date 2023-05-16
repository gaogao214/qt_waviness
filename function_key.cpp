#include "function_key.h"
#include "ui_function_key.h"

function_key::function_key(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::function_key)
{
    ui->setupUi(this);
}

function_key::~function_key()
{
    delete ui;
}

void function_key::on_connect_pushButton_clicked()
{

    emit signal_F9();
}

void function_key::on_config_pushButton_clicked()
{
    emit signal_config_F8();
}

void function_key::on_pole_diagram_pushButton_clicked()
{
    emit signal_pole_diagram_F5();
}

void function_key::on_graph_pushButton_clicked()
{
    emit signal_graph_F6();
}

void function_key::on_configuration_Button_clicked()
{
    emit signal_F3();
}

void function_key::on_pushButton_4_clicked()
{
    emit signal_F4();
}

void function_key::on_pushButton_10_clicked()
{
    emit signal_F10();
}
