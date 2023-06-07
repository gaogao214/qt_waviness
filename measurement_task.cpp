#include "measurement_task.h"
#include "ui_measurement_task.h"
#include <QFileDialog>
#include <QDebug>
#include <QDateTime>
#include <QSettings>

measurement_task::measurement_task(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::measurement_task)
{
    ui->setupUi(this);

    ui->path_lineedit->setText(QDir::currentPath());

    ui->comboBox->addItem(".");
    ui->comboBox->addItem("setup");


}

measurement_task::~measurement_task()
{
    delete ui;
}

void measurement_task::on_pushButton_clicked()
{
    ui->listWidget->clear();

    path_name_= QFileDialog::getExistingDirectory(this, "选择配置文件所在文件夹", "./");

    ui->path_lineedit->setText(path_name_);

    QString workpath_=QDir::currentPath();

    if(path_name_==workpath_)
    {
        inputCharComboBox(".");
    }

}

void measurement_task::inputCharComboBox(const QString &str)
{
    QVector<QString> combobox_str;

    for(int idx=0; idx<ui->comboBox->count(); idx++){
        QString itemName = ui->comboBox->itemText(idx);

        combobox_str.push_back(itemName);
    }

    auto iter=qFind(combobox_str.begin(),combobox_str.end(),str);
    if(iter!=combobox_str.end())
    {

    }else
    {
        ui->comboBox->addItem(str);
    }

}


void measurement_task::on_comboBox_currentIndexChanged(const QString &arg1)
{

    QString str;
    if(arg1==".")
    {
        ui->path_lineedit->setText(QDir::currentPath());

        path_name_=ui->path_lineedit->text();

        inputCharListWidget(ui->path_lineedit->text());
    }
    else
    {
        str =path_name_+"/"+arg1;
        ui->path_lineedit->setText(str);

        inputCharListWidget(ui->path_lineedit->text());
    }

}

void measurement_task::inputCharListWidget(const QString &str)
{

    qDebug()<<str;

    ui->listWidget->clear();
    QDir dir(str);
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Dirs);
    foreach (auto fileInfo, fileInfoList) {
        if(fileInfo.isDir())
        {
            int count_=fileInfo.absoluteFilePath().lastIndexOf('/');
            QString str_filename=fileInfo.absoluteFilePath().mid(count_+1);

            listwidget_str_.push_back(str_filename);

            ui->listWidget->addItem(str_filename);
        }
    }

}

void measurement_task::on_listWidget_currentTextChanged(const QString &currentText)
{


    ui->listWidget_2->clear();

    qDebug()<<currentText;

    pitch_on_listwidget_=currentText;

    ui->listWidget_2->addItem("(new)");
    QString path_= path_name_+"/"+currentText;

    ui->path_lineedit->setText(path_);

    QString str;
    QDir fileDir(path_);

    QFileInfoList list = fileDir.entryInfoList(QStringList() << "*.job");
    for (int i = 0; i < list.length(); i++)
    {
        qDebug() << list.at(i).baseName();
        ui->listWidget_2->addItem(list.at(i).baseName());
    }

}

void measurement_task::on_listWidget_2_currentTextChanged(const QString &currentText)
{
    if(currentText=="(new)")
    {
        QDateTime time = QDateTime::currentDateTime();
        QString str = time.toString("yyyyMM_dd hhmm");
        str=pitch_on_listwidget_+"_"+str;

        ui->file_lineEdit->setText(str);

        ui->path_lineedit->setText(path_name_+"/"+pitch_on_listwidget_+"/"+str+".job");

    }
    else
    {
        ui->file_lineEdit->setText(currentText);

        ui->path_lineedit->setText(path_name_+"/"+pitch_on_listwidget_+"/"+currentText+".job");

    }
}

//文件创建  并弹窗
void measurement_task::on_file_pashbutton_clicked()
{

    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("加载测量任务"),
                                                    "./",
                                                    tr("*.job;; Allfile(*.*)")); //选择路径
    if(filename.isEmpty())
    {
        return;
    }
    else
    {


    }
    ui->path_lineedit->setText(filename);
}


