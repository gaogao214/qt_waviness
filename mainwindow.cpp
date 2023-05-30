
/** @file modbuscli.c MODBUS/UDP client sample for the MSXE3711
 *
 * FC16 queries are for setting things, FC3 for fetching values
 *
 * This code doesn't handle timeouts.
 */

#ifndef __INCvxWorksh /*防止该头文件被重复引用*/


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <thread>
#include <sstream>
#include "qcustomplot.h"
#include "configuration.h"

#include "mouse_release_event.h"
#include "result_area_attribute.h"
#include "identify_area_attributes.h"

#include <QPrintDialog>
#include <QSettings>
#include "calibration_password.h"
#include "string.h"
#include "calibration.h"
#include <string>
#include "function_key.h"

#include <thread>

#define __INCvxWorksh
#endif /*end of __INCvxWorksh*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qRegisterMetaType<struct modbus>("struct modbus");

    real_time_=std::make_unique<QTimer>(this);
    connect(real_time_.get(),SIGNAL(timeout()),this,SLOT(showTime()));
    real_time_->start(1000);

    initStatusBar();//初始化状态栏

    initFunctionKey();//初始化功能键栏

    connect(ui->polar_diagram,&QAction::triggered,this,&MainWindow::poleDiagramPushButtonClicked);
    connect(ui->spectrogram,&QAction::triggered,this,&MainWindow::graphPushButtonClicked);
    connect(ui->statistical_report,&QAction::triggered,this,&MainWindow:: statisticalReportClicked);

    // 线程
    if(controller_==nullptr)
    {
        controller_=std::make_unique<controller>();
        connect(controller_.get(),&controller::signal_msx_data,this,&MainWindow::reciveMsxe3711Data);
        connect(controller_.get(),&controller::signal_msx_measurement_point,this,&MainWindow::getMSXE3711MeasurementPoint);
        connect(controller_.get(),&controller::signal_msx_sensor_type,this,&MainWindow::reciveSensorType);
        connect(controller_.get(),&controller::signal_msx_channel,this,&MainWindow::reciveMsxE3711Channel);
        connect(controller_.get(),&controller::signal_msx_evaluation_criterion,this,&MainWindow::getMSXE3711EvaluationCriterion);
    }

//    emit controller_->msx_connect();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showTime()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");

    show_time_label_=std::make_unique<QLabel>(this);

    show_time_label_->setText(str);

    ui->statusbar->addPermanentWidget(show_time_label_.get());
    ui->statusbar->setSizeGripEnabled(true);

}

//初始化状态栏
void MainWindow::initStatusBar()
{
    show_part_count_label_=std::make_unique<QLabel>(this);//状态栏显示计件数
    show_part_count_label_->setText("n= 0 ");
    ui->statusbar->addPermanentWidget(show_part_count_label_.get());
}

//初始化功能键栏
void MainWindow::initFunctionKey()
{
    if(function_key_==nullptr)
    {
        function_key_=std::make_unique<function_key>();
        connect(function_key_.get(),SIGNAL(signal_F9()),this,SLOT(on_start_measuring_triggered()));
        connect(function_key_.get(),SIGNAL(signal_config_F8()),this,SLOT(configurePopover()));
        connect(function_key_.get(),SIGNAL(signal_pole_diagram_F5()),this,SLOT(poleDiagramPushButtonClicked()));
        connect(function_key_.get(),SIGNAL(signal_graph_F6()),this,SLOT( graphPushButtonClicked()));
        connect(function_key_.get(),SIGNAL(signal_F3()),this,SLOT(configurationButtonClicked()));
        connect(function_key_.get(),SIGNAL(signal_F4()),this,SLOT(statisticalReportClicked()));
        connect(function_key_.get(),SIGNAL(signal_F10()),this,SLOT(pushButton_10_clicked()));
    }
    ui->horizontalLayout->addWidget(function_key_.get());

    //初始化校准页面
    if(calibration_==nullptr)
    {
        calibration_=std::make_unique<calibration>();
    }

    //默认显示极图页面 F5 (在主页面显示)
    if(config_polar_plot_==nullptr)
    {
         config_polar_plot_=std::make_unique<PolarPlotShow>();

         connect(config_polar_plot_.get(),SIGNAL(signalLSCrToController(QString,QString)),calibration_.get(),SLOT(measuringLSCrResult(QString,QString)));


    }
    ui->verticalLayout->addWidget(config_polar_plot_.get());
    config_polar_plot_->show();

    //配置 F3
    if (config_configure_ == nullptr)
    {
      config_configure_=std::make_unique<configuration>();
      connect(config_configure_.get(), SIGNAL(signalResultAreaArribute(QVector<table_config>)), config_polar_plot_.get(), SLOT(reciveResultAreaAttribute_(QVector<table_config>)));
      connect(config_configure_.get(), SIGNAL(signal_recognition_config(recognition_config)), config_polar_plot_.get(), SLOT(reciveRecogntionConfig(recognition_config)));
    }

    // F4 统计报告 (在主页面显示)
    if(report_==nullptr)
    {
      report_=std::make_unique<statistical_report>();
    }
    ui->verticalLayout->addWidget(report_.get());
    report_->hide();

    //光谱图 F6 (在主页面显示)
    if(config_graph_==nullptr)
    {
        config_graph_=std::make_unique<new_profile>();
    }
    ui->verticalLayout->addWidget(config_graph_.get());
    config_graph_->hide();

}

