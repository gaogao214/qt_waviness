
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
#include "start_measuring_the_thread.h"
#include <thread>

#ifdef _WIN32
#define sleep Sleep
#else
#include "msxe371x_modbus_clientlib.h"
#include <unistd.h>
#endif

#define __INCvxWorksh
#endif /*end of __INCvxWorksh*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connectingDevice();//开始连接

    QTimer *timer_calendar;
    timer_calendar=new QTimer(this);
    //    std::shared_ptr<QTimer> timer_calendar=std::make_shared<QTimer>(this);
    connect(timer_calendar,SIGNAL(timeout()),this,SLOT(show_time()));
    timer_calendar->start(1000);



    show_time_label_=new QLabel(this);      //状态栏显示时间
    show_part_count_label_=new QLabel(this);//状态栏显示计件数
    show_connect_information_=new QLabel(this);//状态栏显示连接状态


    show_part_count_label_->setText("n= 0 ");
    ui->statusbar->addPermanentWidget(show_part_count_label_);
    ui->statusbar->addPermanentWidget(show_connect_information_);

    if(config_polar_plot_==nullptr)
    {
           config_polar_plot_=std::make_unique<PolarPlotShow>();   //默认显示极图页面
    }

    if(function_key_==nullptr)
    {
        function_key_=std::make_unique<function_key>();
        connect(function_key_.get(),SIGNAL(signal_F9()),this,SLOT(on_start_measuring_triggered()));
        connect(function_key_.get(),SIGNAL(signal_config_F8()),this,SLOT(configurePopover()));
        connect(function_key_.get(),SIGNAL(signal_pole_diagram_F5()),this,SLOT(pole_diagram_pushButton_clicked()));
        connect(function_key_.get(),SIGNAL(signal_graph_F6()),this,SLOT( graph_pushButton_clicked()));
        connect(function_key_.get(),SIGNAL(signal_F3()),this,SLOT(configuration_Button_clicked()));
        connect(function_key_.get(),SIGNAL(signal_F4()),this,SLOT(pushButton_4_clicked()));
        connect(function_key_.get(),SIGNAL(signal_F10()),this,SLOT(pushButton_10_clicked()));
    }

    ui->horizontalLayout->addWidget(function_key_.get());


    ui->verticalLayout->addWidget(config_polar_plot_.get());


    config_graph_=std::make_shared<new_profile>();

    ui->verticalLayout->addWidget(config_graph_.get());

    config_graph_->hide();

    if(report_==nullptr)
    {
      report_=std::make_unique<statistical_report>();
    }


    ui->verticalLayout->addWidget(report_.get());

    report_->hide();


//    connect(ui->connect_pushButton,&QPushButton::clicked,this,&MainWindow::on_start_measuring_triggered);

    displacement_data_.push_back(0);

    //    config_polar_plot_->maindowThreadData(displacement_data_);
    if (config_configure_ == nullptr)
    {

      config_configure_=std::make_unique<configuration>();
      connect(config_configure_.get(), SIGNAL(signalResultAreaArribute(QVector<table_config>)), config_polar_plot_.get(), SLOT(reciveResultAreaAttribute_(QVector<table_config>)));
      connect(config_configure_.get(), SIGNAL(signal_recognition_config(recognition_config)), config_polar_plot_.get(), SLOT(recive_recogntion_config(recognition_config)));

    }

//    start_measuring_the_thread* start_=new start_measuring_the_thread();
//    QThread thread_;
//    start_->moveToThread(&thread_);
//    thread_.start();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::windowtitle(QString title)
{
    setWindowTitle("波纹度仪");//标题栏
}

uint32_t TRANSDUCER_SELECTION = 0;

int MainWindow::sampleMX371xTransducerGetAutoRefreshValuesEx(struct modbus * modbus, int NumberOfChannels)
{
    struct MX371x__TransducerGetAutoRefreshValuesEx_parameters_t Response;
    int ret = 0;

    memset(&Response,0,sizeof(Response));

    ret = modbus_call_MX371x__TransducerGetAutoRefreshValuesEx(modbus,&Response);

    if (!handle_result_of_FC3_query(modbus, ret, "modbus_call_MX371x__TransducerGetAutoRefreshValuesEx"))
    {
        int i;

        for (i=1;i<=NumberOfChannels;i++)
        {
            ui->label_4->setNum((*((float*)&Response.Value[i]))*1000.0);

            displacement_data_.push_back((*((float*)&Response.Value[i]))*1000.0);

        }
        return 0;
    }
    return -1;
}

