#include "configuration.h"
#include "ui_configuration.h"
#include <QDebug>
#include "formdoc.h"
#include <QMessageBox>
#include <QModelIndexList>
#include <QSettings>
#include <QFileDialog>
#include <QDateTime>

configuration::configuration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::configuration)
{
    ui->setupUi(this);

    /*std::shared_ptr<formdoc>*/ ptr_=std::make_shared<formdoc>(ui->tabWidget_2);

    connect(ptr_.get(),SIGNAL(signal_filter_formdoc_to_configuration(QVector<table_widget_band>)),this,SLOT(recive_filter_band_formoc(QVector<table_widget_band>)));
    formdoc_vector_.push_back(ptr_.get());

    ui->horizontalLayout_21->addWidget(ptr_.get());

    ui->tabWidget_2->setVisible(true);

}

configuration::~configuration()
{
    delete ui;
}

void configuration::on_tabWidget_2_tabCloseRequested(int index)
{
    ui->tabWidget_2->removeTab(index);
}

void configuration::on_add_pushButton_clicked()
{


    //        std::shared_ptr<formdoc> ptr_add=std::make_shared<formdoc>(ui->tabWidget_2);// 新建选项卡

    //        formdoc_vector_.push_back(ptr_add.get());

    //        int cur = ui->tabWidget_2->addTab(ptr_add.get(),QString::asprintf(" MS%d",ui->tabWidget_2->count()+1));

    //        ui->tabWidget_2->setCurrentIndex(cur);

    //        ui->tabWidget_2->setVisible(true);


    formdoc *formtable = new formdoc(ui->tabWidget_2);

    int cur = ui->tabWidget_2->addTab(formtable,QString::asprintf(" MS%d",ui->tabWidget_2->count()+1));
    ui->tabWidget_2->setCurrentIndex(cur);


}

void configuration::on_clear_pushButton_clicked()
{
    QMessageBox::StandardButton rb=QMessageBox::question(this,
                                                         tr("清除测量站"),
                                                         tr("清除测量站 MS%1 ").arg(ui->tabWidget_2->count()),
                                                         QMessageBox::Ok | QMessageBox::Cancel,
                                                         QMessageBox::Ok);
    if(rb== QMessageBox::Ok)
    {
        ui->tabWidget_2->removeTab(ui->tabWidget_2->count()-1);
    }

}

void configuration::on_copy_pushbutton_clicked()
{
    ui->paste_pushbutton->setEnabled(true);

    qDebug()<<ui->tabWidget_2->currentIndex();

    ptr_copy_=std::make_unique<formdoc>(ui->tabWidget_2);
    //    ptr_copy_.get()= formdoc_vector_[ui->tabWidget_2->currentIndex()];



    //    QString copied_text;
    //               QModelIndexList current_selected_indexs = ui->tabWidget_2->se->selectedIndexes();
    //               int current_row = current_selected_indexs.at(0).row();
    //               for(int i = 0; i < current_selected_indexs.count(); i++){
    //                   if(current_row != current_selected_indexs.at(i).row()){
    //                       current_row = current_selected_indexs.at(i).row();
    //                       copied_text.append("\n");
    //                       copied_text.append(current_selected_indexs.at(i).data().toString());
    //                       continue;
    //                   }
    //                   if(0 != i){
    //                       copied_text.append("\t");
    //                   }
    //                   copied_text.append(current_selected_indexs.at(i).data().toString());
    //               }
    //               copied_text.append("\n");
    //               QApplication::clipboard()->setText(copied_text);
    //               event->accept();

}

void configuration::on_paste_pushbutton_clicked()
{

    //    std::shared_ptr<formdoc> ptr_=std::make_unique<formdoc>(ui->tabWidget_2);
    //    formdoc_vector_.push_back(ptr_.get());
    formdoc *formtable = new formdoc(ui->tabWidget_2);

    int cur = ui->tabWidget_2->addTab(formtable,QString::asprintf(" MS%d",ui->tabWidget_2->count()+1));

    ui->tabWidget_2->setCurrentIndex(cur);

    ui->tabWidget_2->setVisible(true);
}



void configuration::on_cancel_pushButton_clicked()
{
    this->hide();
}

void configuration::on_tabWidget_2_tabBarClicked(int index)
{
    ui->tabWidget_2->setCurrentIndex(index);
}

void configuration::on_tabWidget_2_tabBarDoubleClicked(int index)
{
    ui->tabWidget_2->setCurrentIndex(index);
}

