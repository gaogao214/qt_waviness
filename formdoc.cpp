#include "formdoc.h"
#include "ui_formdoc.h"
#include <QVBoxLayout>
#include <QDebug>

formdoc::formdoc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::formdoc)
{
    ui->setupUi(this);

    //水平表头背景显示
    ui->tableWidget_2->horizontalHeader()->setStyleSheet("QHeaderView::section{background: rgb(236, 236, 236);}");

    //垂直表头背景显示
    ui->tableWidget_2->verticalHeader()->setStyleSheet("QHeaderView::section{background: rgb(236, 236, 236);}");

    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);//整行选中的方式
//    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止修改
    ui->tableWidget_2->setSelectionMode(QAbstractItemView::SingleSelection);//可以选中单个

}

formdoc::~formdoc()
{
    delete ui;
}

void formdoc::table_widget(QString str)
{

    auto item=std::find(table_string.begin(),table_string.end(),str);
    if(item==table_string.end())
    {
        int RowCont;
        RowCont=ui->tableWidget_2->rowCount();
        ui->tableWidget_2->insertRow(RowCont);//增加一行

        //插入元素
        ui->tableWidget_2->setItem(RowCont,0,new QTableWidgetItem(str));
        ui->tableWidget_2->setItem(RowCont,1,new QTableWidgetItem("0.0000000"));
        ui->tableWidget_2->setItem(RowCont,2,new QTableWidgetItem("0.0000000"));
        ui->tableWidget_2->setItem(RowCont,3,new QTableWidgetItem("0.0000000"));
        ui->tableWidget_2->setItem(RowCont,4,new QTableWidgetItem("0.0000000"));
        ui->tableWidget_2->setItem(RowCont,5,new QTableWidgetItem("0.0000000"));

        table_string.push_back(str);
    }

    add_criteria_data_=add_criteria_->band_;

}

void formdoc::recive_filter_band(QVector<table_widget_band> band)
{
    emit signal_filter_formdoc_to_configuration(band);
}

QVector<table_config> formdoc::getTableWidgetString()
{
    QVector<table_config> vec_config;
    QVector<table_config> vec_config_send_result;
    QVector<QString> vec_string;
    table_config config;
    int count=0;

    for(int row=0;row<ui->tableWidget_2->rowCount();row++)
    {

        QStringList list1 = ui->tableWidget_2->item(row,0)->text().split(" ");

        for (int i = 0; i < list1.size(); i++)
        {
            qDebug() << list1[i];
            config.main_criteria=list1[0];
            config.secondary_criteria=list1[1];
            if(list1.size()>2)
            {
                 config.Harm_count=list1[2];
            }

        }

        config.lower_tolerance_limit= ui->tableWidget_2->item(row,1)->text();
        config.lower_limit_of_warning= ui->tableWidget_2->item(row,2)->text();
        config.upper_limit_of_warning= ui->tableWidget_2->item(row,3)->text();
        config.reasonable_upper_limit= ui->tableWidget_2->item(row,4)->text();

        vec_config.push_back(config);
        vec_string.push_back(config.main_criteria);
    }



    std::sort(vec_config.begin(),vec_config.end(),compare);


    return  vec_config;
}

bool formdoc::compare(const table_config &s1, const table_config &s2)
{
    return  s1.main_criteria< s2.main_criteria;
}


void formdoc::on_add_pushbutton_clicked()
{

    if(add_criteria_==nullptr)
    {
        add_criteria_=std::make_unique<add_criteria>();
        connect(add_criteria_.get(),SIGNAL(signal_table_widget(QString)),this,SLOT(table_widget(QString)));
        connect(add_criteria_.get(),SIGNAL(signal_filter_band(QVector<table_widget_band>)),this,SLOT(recive_filter_band(QVector<table_widget_band>)));
        add_criteria_->setWindowModality(Qt::ApplicationModal); //顶层模态
        add_criteria_->show();
    }
    else
    {
        add_criteria_->show();
    }
}

void formdoc::on_tableWidget_2_cellClicked(int row, int column)
{
      row_=row;
      column_=column;
}

void formdoc::on_clear_row_pushButton_clicked()
{
    ui->tableWidget_2->removeRow(row_);

    table_string.erase(table_string.begin()+row_);

}

void formdoc::on_change_pushButton_clicked()
{
    if(add_criteria_==nullptr)
    {
        add_criteria_=std::make_unique<add_criteria>();
        connect(add_criteria_.get(),SIGNAL(signal_table_widget(QString)),this,SLOT(table_widget(QString)));
        add_criteria_->setWindowModality(Qt::ApplicationModal); //顶层模态
        add_criteria_->show();
    }
    else
    {
        add_criteria_->show();
    }

    ui->tableWidget_2->removeRow(row_);

    //插入元素
    ui->tableWidget_2->setItem(row_,0,new QTableWidgetItem(add_criteria_->criteria_for_selection_));
    ui->tableWidget_2->setItem(row_,1,new QTableWidgetItem("0.0000"));
    ui->tableWidget_2->setItem(row_,2,new QTableWidgetItem("0.0000"));
    ui->tableWidget_2->setItem(row_,3,new QTableWidgetItem("0.0000"));
    ui->tableWidget_2->setItem(row_,4,new QTableWidgetItem("0.0000"));
    ui->tableWidget_2->setItem(row_,5,new QTableWidgetItem("0.0000"));
}

 void formdoc::write_tablewidget(QVector<table_config> str_list)
 {

     for(int row = ui->tableWidget_2->rowCount() - 1;row >= 0; row--)
     {
         ui->tableWidget_2->removeRow(row);
     }

     int RowCont;

    for(auto item:str_list)
    {


        auto str=item.criterion_name;
        QStringList list =str.split("/");

        QString main_second_criterion = list[0];
        QString str_type=list[1];

        auto item_=std::find(table_string.begin(),table_string.end(),main_second_criterion);
        if(item_==table_string.end())
        {

            RowCont=ui->tableWidget_2->rowCount();
            ui->tableWidget_2->insertRow(RowCont);//增加一行
            ui->tableWidget_2->setItem(RowCont,0,new QTableWidgetItem(main_second_criterion));

            table_string.push_back(main_second_criterion);

        } if(str_type=="lower_tolerance_limit")
        {
            ui->tableWidget_2->setItem(RowCont,1,new QTableWidgetItem(item.profile_data));
        } else if(str_type=="lower_limit_of_warning")
        {
            ui->tableWidget_2->setItem(RowCont,2,new QTableWidgetItem(item.profile_data));
        }else if(str_type=="upper_limit_of_warning")
        {
             ui->tableWidget_2->setItem(RowCont,3,new QTableWidgetItem(item.profile_data));
        }else if(str_type=="reasonable_upper_limit")
        {
            ui->tableWidget_2->setItem(RowCont,4,new QTableWidgetItem(item.profile_data));
        }
    }

 }