int MainWindow::sampleMX371xTransducerInitAndStartAutoRefreshEx(struct modbus * modbus)
{
    struct GetLastCommandStatus_parameters_t CommandStatus;
    int ret = 0;

    std::stringstream channelmask_dec;
    int channel_num;

    uint32_t TransducerSelection = TRANSDUCER_SELECTION;
    uint32_t ChannelMask = 0x1;  //通道
    uint32_t AverageMode = 0;
    uint32_t AverageValue = 0;
    uint32_t TriggerMask = 0;
    uint32_t TriggerMode = 0;
    uint16_t HardwareTriggerCount = 0;
    uint16_t HardwareTriggerEdge = 0;
    uint32_t ByTriggerNbrOfSeqToAcquire = 1;
    uint32_t DataFormat = 3;
    uint32_t Option1 = 0;
    uint32_t Option2 = 0;
    uint32_t Option3 = 0;
    uint32_t Option4 = 0;

    channelmask_dec<<ChannelMask;  //0x转 dec
    channelmask_dec>>channel_num;

    QString str="T"+QString::number(channel_num);
    ui->transducer_label->setText(str);

    ret = modbus_call_MX371x__TransducerInitAndStartAutoRefreshEx(modbus,TransducerSelection,ChannelMask,AverageMode,AverageValue,TriggerMask,TriggerMode,HardwareTriggerEdge,HardwareTriggerCount,ByTriggerNbrOfSeqToAcquire,DataFormat,Option1,Option2,Option3,Option4,&CommandStatus);

    return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MX371x__TransducerInitAndStartAutoRefreshEx");
}

int MainWindow::sampleMX371xTransducerStopAndReleaseAutoRefreshEx(struct modbus * modbus)
{
    struct GetLastCommandStatus_parameters_t CommandStatus;
    int ret = 0;
    uint32_t Dummy = 0;

    ret = modbus_call_MX371x__TransducerStopAndReleaseAutoRefreshEx(modbus,Dummy,&CommandStatus);

    return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MX371x__TransducerStopAndReleaseAutoRefreshEx");
}

const char * MainWindow::typestr(uint32_t type)
{
    switch (type)
    {
    case 0:
        return "half-bridge";
    case 1:
        return "LVDT";
    case 2:
        return "Knaebel";
    case 3:
        return "half-bridge Mahr";
    case 4:
        return "LVDT Mahr";
    default:
        return "UNKNOWN";
    }
}

int MainWindow::sampleMX371xTransducerGetNbrOfTypeEx(struct modbus * modbus, uint32_t* NumberOfTransducerTypes)
{
    struct MX371x__TransducerGetNbrOfTypeEx_parameters_t Response;
    int ret = 0;

    memset(&Response,0,sizeof(Response));

    ret = modbus_call_MX371x__TransducerGetNbrOfTypeEx(modbus,&Response);

    if (handle_result_of_FC3_query(modbus, ret, "modbus_call_MX371x__TransducerGetNbrOfTypeEx"))
        return -1;

    *NumberOfTransducerTypes = Response.NumberOfTransducerTypes;

    return 0;
}

int MainWindow::sampleMX371xGetTransducerDatabaseCursorEx(struct modbus * modbus, uint32_t* TransducerDatabaseCursor)
{
    struct MX371x__GetTransducerDatabaseCursorEx_parameters_t Response;
    int ret = 0;

    memset(&Response,0,sizeof(Response));

    ret = modbus_call_MX371x__GetTransducerDatabaseCursorEx(modbus,&Response);

    if (handle_result_of_FC3_query(modbus, ret, "modbus_call_MX371x__GetTransducerDatabaseCursorEx"))
        return -1;

    *TransducerDatabaseCursor = Response.TransducerDatabaseCursor;

    return 0;
}

int MainWindow::sampleMX371xTransducerGetTypeInformationEx(struct modbus * modbus, struct MX371x__TransducerGetTypeInformationEx_parameters_t* TransducerGetTypeInformation)
{
    struct MX371x__TransducerGetTypeInformationEx_parameters_t Response;
    int ret = 0;

    memset(&Response,0,sizeof(Response));

    ret = modbus_call_MX371x__TransducerGetTypeInformationEx(modbus,&Response);

    if (handle_result_of_FC3_query(modbus, ret, "modbus_call_MX371x__TransducerGetTypeInformationEx"))
        return -1;

    memcpy(TransducerGetTypeInformation, &Response, sizeof(struct MX371x__TransducerGetTypeInformationEx_parameters_t));

    return 0;
}

