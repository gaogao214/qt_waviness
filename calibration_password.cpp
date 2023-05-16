#include "calibration_password.h"
#include "ui_calibration_password.h"
#include <QMessageBox>
#include <QDialog>
//#include "calibration.h"

calibration_password::calibration_password(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::calibration_password)
{
    ui->setupUi(this);
}

calibration_password::~calibration_password()
{
    delete ui;
}

void calibration_password::on_pushButton_4_clicked()
{
    this->close();
}

void calibration_password::on_pushButton_3_clicked()
{
    if(ui->password_lineEdit_2->text()==tr("123456"))
    {
//        calibration* calib_=new calibration();
//        calib_->setWindowModality(Qt::ApplicationModal); //顶层模态
//        calib_->show();

        emit signal_password();

        this->close();
    }
    else
    {
        QMessageBox::warning( this, tr("Warning"), tr("password error!"), QMessageBox::Yes);
    }
}
