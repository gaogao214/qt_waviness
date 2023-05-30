#include "calibration.h"
#include "ui_calibration.h"
#include <QPrinter>
#include <QPrintDialog>
#include <qDebug>
#include <QPrintPreviewDialog>
#include <QTimer>
#include <QDateTime>
#include <QSettings>

calibration::calibration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::calibration)
{
    ui->setupUi(this);

    QTimer *timer_calendar;
    timer_calendar=new QTimer(this);
    //    std::shared_ptr<QTimer> timer_calendar=std::make_shared<QTimer>(this);
    connect(timer_calendar,SIGNAL(timeout()),this,SLOT(show_time()));
    timer_calendar->start(1000);

    ui->lineEdit_24->setText("1.0");
    ui->lineEdit_25->setText("1.0");

}

calibration::~calibration()
{
    delete ui;


}
void calibration::show_time()
{
    QDateTime time = QDateTime::currentDateTime();
    str_time_ = time.toString("yyyy-MM-dd hh:mm:ss dddd");


}
void calibration::on_pushButton_18_clicked()
{
    this->close();


    QSettings setting(profile_filename_, QSettings::IniFormat);

    QString str=ui->lineEdit_20->text();
    setting.setValue("CALIBRATION/MasterPartNr",str);

    str=ui->lineEdit_21->text();
    setting.setValue("CALIBRATION/MasterPartValue",str);

    str=ui->lineEdit_26->text();
    setting.setValue("CALIBRATION/SmallMasterPartLastTime",str);
    str=ui->lineEdit_27->text();
    setting.setValue("CALIBRATION/BigMasterPartLastTime",str);



    setting.sync();//写入配置文件

    emit signal_calibration_quit();

}



//打印预览
void calibration::on_pushButton_17_clicked()
{

    QPrintPreviewDialog dlg1;//创建打印预览dlg
    connect(&dlg1, SIGNAL(paintRequested(QPrinter*)), this,SLOT(paintRequestedHandler(QPrinter*)));
    dlg1.setWindowFlag(Qt::WindowMinMaxButtonsHint);//设置窗口属性，支持最大最小化，不设置这个窗口不能双击标题栏放大缩小，也没有最大最小化按钮
    dlg1.exec();//界面显示
}

void calibration::paintRequestedHandler(QPrinter *printerPixmap)
{
    //QPageLayout("A4", Landscape, l:0 r:0 t:0 b:0 pt);
    //A4纸张大小；Portrait纵向；Landscape横向；l/r/t/b上下左右页边距；
    //    qDebug()<<"当前页面布局："<<printerPixmap->pageLayout();//下面的绘图参考这几个参数

//    QPainter Painter(printerPixmap);

//    ui->widget_3->render(&Painter);

//    printerPixmap->newPage();//新建立一个页面
//    Painter.drawText(30,20,"第2页");//页面显示内容

//    printerPixmap->newPage();//新建立一个页面
//    Painter.drawText(30,20,"第3页");//页面显示内容
}

void calibration::combox_additem(QString item)
{
     ui->comboBox_3->addItem(item);
}

//小 测量
void calibration::on_pushButton_22_clicked()
{
    ui->lineEdit_26->setText(str_time_);

    QVector<table_config> config_;
    table_config con_;

    con_.main_criteria="LSC";
    con_.secondary_criteria="△r";

    config_.push_back(con_);

    QSettings setting(profile_filename_, QSettings::IniFormat);

    QString str=ui->lineEdit_20->text();
    setting.setValue("CALIBRATION/MasterPartNr",str);

    str=ui->lineEdit_21->text();
    setting.setValue("CALIBRATION/MasterPartValue",str);

    str=ui->lineEdit_26->text();
    setting.setValue("CALIBRATION/SmallMasterPartLastTime",str);
    str=ui->lineEdit_27->text();
    setting.setValue("CALIBRATION/BigMasterPartLastTime",str);



    setting.sync();//写入配置文件

    emit signalResultsAreaDisplayed(config_);
    emit signalRange( ui->lineEdit_22->text());

    emit signalMeasurement();




}

//大 测量
void calibration::on_pushButton_23_clicked()
{
     ui->lineEdit_27->setText(str_time_);

     QVector<table_config> config_;
     table_config con_;

     con_.main_criteria="LSC";
     con_.secondary_criteria="△r";

     config_.push_back(con_);

     QSettings setting(profile_filename_, QSettings::IniFormat);

     QString str=ui->lineEdit_20->text();
     setting.setValue("CALIBRATION/MasterPartNr",str);

     str=ui->lineEdit_21->text();
     setting.setValue("CALIBRATION/MasterPartValue",str);

     str=ui->lineEdit_26->text();
     setting.setValue("CALIBRATION/SmallMasterPartLastTime",str);
     str=ui->lineEdit_27->text();
     setting.setValue("CALIBRATION/BigMasterPartLastTime",str);



     setting.sync();//写入配置文件

     emit signalResultsAreaDisplayed(config_);
     emit signalRange( ui->lineEdit_23->text());



     emit signalMeasurement();

}

void calibration::getLineEdit()
{
    ui->lineEdit_22->text();
}

void calibration::show_profile(QString filename)
{
    QVector<QString> vec_value;

    profile_filename_=filename;

    QSettings *settings = new QSettings(/*"C://Users//gaohuan//Desktop//qt_waviness//01.mwa"*/filename,QSettings::IniFormat);


    QString str= settings->value("CALIBRATION/MasterPartNr").toString();
    ui->lineEdit_20->setText(str);

    str=settings->value("CALIBRATION/MasterPartValue").toString();
    ui->lineEdit_21->setText(str);

    str=settings->value("CALIBRATION/SmallMasterPartLastTime").toString();
    ui->lineEdit_26->setText(str);

    str=settings->value("CALIBRATION/BigMasterPartLastTime").toString();
    ui->lineEdit_27->setText(str);
}

void calibration::measuringLSCrResult(QString data,QString range)
{
    qDebug()<<"measuringLSCrResult()range:"<<range<<"\n";

    QString str=ui->lineEdit_21->text();//标定值

    QString lsc= data.mid(6,data.size());//测量值

    qDebug()<<"measuringLSCrResult()信号槽接收到"<<lsc<<"\n";

    float lsc_=lsc.toFloat();
    data = QString::number(lsc_, 'f', 2);



    if(data!=str)
    {
        if(range=="+-50μm")
        {
            small_amplification_coefficient_=str.toFloat()/lsc_;//放大系数

            qDebug()<<"measuringLSCrResult() amplification:"<<small_amplification_coefficient_<<"\n";

            ui->lineEdit_24->setText(QString::number(small_amplification_coefficient_));//放大系数显示

        }else if(range=="+-500μm")
        {
            large_amplification_coefficient_=str.toFloat()/lsc_;//放大系数

            qDebug()<<"measuringLSCrResult() amplification:"<<large_amplification_coefficient_<<"\n";

            ui->lineEdit_25->setText(QString::number(large_amplification_coefficient_));//放大系数显示
        }


    }


}

void calibration::on_small_calibration_clicked()
{
    emit signalSmallCalibration(small_amplification_coefficient_);
}

void calibration::on_large_calibration_clicked()
{
    emit signalLargeCalibration(large_amplification_coefficient_);
}