int MainWindow::sampleMX371xSetTransducerDatabaseCursorEx(struct modbus * modbus, uint32_t new_)
{
    struct GetLastCommandStatus_parameters_t CommandStatus;
    int ret = 0;

    ret = modbus_call_MX371x__SetTransducerDatabaseCursorEx(modbus, new_, &CommandStatus);

    return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MX371x__SetTransducerDatabaseCursorEx");
}

//计数器---------------------------------------------------------------------------------
int MainWindow::sample_MSXE371x__IncCounterInit(struct modbus * modbus)
{
    struct GetLastCommandStatus_parameters_t CommandStatus;
    int ret = 0;

    uint32_t ulCounterMode = 0x1;
    uint32_t ulCounterOption = 0;
    uint32_t ulOption01 = 0;
    uint32_t ulOption02 = 0;
    uint32_t ulOption03 = 0;
    uint32_t ulOption04 = 0;

    /* Remote function call */
    ret = modbus_call_MSXE371x__IncCounterInit(modbus,ulCounterMode,ulCounterOption,ulOption01,ulOption02,ulOption03,ulOption04,&CommandStatus);

    /* Return value check */
    return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MSXE371x__IncCounterInit");
}

//---------------------------------------------------------------------------------
int MainWindow::sample_MSXE371x__IncCounterClear(struct modbus * modbus)
{
    struct GetLastCommandStatus_parameters_t CommandStatus;
    int ret = 0;

    /* Remote function call */
    ret = modbus_call_MSXE371x__IncCounterClear(modbus, 0, &CommandStatus);

    /* Return value check */
    return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MSXE371x__IncCounterClear");
}

int MainWindow::sample_MSXE371x__IncCounterWrite32BitValue(struct modbus * modbus)
{
    struct GetLastCommandStatus_parameters_t CommandStatus;
    int ret = 0;

    uint32_t ulCounterValue = 1000;

    /* Remote function call */
    ret = modbus_call_MSXE371x__IncCounterWrite32BitValue(modbus, ulCounterValue, &CommandStatus);

    /* Return value check */
    return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MSXE371x__IncCounterWrite32BitValue");
}

int MainWindow::sample_MSXE371x__IncCounterRead32BitsValue(struct modbus * modbus, uint32_t *pulValue, uint32_t *pulTimeStampLow, uint32_t *pulTimeStampHigh)
{
    struct MSXE371x__IncCounterRead32BitValue_parameters_t Response;
    int ret = 0;

    memset(&Response,0,sizeof(Response));

    ret = modbus_call_MSXE371x__IncCounterRead32BitValue(modbus,&Response);

    if (handle_result_of_FC3_query(modbus, ret, "modbus_call_MSXE371x__IncCounterRead0"))
    {
        return -1;
    }

    *pulValue         = Response.ulValue;
    *pulTimeStampLow  = Response.ulTimeStampLow;
    *pulTimeStampHigh = Response.ulTimeStampHigh;

    printf("Counter/position: %d\n",*pulValue);
    printf("Time stamp low  : %u\n", *pulTimeStampLow);
    printf("Time stamp high : %u\n", *pulTimeStampHigh);

    return 0;
}

int MainWindow::sample_MSXE371x__IncCounterRelease(struct modbus * modbus)
{
    struct GetLastCommandStatus_parameters_t CommandStatus;
    int ret = 0;

    /* Remote function call */
    ret = modbus_call_MSXE371x__IncCounterRelease(modbus, 0, &CommandStatus);

    /* Return value check */
    return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MSXE371x__IncCounterRelease");
}


/* prints the content of the transducer database on STDOUT */
int MainWindow::dumpTransducerDatabase(struct modbus * modbus)
{
    uint32_t NumberOfTransducerTypes = 0;
    int      ret                     = 0;

    ret =  sampleMX371xTransducerGetNbrOfTypeEx(modbus, &NumberOfTransducerTypes);

    /* Go through the transducer database */
    {
        unsigned int i;
        for (i=0; i<NumberOfTransducerTypes; i++)
        {
            uint32_t TransducerDatabaseCursor = 0;
            struct MX371x__TransducerGetTypeInformationEx_parameters_t TransducerGetTypeInformation;

            ret =  sampleMX371xSetTransducerDatabaseCursorEx(modbus, i);

            ret =  sampleMX371xGetTransducerDatabaseCursorEx(modbus, &TransducerDatabaseCursor);

            if (i != TransducerDatabaseCursor)
            {
                return -1;
            }

            ret =  sampleMX371xTransducerGetTypeInformationEx(modbus, &TransducerGetTypeInformation);



            transducer_name_=QString::fromUtf8(reinterpret_cast<const char*>(TransducerGetTypeInformation.Name),sizeof(TransducerGetTypeInformation.Name));

//            memcpy(&s,&TransducerGetTypeInformation.Name,100);
            qDebug()<<"name :"<<transducer_name_;

            if (i == 0)
                TRANSDUCER_SELECTION = TransducerGetTypeInformation.SelectionIndex;
        }
    }
    return 0;
}