//标题栏
void MainWindow::windowTitle(QString title)
{
    setWindowTitle("波纹度仪");
}

//按下按钮 开始测量
void MainWindow::on_start_measuring_triggered()
{
    // 线程
    if(controller_==nullptr)
    {
        controller_=std::make_unique<controller>();
        connect(controller_.get(),&controller::signal_msx_data,this,&MainWindow::reciveMsxe3711Data);
        connect(controller_.get(),&controller::signal_msx_measurement_point,this,&MainWindow::getMSXE3711MeasurementPoint);
        connect(controller_.get(),&controller::signal_msx_sensor_type,this,&MainWindow::reciveSensorType);
        connect(controller_.get(),&controller::signal_msx_channel,this,&MainWindow::reciveMsxE3711Channel);
        connect(controller_.get(),&controller::signal_msx_evaluation_criterion,this,&MainWindow::getMSXE3711EvaluationCriterion);
    }
    emit controller_->operate_msx_data();

    qDebug() << "MainWindow()"<< "thread:" << QThread::currentThreadId();

    loadingMeasurement();//加载中

    part_count_++;//计件数

    QString part_str_="n= "+QString::number(part_count_);
    show_part_count_label_->setText(part_str_);

}

//加载中
void MainWindow::loadingMeasurement()
{
    // show delay dialog
    delay_dialog_ = std::make_unique<QDialog>(this);
    delay_dialog_->setFixedSize(150, 150);
    delay_dialog_->setAttribute(Qt::WA_TranslucentBackground);
    delay_dialog_->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    delay_label_ = std::make_unique<QLabel>(delay_dialog_.get());
    delay_label_->setStyleSheet("background-color:transparent");
    delay_label_->resize(150, 150);
    delay_label_->setScaledContents(true);

    delay_movie_ = std::make_unique<QMovie>("C:/Users/gaohuan/Desktop/qt_waviness/qt_msx-e371x_modbus_qwt/images/waiting.gif");
    delay_movie_->setCacheMode(QMovie::CacheAll);
    delay_label_->setMovie(delay_movie_.get());

    delay_movie_->start();
    delay_dialog_->exec();
}

void MainWindow::getMSXE3711MeasurementPoint(double data)
{
    ui->label_4->setNum(data);
}

void MainWindow::reciveSensorType(QString sensor)
{
    transducer_name_=sensor;
}

void MainWindow::reciveMsxE3711Channel(QString channel)
{
    ui->transducer_label->setText(channel);
}

void MainWindow::getMSXE3711EvaluationCriterion(double leastsquare_radius_of_circle,double minimum_radius_of_outer_circle)
{

    config_polar_plot_->MSXE3711EvaluationCriterion(leastsquare_radius_of_circle,minimum_radius_of_outer_circle);
}