//ok  创建文件并弹窗
void measurement_task::on_pushButton_3_clicked()
{

    QString filename = ui->path_lineedit->text(); //选择路径
    if(filename.isEmpty())
    {
        return;
    }
    else
    {


        QString str=path_name_+"/"+pitch_on_listwidget_+".mwa";

        QSettings *file_settings=new QSettings(str,QSettings::IniFormat);

        QSettings *settings = new QSettings(/*"C://Users//gaohuan//Desktop//qt_waviness//01.mwa"*/filename,QSettings::IniFormat);
//        qDebug() << QCoreApplication::applicationDirPath();

        settings->setValue("Filter/FL0",file_settings->value("Filter/FL0").toString());

        settings->setValue("Filter/FL1",file_settings->value("Filter/FL1").toString());

        settings->setValue("Filter/FL2",file_settings->value("Filter/FL2").toString());

        settings->setValue("Filter/FL3",file_settings->value("Filter/FL3").toString());

        settings->setValue("Filter/FL4",file_settings->value("Filter/FL4").toString());


        settings->setValue("Filter/FL5",file_settings->value("Filter/FL5").toString());

        settings->setValue("Filter/FL6",file_settings->value("Filter/FL6").toString());

        settings->setValue("Filter/FL7",file_settings->value("Filter/FL7").toString());

        settings->setValue("Filter/FL8",file_settings->value("Filter/FL8").toString());

        settings->setValue("Filter/FL9",file_settings->value("Filter/FL9").toString());

        settings->setValue("Order/PartType",file_settings->value("Order/PartType").toString());
        settings->setValue("Order/PartName",file_settings->value("Order/PartName").toString());
        settings->setValue("Order/Number of pieces",file_settings->value("Order/Number of pieces").toString());
        settings->setValue("Order/Order",file_settings->value("Order/Order").toString());
        settings->setValue("Order/Customer",file_settings->value("Order/Customer").toString());
        settings->setValue("Order/Channel",file_settings->value("Order/Channel").toString());
        settings->setValue("Order/Operator",file_settings->value("Order/Operator").toString());
        settings->setValue("Order/Operator ID",file_settings->value("Order/Operator ID").toString());
        settings->setValue("Order/Factory",file_settings->value("Order/Factory").toString());
        settings->setValue("Order/Department",file_settings->value("Order/Department").toString());
        settings->setValue("Order/Machine",file_settings->value("Order/Machine").toString());
        settings->setValue("Order/Operation",file_settings->value("Order/Operation").toString());

        settings->setValue("AUTOSAVE/AutoSaveMode",file_settings->value("AUTOSAVE/AutoSaveMode").toString());
        settings->setValue("AUTOSAVE/AutoSaveFile",file_settings->value("AUTOSAVE/AutoSaveFile").toString());
        settings->setValue("AUTOSAVE/AddStatMode",file_settings->value("AUTOSAVE/AddStatMode").toString());


        settings->setValue("Measurement/MS",file_settings->value("Measurement/MS").toString());
        settings->setValue("Measurement/CalcWZTol",file_settings->value("Measurement/CalcWZTol").toString());
        settings->setValue("General/Version",file_settings->value("General/Version").toString());

        settings->setValue("CALIBRATION/MasterPartNr",file_settings->value("CALIBRATION/MasterPartNr").toString());
        settings->setValue("CALIBRATION/MasterPartValue",file_settings->value("CALIBRATION/MasterPartValue").toString());
        settings->setValue("CALIBRATION/SmallMasterPartLastTime",file_settings->value("CALIBRATION/SmallMasterPartLastTime").toString());
        settings->setValue("CALIBRATION/BigMasterPartLastTime",file_settings->value("CALIBRATION/BigMasterPartLastTime").toString());

        settings->beginGroup("CustomObjects");

        file_settings->beginGroup("CustomObjects");    // 设置查找节点
        QStringList str2 = file_settings->allKeys();    // 获取所有的key

        qDebug() <<"CustomObjects--  allKeys : " << str2 << endl;

        table_config table_str;
        QVector<table_config> vec_table;

        foreach(QString key,str2)
        {
             settings->beginGroup(key);

    //        QStringList list =key.split("/");

    //        QString value_begin_num = list[0];
            table_str.criterion_name=key;


            QString value  = file_settings->value(key).toString();  // 直接用 key 获取数据
            table_str.profile_data=value;
            qDebug() <<key << " : "<< value << endl;

            settings->setValue(key,value);
            vec_table.push_back(table_str);
            settings->endGroup();
        }


//        for(auto item:vec_table)
//        {
//            settings->beginGroup(item.criterion_name);

//            settings->setValue("lower_tolerance_limit",item.lower_tolerance_limit);
//            settings->setValue("lower_limit_of_warning",item.lower_limit_of_warning);
//            settings->setValue("upper_limit_of_warning",item.upper_limit_of_warning);
//            settings->setValue("reasonable_upper_limit",item.reasonable_upper_limit);

//            settings->endGroup();

//        }


        settings->endGroup();
        file_settings->endGroup();    // 设置查找节点

    }

    emit down_ok();

    QVector<QString> vec_str;
    QString str=ui->file_lineEdit->text();
    for(int i = 0; i < ui->listWidget_2->count();i++)
   {
        vec_str.push_back(ui->listWidget_2->item(i)->text());
   }

    auto item=qFind(vec_str.begin(),vec_str.end(),str);
    if(item!=vec_str.end())
    {
        emit signal_profile_file(filename);

        emit signal_profile_filename(filename);
    }


    this->hide();
}

void measurement_task::on_lineEdit_textEdited(const QString &arg1)
{
    qDebug()<<arg1;
    //ui->listWidget->clear();

    QVector<QString> vec_listwidget_;

    if(arg1=="")
    {
        vec_listwidget_=listwidget_str_;
    }else
    {

        for(int i=0;i<ui->listWidget->count();i++)
        {
            QString itemText = ui->listWidget->item(i)->text();
            if(itemText[0]==arg1)
            {
                vec_listwidget_.push_back(itemText);

            }
        }
    }
    ui->listWidget->clear();

    for(auto iter:vec_listwidget_)
    {
        ui->listWidget->addItem(iter);
    }


}

void measurement_task::on_listWidget_2_itemDoubleClicked(QListWidgetItem *item)
{
    qDebug()<<"item:"<<item->text();
    QString path=ui->path_lineedit->text();

}