//连接
void MainWindow::connectingDevice()
{
    modbus_init(&modbus_);

    switch (modbus_connect(&modbus_, k_ip, SOCK_STREAM, k_port))
    {
    case 0:
        ui->statusbar->clearMessage();
        ui->statusbar->showMessage("连接成功");
        break;
    case -1:
        addidata_network_perror("modbus_connect");
        ui->statusbar->clearMessage();
        ui->statusbar->showMessage("连接失败");
        break;
    case -2:
        ui->statusbar->clearMessage();
        ui->statusbar->showMessage("modbus_connect: incorrect parameter");

        break;
    case -3:
        ui->statusbar->clearMessage();
        ui->statusbar->showMessage("incorrect address");

        break;
    default:
        ui->statusbar->clearMessage();
        ui->statusbar->showMessage("连接失败");
        break;
    }
     dumpTransducerDatabase(&modbus_);

}

void MainWindow::threadGetMSXdata()
{

}
//按下按钮 开始测量
void MainWindow::on_start_measuring_triggered()
{
    uint32_t 	  ulChannelNumber      = 0;
    int           ret                  = -1;

    displacement_data_.clear();




//    dumpTransducerDatabase(&modbus_);

    int i = 0;

    sampleMX371xTransducerInitAndStartAutoRefreshEx(&modbus_);

    for (i=0;i<k_measurement_point;i++)
    {
        sampleMX371xTransducerGetAutoRefreshValuesEx(&modbus_,1);
    }

    sampleMX371xTransducerStopAndReleaseAutoRefreshEx(&modbus_);

    QString range=ui->comboBox_2->currentText();

    config_polar_plot_->maindowThreadData(displacement_data_,range);

    part_count_++;

    QString part_str_="n= "+QString::number(part_count_);
    show_part_count_label_->setText(part_str_);
    config_polar_plot_->theResultsWereCalculatedAccordingToThePrimaryAndSecondaryCriteria(config_configure_.get()->tab_band_);

    report_->tableWidgetAddData(config_polar_plot_->vec_tab_config);
    report_->recognition_data(config_polar_plot_->widget_config_);
}

void MainWindow::counterInput()
{
    int i = 0;
    uint32_t      ulValue              = 0;
    uint32_t      ulTimeStampLow       = 0;
    uint32_t      ulTimeStampHigh      = 0;

        /* Counter sample */
        if (sample_MSXE371x__IncCounterInit(&modbus_) != 0)
            { qDebug()<<"sample_MSXE371x__IncCounterInit error\n";  }

        if (sample_MSXE371x__IncCounterClear(&modbus_) != 0)
            { qDebug()<<"sample_MSXE371x__IncCounterClear error\n";  }

        if (sample_MSXE371x__IncCounterWrite32BitValue(&modbus_) != 0)
            { qDebug()<<"sample_MSXE371x__IncCounterWrite32BitValue error\n";}

        for (i=0;i<100;i++)
        {
            if (sample_MSXE371x__IncCounterRead32BitsValue(&modbus_, &ulValue, &ulTimeStampLow, &ulTimeStampHigh) != 0)
                { qDebug()<<"sample_MSXE371x__IncCounterRead32BitsValue error\n";  }
#ifdef WIN32
                sleep (100);
#else
                sleep(1);
#endif
                printf("\n");
        }

        if (sample_MSXE371x__IncCounterRelease(&modbus_) != 0)
            { qDebug()<<"sample_MSXE371x__IncCounterRelease error\n"; }
}


void MainWindow::show_time()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");

    show_time_label_->setText(str);

    ui->statusbar->addPermanentWidget(show_time_label_);
    ui->statusbar->setSizeGripEnabled(true);

}

//F6
void MainWindow::graph_pushButton_clicked()
{
    config_graph_->show();
    report_->hide();
    config_polar_plot_->hide();
}