void MainWindow::reciveMsxe3711Data(QVector<double> data,QVector<double> amplitude)
{
    qDebug()<<"recive_msxe3711_data!\n";

    QString range=ui->comboBox_2->currentText();

    config_polar_plot_->maindowThreadData(data,amplitude,range);//极图半径范围

//    connect(config_polar_plot_.get(),SIGNAL(signalLSCrToController(QString)),calibration_.get(),SLOT(measuringLSCrResult(QString)));


    config_polar_plot_->theResultsWereCalculatedAccordingToThePrimaryAndSecondaryCriteria(config_configure_.get()->tab_band_);

    report_->tableWidgetAddData(config_polar_plot_->vec_tab_config);
    report_->recognition_data(config_polar_plot_->widget_config_);

    finishedMeasurement();//加载完成
}

void MainWindow::finishedMeasurement()
{
    if(delay_movie_)
    {
        delay_movie_->stop();
        delay_movie_.reset(nullptr);
    }

    if(delay_dialog_)
    {
        delay_dialog_->close();
        delay_movie_.reset(nullptr);
    }

    if(delay_label_)
    {
        delay_label_.reset(nullptr);
    }
}

//F4 统计报告
void MainWindow::statisticalReportClicked()
{
    config_polar_plot_->hide();

    config_graph_->hide();

    report_->show();
}

//F5 显示极图
void MainWindow::poleDiagramPushButtonClicked()
{
    config_polar_plot_->show();

    report_->hide();

    config_graph_->hide();

}

//F6 显示光谱图
void MainWindow::graphPushButtonClicked()
{
    config_graph_->show();
    report_->hide();
    config_polar_plot_->hide();
}

//文件-》新建-》配置-》保存并出现弹窗
void MainWindow::on_new_profile_triggered()
{
    profile_arguments::profile_order order_;

    filename_mwa_ = QFileDialog::getSaveFileName(this,
                                                    tr("Save Image"),
                                                    "",
                                                    tr("*.mwa")); //选择路径
    qDebug()<<filename_mwa_;
    if(filename_mwa_.isEmpty())
    {
        QMessageBox::critical(this, tr("提示消息框"), tr("文件名为空！"));
        //        return;
    }
    else
    {
        QSettings *settings = new QSettings(/*"C://Users//gaohuan//Desktop//qt_waviness//01.mwa"*/filename_mwa_,QSettings::IniFormat);
//        qDebug() << QCoreApplication::applicationDirPath();
        QString value_0=QString::number(0)+","+QString::number(0)
                +","+QString::number(0);
        settings->setValue("Filter/FL0",value_0);

        QString value_1=QString::number(0)+","+QString::number(0)
                +","+QString::number(0);
         settings->setValue("Filter/FL1",value_1);

        QString value_2=QString::number(0)+","+QString::number(0)
                +","+QString::number(0);
        settings->setValue("Filter/FL2",value_2);

        QString value_3=QString::number(0)+","+QString::number(0)
                +","+QString::number(0);
        settings->setValue("Filter/FL3",value_3);

        QString value_4=QString::number(0)+","+QString::number(0)
                +","+QString::number(0);
        settings->setValue("Filter/FL4",value_4);

        QString value_5=QString::number(0)+","+QString::number(0)
                +","+QString::number(0);
        settings->setValue("Filter/FL5",value_5);

        QString value_6=QString::number(0)+","+QString::number(0)
                +","+QString::number(0);
        settings->setValue("Filter/FL6",value_6);

        QString value_7=QString::number(0)+","+QString::number(0)
                +","+QString::number(0);
        settings->setValue("Filter/FL7",value_7);

        QString value_8=QString::number(0)+","+QString::number(0)
                +","+QString::number(0);
        settings->setValue("Filter/FL8",value_8);

        QString value_9=QString::number(0)+","+QString::number(0)
                +","+QString::number(0);
        settings->setValue("Filter/FL9",value_9);

        settings->setValue("Order/PartNumber"," ");
        settings->setValue("Order/PartName"," ");
        settings->setValue("Order/Number of pieces"," ");
        settings->setValue("Order/Order"," ");
        settings->setValue("Order/Customer"," ");
        settings->setValue("Order/Channel"," ");
        settings->setValue("Order/Operator"," ");
        settings->setValue("Order/Operator ID"," ");
        settings->setValue("Order/Factory","GTCC");
        settings->setValue("Order/Department","PIC");
        settings->setValue("Order/Machine","MWA 420C");
        settings->setValue("Order/Operation"," ");

        settings->setValue("AUTOSAVE/AutoSaveMode","0");
        settings->setValue("AUTOSAVE/AutoSaveFile"," ");
        settings->setValue("AUTOSAVE/AddStatMode","0");


        settings->setValue("Measurement/MS","");
        settings->setValue("Measurement/CalcWZTol","");
        settings->setValue("General/Version","");

        settings->setValue("CALIBRATION/MasterPartNr","");
        settings->setValue("CALIBRATION/MasterPartValue","");
        settings->setValue("CALIBRATION/SmallMasterPartLastTime","2023/5/11");
        settings->setValue("CALIBRATION/BigMasterPartLastTime","2023/5/11");



        qDebug() << settings->value("Criterias/ClassNames");


        QDir path; // 创建一个.mwa配置文件 同时创建一个同名的文件夹

        auto num=filename_mwa_.size()-4;//获取路径名
        auto file=filename_mwa_.mid(0,filename_mwa_.size()-4);

        if (!path.exists(file)) {  // 使用QDir成员函数exists()来判断文件夹是否存在
            path.mkdir(file);  // 使用mkdir来创建文件夹
        }

        configurePopover();

    }



}