void configuration::on_confirm_pushButton_clicked()
{
    tab_str= ptr_.get()->getTableWidgetString();
    tab_band_=ptr_.get()->add_criteria_data_;
    emit signalResultAreaArribute(tab_str);

    recognition_config rec_config;

    rec_config.part_type=ui->part_type_lineEdit->text();
    rec_config.attribute=ui->attribute_lineEdit->text();
    rec_config.amount=ui->amount_lineEdit->text();
    rec_config.operate=ui->operate_lineEdit->text();
    rec_config.order=ui->order_lineEdit->text();
    rec_config.client=ui->client_lineEdit->text();
    rec_config.channel=ui->channel_lineEdit->text();
    rec_config.operator_=ui->operator_lineEdit->text();
    rec_config.operator_num=ui->operator_num_lineEdit->text();
    rec_config.factory=ui->factory_lineEdit->text();
    rec_config.branch=ui->branch_lineEdit->text();
    rec_config.machine=ui->machine_lineEdit->text();


    emit signal_recognition_config(rec_config);

    QSettings setting(configuration_filename_mwa_, QSettings::IniFormat);

    qDebug()<<"on_confirm_pushButton_clicked()configuration_filename_mwa_:"<<configuration_filename_mwa_<<"\n";

    QString value_0=ui->tableWidget->item(0,0)->text()+","+ui->tableWidget->item(0,1)->text()
            +","+ui->tableWidget->item(0,2)->text();
    setting.setValue("Filter/FL0",value_0);

    QString value_1=ui->tableWidget->item(1,0)->text()+","+ui->tableWidget->item(1,1)->text()
            +","+ui->tableWidget->item(1,2)->text();
    setting.setValue("Filter/FL1",value_1);

    QString value_2=ui->tableWidget->item(2,0)->text()+","+ui->tableWidget->item(2,1)->text()
            +","+ui->tableWidget->item(2,2)->text();
    setting.setValue("Filter/FL2",value_2);

    QString value_3=ui->tableWidget->item(3,0)->text()+","+ui->tableWidget->item(3,1)->text()
            +","+ui->tableWidget->item(3,2)->text();
    setting.setValue("Filter/FL3",value_3);

    QString value_4=ui->tableWidget->item(4,0)->text()+","+ui->tableWidget->item(4,1)->text()
            +","+ui->tableWidget->item(4,2)->text();
    setting.setValue("Filter/FL4",value_4);

    QString value_5=ui->tableWidget->item(5,0)->text()+","+ui->tableWidget->item(5,1)->text()
            +","+ui->tableWidget->item(5,2)->text();
    setting.setValue("Filter/FL5",value_5);

    QString value_6=ui->tableWidget->item(6,0)->text()+","+ui->tableWidget->item(6,1)->text()
            +","+ui->tableWidget->item(6,2)->text();
    setting.setValue("Filter/FL6",value_6);

    QString value_7=ui->tableWidget->item(7,0)->text()+","+ui->tableWidget->item(7,1)->text()
            +","+ui->tableWidget->item(7,2)->text();
    setting.setValue("Filter/FL7",value_7);

    QString value_8=ui->tableWidget->item(8,0)->text()+","+ui->tableWidget->item(8,1)->text()
            +","+ui->tableWidget->item(8,2)->text();
    setting.setValue("Filter/FL8",value_8);

    QString value_9=ui->tableWidget->item(9,0)->text()+","+ui->tableWidget->item(9,1)->text()
            +","+ui->tableWidget->item(9,2)->text();
    setting.setValue("Filter/FL9",value_9);

    setting.setValue("Order/PartType",rec_config.part_type);
    setting.setValue("Order/PartName",rec_config.attribute);
    setting.setValue("Order/Number of pieces",rec_config.amount);
    setting.setValue("Order/Order",rec_config.order);
    setting.setValue("Order/Customer",rec_config.client);
    setting.setValue("Order/Channel",rec_config.channel);
    setting.setValue("Order/Operator",rec_config.operator_);
    setting.setValue("Order/Operator ID",rec_config.operator_num);
    setting.setValue("Order/Factory",rec_config.factory);
    setting.setValue("Order/Department",rec_config.branch);
    setting.setValue("Order/Machine",rec_config.machine);
    setting.setValue("Order/Operation", rec_config.operate);

    setting.setValue("AUTOSAVE/AutoSaveMode","1");
    setting.setValue("AUTOSAVE/AutoSaveFile",ui->filepath_lineEdit->text());
    setting.setValue("AUTOSAVE/AddStatMode","1");

    setting.beginGroup("CustomObjects");
    for(auto item:tab_str)
    {
        setting.beginGroup(item.main_criteria+" "+item.secondary_criteria);

        setting.setValue("lower_tolerance_limit",item.lower_tolerance_limit);
        setting.setValue("lower_limit_of_warning",item.lower_limit_of_warning);
        setting.setValue("upper_limit_of_warning",item.upper_limit_of_warning);
        setting.setValue("reasonable_upper_limit",item.reasonable_upper_limit);

        setting.endGroup();

    }

    setting.endGroup();

    setting.sync();//写入配置文件

    this->hide();
}

void configuration::recive_filter_band_formoc(QVector<table_widget_band> band)
{

    for(auto item:band)
    {
        for(int i=0;i<ui->tableWidget->rowCount();i++)
        {
            if(item.header==ui->tableWidget->verticalHeaderItem(i)->text())
            {
                ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString::number(item.begin_num)));  //次要准则
                ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString::number(item.end_num)));  //次要准则

            }
        }
    }

}