//F5
void MainWindow::pole_diagram_pushButton_clicked()
{
    config_polar_plot_->show();

    report_->hide();

    config_graph_->hide();

}

//F4
void MainWindow::pushButton_4_clicked()
{
    config_polar_plot_->hide();

    config_graph_->hide();

    report_->show();
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



        //
//        QFile file("C://Users//gaohuan//Desktop//qt_waviness//filename.ini");
//        bool ok_ = file.rename("C://Users//gaohuan//Desktop//qt_waviness//filename.mwa");


        //因为QFile::rename()也是个静态方法,可以直接调用.返回值非常重要,是用来判断是否成功操作的.
        //                QFile file_("C://Users//gaohuan//Desktop//qt_waviness//filename.ini");
        //                file_.remove();
        //              bool ok = QFile::remove("C://Users//gaohuan//Desktop//qt_waviness//filename.ini");

        //删除文件.返回一个bool值,判断是否成功删除,如果路径不存在这些,则返回false.
        //              QFile file_("C://Users//gaohuan//Desktop//qt_waviness//filename.ini");
        //              bool ok = file_.remove();
        //              //因为QFile::remove()是个静态方法,可以直接调用.
        //              bool ok_t = QFile::remove("C://Users//gaohuan//Desktop//qt_waviness//filename.ini");



        QDir path; // 创建一个QDir变量

        auto num=filename_mwa_.size()-4;
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

//    measurement_task *config_task_=new measurement_task();
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

void MainWindow::on_config_action1_triggered()
{
    configurePopover();
}

void MainWindow::configuration_Button_clicked()
{
    config_configure_->show();
}

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

//统计报告
void MainWindow::on_action1_28_triggered()
{
    config_polar_plot_->hide();

    config_graph_->hide();

    report_->show();

}

//显示极图
void MainWindow::on_action1_25_triggered()
{

    config_polar_plot_->show();

    report_->hide();

    config_graph_->hide();
}

//显示光谱
void MainWindow::on_action1_26_triggered()
{
    config_graph_->show();
    report_->hide();
    config_polar_plot_->hide();
}

//F10
void MainWindow::pushButton_10_clicked()
{
    config_polar_plot_->hide();

    config_graph_->hide();

    report_->show();
}

void MainWindow::on_action1_9_triggered()
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

//配置密码
void MainWindow::on_action12_4_triggered()
{
    calibration_password* password_=new calibration_password();


    password_->setWindowModality(Qt::ApplicationModal); //顶层模态


    connect(password_, SIGNAL(signal_password()),this, SLOT(recive_calibration()));

    password_->show();

}

void MainWindow::recive_calibration()
{
    //auto pos=qFind(transducer_name_.begin(),transducer_name_.end(),'/');
    QStringList list = transducer_name_.split("/");

    if(calibration_==nullptr)
    {
        calibration_=std::make_unique<calibration>();
        calibration_->combox_additem(list[0]);
        calibration_->show_profile(profile_fileName_);
        calibration_->setWindowModality(Qt::ApplicationModal); //顶层模态

        connect(calibration_.get(),SIGNAL(signalMeasurement()),this,SLOT(on_start_measuring_triggered()));
        connect(calibration_.get(),SIGNAL(signalRange(QString)),config_polar_plot_.get(),SLOT(polar_diagram(QString)));
        connect(calibration_.get(),SIGNAL(signalResultsAreaDisplayed(QVector<table_config>)),config_polar_plot_.get(),SLOT(reciveResultAreaAttribute_(QVector<table_config>)));
        connect(calibration_.get(),SIGNAL(signalRange(QString)),this,SLOT(comboBox_2_show(QString)));
        connect(calibration_.get(),SIGNAL(signal_calibration_quit()),this,SLOT(calibration_quit()));

        ui->verticalLayout_2->addWidget(calibration_.get());
    }
    function_key_->hide();


}

void MainWindow::calibration_quit()
{
    calibration_->hide();
    function_key_->show();
}

void MainWindow::comboBox_2_show(QString num)
{
    ui->comboBox_2->setCurrentIndex(ui->comboBox_2->findText(num));
//    ui->comboBox_2->addItem(num);
}

void MainWindow::on_comboBox_2_activated(const QString &arg1)
{

    qDebug()<<arg1;
    config_polar_plot_->polar_diagram(arg1);
}

void MainWindow::on_action1_10_triggered()
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

void MainWindow::on_action1_triggered()
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