//文件-》新建-》测量任务
void MainWindow::on_measurement_task_triggered()
{

    if(config_task_.get()==nullptr)
    {
        config_task_=std::make_unique<measurement_task>();
        connect(config_task_.get(), SIGNAL(down_ok()), this, SLOT(configurePopover()));
        connect(config_task_.get(), SIGNAL(signal_profile_file(QString )), config_configure_.get(), SLOT(open_mwa_profile(QString)));
    }

    config_task_->setWindowModality(Qt::ApplicationModal);
    config_task_->show();

}

//退出
void MainWindow::on_exit_triggered()
{
    QApplication* app;
    app->exit(0);

}

//配置
void MainWindow::configurePopover()
{
    if (config_configure_ == nullptr)
    {
        config_configure_=std::make_unique<configuration>();
        config_configure_->configuration_filename_mwa_=filename_mwa_;
        config_configure_->setWindowModality(Qt::ApplicationModal); //顶层模态
        config_configure_->show();

        connect(config_configure_.get(), SIGNAL(signalResultAreaArribute(QVector<table_config>)), config_polar_plot_.get(), SLOT(reciveResultAreaAttribute_(QVector<table_config>)));
        connect(config_configure_.get(), SIGNAL(signal_recognition_config(recognition_config)), config_polar_plot_.get(), SLOT(recive_recogntion_config(recognition_config)));

    }
    else
    {
        config_configure_->configuration_filename_mwa_=filename_mwa_;
        config_configure_->show();
    }

}

//配置-》配置
void MainWindow::on_config_action1_triggered()
{
    configurePopover();
}

//F8
void MainWindow::configurationButtonClicked()
{
    config_configure_->show();
}

//打印机
void MainWindow::on_action1_12_triggered()
{

    QPrinter printerpng;//创建一个打印机
    QPrintDialog dlg(&printerpng);//创建打印页面，并传入打印机
    qDebug()<<dlg.exec();//显示打印界面，返回值判断点击的是0表示取消，1表示打印
    //界面显示后，可以通过printerpng获取设置的打印页面信息
    qDebug()<<"打印页面设置的纸张页面布局："<<printerpng.pageLayout()<<endl;
    qDebug()<<"打印页面设置的打印机名称："<<printerpng.printerName()<<endl;


}

void MainWindow::on_action1_13_triggered()
{

}

//F10
void MainWindow::pushButton_10_clicked()
{
    config_polar_plot_->hide();

    config_graph_->hide();

    report_->show();
}

void MainWindow::calibrationMeasurement()
{
    config_polar_plot_->reciveFlag(true);

    on_start_measuring_triggered();
}

