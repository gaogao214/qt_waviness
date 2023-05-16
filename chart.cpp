#include "chart.h"
#include "ui_chart.h"
#include <QDebug>
#include <math.h>
chart::chart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chart)
{
    ui->setupUi(this);

    ui->spinBox->setRange(8, 2048);
    ui->spinBox->setFocusPolicy(Qt::NoFocus);


}

chart::~chart()
{
    delete ui;
}



void chart::on_comboBox_2_currentIndexChanged(const QString &arg1)
{
    if(arg1=="波纹度")
    {
        emit signal_show_type_of_spectrum("waviness");
    }
    else if(arg1=="谐波")
    {
        emit signal_show_type_of_spectrum("harmonic_wave");
    }
    else if(arg1=="线性曲线")
    {
        emit signal_show_type_of_spectrum("linearity_curve");
    }
}

void chart::on_pushButton_clicked()
{
    this->hide();
}

void chart::on_spinBox_valueChanged(int arg1)
{

    int count =0;

    emit signal_show_x(arg1);

    if(count<12)
    {
        ui->spinBox->setSingleStep(arg1*pow(2,count));   //设置步长
        count++;
    }
}


void chart::on_comboBox_currentTextChanged(const QString &arg1)
{
     qDebug()<<arg1;
     emit signal_show_y(arg1);
}