void configuration::on_file_pushButton_clicked()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyyMM_dd");


    QStringList list = configuration_filename_mwa_.split(".");
    QString filename = list[0];

    str=filename+"_"+str;

    QString filename_mwa_ = QFileDialog::getSaveFileName(this,
                                                         tr("Save Image"),
                                                         str,
                                                         tr("*.mda")); //选择路径
    qDebug()<<filename_mwa_;
    if(filename_mwa_.isEmpty())
    {
        QMessageBox::critical(this, tr("提示消息框"), tr("文件名为空！"));
        //        return;
    }
    else
    {
        QSettings *settings = new QSettings(/*"C://Users//gaohuan//Desktop//qt_waviness//01.mwa"*/filename_mwa_,QSettings::IniFormat);
        qDebug() << QCoreApplication::applicationDirPath();

        settings->setValue("Order/PartType"," ");
        settings->setValue("Order/PartName"," ");
        settings->setValue("Order/Number of pieces"," ");
        settings->setValue("Order/Order"," ");
        settings->setValue("Order/Customer"," ");



    }
    ui->filepath_lineEdit->setText(str+".mda");


}

void configuration::open_mwa_profile(QString filename)
{

    configuration_filename_mwa_=filename;
    qDebug()<<"open_mwa_profile()configuration_filename_mwa_:"<<configuration_filename_mwa_<<"\n";

    QVector<QString> vec_value;
    QSettings *settings = new QSettings(/*"C://Users//gaohuan//Desktop//qt_waviness//01.mwa"*/filename,QSettings::IniFormat);

    QString value_0= settings->value("Filter/FL0").toString();
    vec_value.push_back(value_0);

    QString value_1= settings->value("Filter/FL1").toString();
    vec_value.push_back(value_1);

    QString value_2= settings->value("Filter/FL2").toString();
    vec_value.push_back(value_2);

    QString value_3= settings->value("Filter/FL3").toString();
    vec_value.push_back(value_3);

    QString value_4=settings->value("Filter/FL4").toString();
    vec_value.push_back(value_4);

    QString value_5=settings->value("Filter/FL5").toString();
    vec_value.push_back(value_5);

    QString value_6=settings->value("Filter/FL6").toString();
    vec_value.push_back(value_6);

    QString value_7=settings->value("Filter/FL7").toString();
    vec_value.push_back(value_7);

    QString value_8=settings->value("Filter/FL8").toString();
    vec_value.push_back(value_8);

    QString value_9= settings->value("Filter/FL9").toString();
    vec_value.push_back(value_9);

    for(int i=0;i<vec_value.size();i++)
    {
        QStringList list = vec_value[i].split(",");
        QString value_begin_num = list[0];
        QString value_end_num = list[1];
        QString value_speed_num = list[2];


        ui->tableWidget->setItem(i,0,new QTableWidgetItem(value_begin_num));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(value_end_num));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(value_speed_num));
    }


    QString str=settings->value("Order/PartType").toString();

    str=settings->value("Order/PartName").toString();

    str=settings->value("Order/Number of pieces").toString();
    ui->amount_lineEdit->setText(str);

    str=settings->value("Order/Order").toString();
    ui->order_lineEdit->setText(str);

    str=settings->value("Order/Customer").toString();
    ui->client_lineEdit->setText(str);
    str=settings->value("Order/Channel").toString();
    ui->channel_lineEdit->setText(str);

    str=settings->value("Order/Operator").toString();
    ui->operator_lineEdit->setText(str);

    str=settings->value("Order/Operator ID").toString();
    ui->operator_num_lineEdit->setText(str);

    str=settings->value("Order/Factory").toString();
    ui->factory_lineEdit->setText(str);

    str=settings->value("Order/Department").toString();
    ui->branch_lineEdit->setText(str);

    str=settings->value("Order/Machine").toString();
    ui->machine_lineEdit->setText(str);

    str=settings->value("Order/Operation").toString();
    ui->operate_lineEdit->setText(str);

    str=settings->value("AUTOSAVE/AutoSaveMode").toString();
    if(str=="0")
    {
        ui->radioButton->click();
    }else if(str=="1")
    {
        ui->radioButton_3->click();
    }
    str=settings->value("AUTOSAVE/AutoSaveFile").toString();
    ui->filepath_lineEdit->setText(str);
    str=settings->value("AUTOSAVE/AddStatMode").toString();

    if(str=="0")
    {
        ui->radioButton_4->click();

    }else if(str=="1")
    {
        ui->radioButton_5->click();
    }

    settings->beginGroup("CustomObjects");    // 设置查找节点
    QStringList str2 = settings->allKeys();    // 获取所有的key

    qDebug() <<"CustomObjects--  allKeys : " << str2 << endl;



    table_config table_str;
    QVector<table_config> vec_table;

    foreach(QString key,str2)
    {

//        QStringList list =key.split("/");

//        QString value_begin_num = list[0];
        table_str.criterion_name=key;


        QString value  = settings->value(key).toString();  // 直接用 key 获取数据
        table_str.profile_data=value;
        qDebug() <<key << " : "<< value << endl;

        vec_table.push_back(table_str);
    }
     ptr_->write_tablewidget(vec_table);
    settings->endGroup();   // 结束掉Group

    ui->confirm_pushButton->click();


}