//校准页面
void MainWindow::reciveCalibration()
{
    QStringList list = transducer_name_.split("/");//获取传感器的名字

    if(calibration_==nullptr)
    {
        calibration_=std::make_unique<calibration>();

    }

    calibration_->combox_additem(list[0]);
    calibration_->show_profile(profile_fileName_);
    calibration_->setWindowModality(Qt::ApplicationModal); //顶层模态

    connect(calibration_.get(),SIGNAL(signalMeasurement()),this,SLOT(calibrationMeasurement()));
    connect(calibration_.get(),SIGNAL(signalRange(QString)),config_polar_plot_.get(),SLOT(polar_diagram(QString)));
    connect(calibration_.get(),SIGNAL(signalResultsAreaDisplayed(QVector<table_config>)),config_polar_plot_.get(),SLOT(reciveResultAreaAttribute_(QVector<table_config>)));
    connect(calibration_.get(),SIGNAL(signalRange(QString)),this,SLOT(comboBox_2_show(QString)));
    connect(calibration_.get(),SIGNAL(signal_calibration_quit()),this,SLOT(calibrationQuit()));
    connect(calibration_.get(),SIGNAL(signalSmallCalibration(float)),config_polar_plot_.get(),SLOT(reciveSmallCalibration(float)));
    connect(calibration_.get(),SIGNAL(signalLargeCalibration(float)),config_polar_plot_.get(),SLOT(reciveLargeCalibration(float)));


    ui->verticalLayout_2->addWidget(calibration_.get());
    function_key_->hide();
}

//校准关闭
void MainWindow::calibrationQuit()
{
    calibration_->hide();
    function_key_->show();

    config_polar_plot_->reciveFlag(false);
}

//范围
void MainWindow::comboBox_2_show(QString num)
{
    ui->comboBox_2->setCurrentIndex(ui->comboBox_2->findText(num));
}


void MainWindow::on_comboBox_2_activated(const QString &arg1)
{
    qDebug()<<arg1;
    config_polar_plot_->polar_diagram(arg1);
}

//校准密码
void MainWindow::on_calibration_password_triggered()
{
    calibration_password* password_=new calibration_password();

    password_->setWindowModality(Qt::ApplicationModal); //顶层模态

    connect(password_, SIGNAL(signal_password()),this, SLOT(reciveCalibration()));

    password_->show();
}

//新建->打开-》配置
void MainWindow::on_open_configuration_triggered()
{
    profile_fileName_ = QFileDialog::getOpenFileName(
            this,
            tr("open a file."),
            "D:/",
            tr("*mwa"));
        if (profile_fileName_.isEmpty()) {
            QMessageBox::warning(this, "Warning!", "Failed to open the video!");
        }
        else {
            if (config_configure_ == nullptr)
            {
                config_configure_=std::make_unique<configuration>();
            }
            config_configure_->open_mwa_profile(profile_fileName_);
        }
}

//新建-》打开-》测量作业
void MainWindow::on_open_measurement_job_triggered()
{
    if(config_task_.get()==nullptr)
    {
        config_task_=std::make_unique<measurement_task>();
        connect(config_task_.get(), SIGNAL(down_ok()), this, SLOT(configurePopover()));
        connect(config_task_.get(), SIGNAL(signal_profile_file(QString )), config_configure_.get(), SLOT(open_mwa_profile(QString)));
    }

    config_task_->setWindowModality(Qt::ApplicationModal);
    config_task_->show();
}

//新建-》另存为-》配置
void MainWindow::on_save_as_configuration_triggered()
{

    QString newFilePath = QFileDialog::getSaveFileName(this,tr("另存为"),"","*.mwa"); //选择路径
    qDebug()<<newFilePath;


      QString filePath;
      QStringList pathList=newFilePath.split("/");//用于检查新路径是否存在
      QString dir;
      for(int i;i<pathList.size();i++)
      {
          if(i!=pathList.size()-1)
          {
              dir+=pathList[i];
              dir+="/";
          }
      }
      QDir path(dir);
      if(path.exists())
      {
          QFile file(profile_fileName_);
          file.copy(profile_fileName_,newFilePath);
      }
      else
      {
          path.mkdir(dir);//若路径不存在则创建不存在的文件夹
          QFile file(profile_fileName_);
          file.copy(profile_fileName_,newFilePath);
      }
}
