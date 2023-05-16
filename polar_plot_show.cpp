#include "polar_plot_show.h"
#include "ui_polar_plot_show.h"

#include <cmath>
#include <sstream>
#include <complex>
#include "qcustomplot.h"
#include "mouse_release_event.h"
#include "result_area_attribute.h"

#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <qwt_plot.h>

#include <algorithm>

#include <qwt_plot_renderer.h>
#include <qwt_plot_canvas.h>

#include <qwt_column_symbol.h>
#include <qwt_plot_layout.h>
#include <qwt_legend.h>
#include <qwt_scale_draw.h>
#include "histogram.h"
#include "get_polar_data.h"

#include <qwt_polar_magnifier.h>


PolarPlotShow::PolarPlotShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::polar_plot_show)
{
    ui->setupUi(this);


    //    ui->histogram_->xAxis->setRange(0, 2048);//x ,y 的坐标
    //    ui->histogram_->yAxis->setRange(0.000,0.05);

    ui->curve_graph_->xAxis->setRange(0, 360);//x ,y 的坐标
    ui->curve_graph_->yAxis->setRange(-2, 15);

    //    ui->waviness->xAxis->setRange(0, 360);//x ,y 的坐标
    //    ui->waviness->yAxis->setRange(-2, 15);

    //    ui->histogram_->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);  //用户可以通过拖拽坐标轴和滑动滚轮的方式来缩放图像

    //    ui->histogram_->hide();
    //    ui->waviness->hide();
    //    ui->circle_graph->hide();
    ui->curve_graph_->hide();
    //    waviness_chart_.get()->hide();

    ui->qwtPlot->hide();

    wavinessChart();
    waviness_chart_.get()->hide();

    //qwt极坐标图
        ui->circle_graph->hide();
//        polar_diagram();




    barChart();

    //各个子菜单显示
    poleDiagramDisplayMenu();
    areaOfRemarksMenu();
    resultsTheMenu();
    identifyTheMenu();
    curveGraphMenu();
    histogramMenu();

    startToPrepare();

    QTimer *timer_calendar;
    timer_calendar=new QTimer(this);
    connect(timer_calendar,SIGNAL(timeout()),this,SLOT(show_time()));
    timer_calendar->start(1000);


    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//整行选中的方式
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止修改
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);//可以选中单个

}

PolarPlotShow::~PolarPlotShow()
{
    delete ui;
}



//结果区域显示
void PolarPlotShow::reciveResultAreaAttribute_(QVector<table_config> tab_config)
{

    formdoc_tablewidget_data_=tab_config;

    for(int row = ui->tableWidget->rowCount() - 1;row >= 0; row--)
    {
        ui->tableWidget->removeRow(row);
    }
    QVector<QString> tab_string_main;

    for(int i=0;i<tab_config.size();i++)
    {

        int RowCont=ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(RowCont);//增加一行

        auto item=qFind(tab_string_main.begin(),tab_string_main.end(),tab_config[i].main_criteria);
        if(item==tab_string_main.end())
        {


            ui->tableWidget->setItem(RowCont,0,new QTableWidgetItem(tab_config[i].main_criteria));       //主要准则

        }
        if(tab_config[i].secondary_criteria=="fln1" ||tab_config[i].secondary_criteria=="△r"
                ||tab_config[i].secondary_criteria=="Harm" ||tab_config[i].secondary_criteria=="max"
                ||tab_config[i].secondary_criteria=="Rms" ||tab_config[i].secondary_criteria=="max1"
                ||tab_config[i].secondary_criteria=="max2" ||tab_config[i].secondary_criteria=="max3")
        {

            ui->tableWidget->setItem(RowCont,3,new QTableWidgetItem("μm"));
        }
        else if(tab_config[i].secondary_criteria=="L" ||tab_config[i].secondary_criteria=="M"
                ||tab_config[i].secondary_criteria=="H"||tab_config[i].secondary_criteria=="wMax"
                ||tab_config[i].secondary_criteria=="W2"||tab_config[i].secondary_criteria=="W3"
                ||tab_config[i].secondary_criteria=="W4"||tab_config[i].secondary_criteria=="W5"
                ||tab_config[i].secondary_criteria=="W6"||tab_config[i].secondary_criteria=="W7"
                ||tab_config[i].secondary_criteria=="W8"||tab_config[i].secondary_criteria=="W9"
                ||tab_config[i].secondary_criteria=="wRms" ||tab_config[i].secondary_criteria=="M1"
                ||tab_config[i].secondary_criteria=="M2")
        {

            ui->tableWidget->setItem(RowCont,3,new QTableWidgetItem("μm/s"));                       //单位
        }


        ui->tableWidget->setItem(RowCont,1,new QTableWidgetItem(tab_config[i].secondary_criteria+" "+tab_config[i].Harm_count));  //次要准则

        ui->tableWidget->setItem(RowCont,2,new QTableWidgetItem("0.0000000"));                       //结果
        //        ui->tableWidget->setItem(RowCont,3,new QTableWidgetItem("0.0000000"));                       //单位
        ui->tableWidget->setItem(RowCont,4,new QTableWidgetItem(tab_config[i].reasonable_upper_limit));                       //限值
        //    ui->tableWidget->setItem(RowCont,5,new QTableWidgetItem("0.0000000"));                 //公差条
        /* QProgressBar* */progressBar =new QProgressBar(ui->tableWidget);

        ui->tableWidget->setColumnWidth(5,200);
        progressBar->setMaximumHeight(25);
        //        progressBar->setMaximum(0);

        ui->tableWidget->setCellWidget(RowCont,5,progressBar);
        tab_string_main.push_back(tab_config[i].main_criteria);




    }


    ui->tableWidget->viewport()->update();

}

//识别区域
void PolarPlotShow::recive_recogntion_config(recognition_config config)
{
    ui->part_type_label->setText(config.part_type);
    ui->attribute_label->setText(config.attribute);
    ui->number_label->setText(config.amount);
    ui->operate_label->setText(config.operate);
    ui->order_label->setText(config.order);
    ui->client_label->setText(config.client);
    ui->channel_label->setText(config.channel);
    ui->operator_label->setText(config.operator_);
    ui->operator_num_label->setText(config.operator_num);
    ui->factory_label->setText(config.factory);
    ui->branch_label->setText(config.branch);
    ui->machine_label->setText(config.machine);
    widget_config_=config;

}

//根据计算显示结果区域
void PolarPlotShow::theResultsWereCalculatedAccordingToThePrimaryAndSecondaryCriteria(QVector<table_widget_band>& data)
{
    fft_. calculateMax1Max2Max3(amplitude_);
    vec_tab_config.clear();

    table_config tab_config_;
    if(ui->tableWidget->rowCount()!=0)
    {
        for(int i=0;i<ui->tableWidget->rowCount();i++)
        {

            if(ui->tableWidget->item(i,0)==nullptr || ui->tableWidget->item(i,0)->text().isEmpty())
            {

            }
            else if(ui->tableWidget->item(i,0)!=nullptr|| !ui->tableWidget->item(i,0)->text().isEmpty())
            {
                tab_config_.main_criteria=ui->tableWidget->item(i,0)->text();
            }

            tab_config_.secondary_criteria=ui->tableWidget->item(i,1)->text();

            tab_config_.reasonable_upper_limit=ui->tableWidget->item(i,4)->text();

            QStringList list1 =tab_config_.secondary_criteria.split(" ");
            for (int i = 0; i < list1.size(); i++)
            {
                qDebug() << list1[i];
                tab_config_.secondary_criteria=list1[0];
                tab_config_.Harm_count=list1[1];
            }

            if(tab_config_.main_criteria=="FFT")
            {
                if(tab_config_.secondary_criteria=="max1")
                {
                    tab_config_.result=QString::number(fft_.max1_,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));
                    //                    ui->tableWidget->item(i,3)->setText(QString("%1").arg("μm"));
                }
                else if(tab_config_.secondary_criteria=="max2")
                {
                    tab_config_.result=QString::number(fft_.max2_,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));
                    //                    ui->tableWidget->item(i,3)->setText(QString("%1").arg("μm"));
                }
                else if(tab_config_.secondary_criteria=="max3")
                {
                    tab_config_.result=QString::number(fft_.max3_,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));
                    //                    ui->tableWidget->item(i,3)->setText(QString("%1").arg("μm"));
                }else if(tab_config_.secondary_criteria=="Harm")
                {
                    double value_=fft_.amplitude_[tab_config_.Harm_count.toDouble()];
                    tab_config_.result=QString::number(value_,'f',3);

                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));
                    //                    ui->tableWidget->item(i,3)->setText(QString("%1").arg("μm"));
                }
            }

            if(tab_config_.main_criteria=="LSC")
            {
                if(tab_config_.secondary_criteria=="△r")
                {
                    auto difference_of_radius_=(num_max_-num_min_)/*-(fft_.tempPoint_.radius-num_min_)*/;
                    tab_config_.result=QString::number(difference_of_radius_,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));//写入
                    //                    ui->tableWidget->item(i,3)->setText(QString("%1").arg("μm"));
                }
                if(tab_config_.secondary_criteria=="fln1")
                {
                    auto difference_of_radius_=(num_max_-fft_.tempPoint_.radius)+(fft_.tempPoint_.radius-num_min_);
                    tab_config_.result=QString::number(difference_of_radius_,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));//写入
                    //                    ui->tableWidget->item(i,3)->setText(QString("%1").arg("μm"));
                }

            }


            QVector<double> gaussian_data;
            QVector<double> dft_data;
            QVector<double> waviness_data;
            double  sum_pow=0;
            double waviness_sum=0;

            if(tab_config_.main_criteria=="MDw")
            {
                if(tab_config_.secondary_criteria=="L")
                {
                    double waviness_L_sum;
                    gaussian_data=fft_.filterBand(2,4);
                    dft_data=fft_.dft(gaussian_data,gaussian_data.size());

                    for(int i=0;i<dft_data.size();i++)
                    {
                        auto w=1.4142 * 3.1416 * (rotate_speed/60) * dft_data[i]*i ; //根号2 * pi * 转速/60 *谐波*波次amplitude_;
                        auto pow_w=pow(w,2);
                        waviness_L_sum+=w;
                    }
                   auto waviness_sqrt=sqrt(waviness_L_sum/dft_data.size());
                    auto w2=1.4142 * 3.1416 * (rotate_speed/60) * amplitude_[2]*2 ; //根号2 * pi * 转速/60 *谐波*波次amplitude_;
                    tab_config_.result=QString::number(w2,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));//写入
                    //                    ui->tableWidget->item(i,3)->setText(QString("%1").arg("μm"));
                }
                if(tab_config_.secondary_criteria=="M")
                {
                    double waviness_M_sum;
                    gaussian_data=fft_.filterBand(5,25);
                    dft_data=fft_.dft(gaussian_data,gaussian_data.size());

                    for(int i=0;i<dft_data.size();i++)
                    {
                        auto w=1.4142 * 3.1416 * (rotate_speed/60) * dft_data[i]*i ; //根号2 * pi * 转速/60 *谐波*波次amplitude_;
                        auto pow_w=pow(w,2);
                        waviness_M_sum+=w;
                    }
                   auto waviness_sqrt=sqrt(waviness_M_sum/dft_data.size());

                    tab_config_.result=QString::number(waviness_sqrt,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));//写入
                    //                    ui->tableWidget->item(i,3)->setText(QString("%1").arg("μm"));
                }
                if(tab_config_.secondary_criteria=="M1")
                {
                    double waviness_M1_sum;
                    gaussian_data=fft_.filterBand(5,8);
                    dft_data=fft_.dft(gaussian_data,gaussian_data.size());

                    for(int i=0;i<dft_data.size();i++)
                    {
                        auto w=1.4142 * 3.1416 * (rotate_speed/60) * dft_data[i]*i ; //根号2 * pi * 转速/60 *谐波*波次amplitude_;
                        auto pow_w=pow(w,2);
                        waviness_M1_sum+=w;
                    }
                   auto waviness_sqrt=sqrt(waviness_M1_sum/dft_data.size());

                    tab_config_.result=QString::number(waviness_sqrt,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));//写入
                    //                    ui->tableWidget->item(i,3)->setText(QString("%1").arg("μm"));
                }
                if(tab_config_.secondary_criteria=="M2")
                {
                    double waviness_M2_sum;
                    gaussian_data=fft_.filterBand(9,25);
                    dft_data=fft_.dft(gaussian_data,gaussian_data.size());

                    for(int i=0;i<dft_data.size();i++)
                    {
                        auto w=1.4142 * 3.1416 * (rotate_speed/60) * dft_data[i]*i ; //根号2 * pi * 转速/60 *谐波*波次amplitude_;
                        auto pow_w=pow(w,2);
                        waviness_M2_sum+=w;
                    }
                   auto waviness_sqrt=sqrt(waviness_M2_sum/dft_data.size());

                    tab_config_.result=QString::number(waviness_sqrt,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));//写入
                    //                    ui->tableWidget->item(i,3)->setText(QString("%1").arg("μm"));
                }
                if(tab_config_.secondary_criteria=="H")
                {
                    double waviness_H_sum;
                    gaussian_data=fft_.filterBand(26,154);
                    dft_data=fft_.dft(gaussian_data,gaussian_data.size());

                    for(int i=0;i<dft_data.size();i++)
                    {
                        auto w=1.4142 * 3.1416 * (rotate_speed/60) * dft_data[i]*i ; //根号2 * pi * 转速/60 *谐波*波次amplitude_;
                        auto pow_w=pow(w,2);
                        waviness_H_sum+=w;
                    }
                   auto waviness_sqrt=sqrt(waviness_H_sum/dft_data.size());

                    tab_config_.result=QString::number(waviness_sqrt,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));//写入
                    //                    ui->tableWidget->item(i,3)->setText(QString("%1").arg("μm"));
                }
                if(tab_config_.secondary_criteria=="W2")
                {
                    auto w2=1.4142 * 3.1416 * (rotate_speed/60) * amplitude_[2]*2 ; //根号2 * pi * 转速/60 *谐波*波次amplitude_;
                    tab_config_.result=QString::number(w2,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));//写入
                    //                    ui->tableWidget->item(i,3)->setText(QString("%1").arg("μm"));
                }
                if(tab_config_.secondary_criteria=="W3")
                {
                    auto w3=1.4142 * 3.1416 * (rotate_speed/60) * amplitude_[3]*3 ; //根号2 * pi * 转速/60 *谐波*波次amplitude_;

                    tab_config_.result=QString::number(w3,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));//写入
                    //                    ui->tableWidget->item(i,3)->setText(QString("%1").arg("μm"));
                }
                if(tab_config_.secondary_criteria=="W4")
                {
                    auto w4=1.4142 * 3.1416 * (rotate_speed/60) * amplitude_[4]*4 ; //根号2 * pi * 转速/60 *谐波*波次amplitude_;

                    tab_config_.result=QString::number(w4,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));//写入
                    //                    ui->tableWidget->item(i,3)->setText(QString("%1").arg("μm"));
                }
                if(tab_config_.secondary_criteria=="W5")
                {
                    auto w5=1.4142 * 3.1416 * (rotate_speed/60) * amplitude_[5]*5 ; //根号2 * pi * 转速/60 *谐波*波次amplitude_;

                    tab_config_.result=QString::number(w5,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));//写入
                    //                    ui->tableWidget->item(i,3)->setText(QString("%1").arg("μm"));
                }
                if(tab_config_.secondary_criteria=="W6")
                {
                    auto w6=1.4142 * 3.1416 * (rotate_speed/60) * amplitude_[6]*6 ; //根号2 * pi * 转速/60 *谐波*波次amplitude_;

                    tab_config_.result=QString::number(w6,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));//写入
                    //                    ui->tableWidget->item(i,3)->setText(QString("%1").arg("μm"));
                }
                if(tab_config_.secondary_criteria=="W7")
                {
                    auto w7=1.4142 * 3.1416 * (rotate_speed/60) * amplitude_[7]*7 ; //根号2 * pi * 转速/60 *谐波*波次amplitude_;

                    tab_config_.result=QString::number(w7,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));//写入
                    //                    ui->tableWidget->item(i,3)->setText(QString("%1").arg("μm"));
                }
                if(tab_config_.secondary_criteria=="W8")
                {
                    auto w8=1.4142 * 3.1416 * (rotate_speed/60) * amplitude_[8]*8 ; //根号2 * pi * 转速/60 *谐波*波次amplitude_;

                    tab_config_.result=QString::number(w8,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));//写入
                    //                    ui->tableWidget->item(i,3)->setText(QString("%1").arg("μm"));
                }
                if(tab_config_.secondary_criteria=="W9")
                {
                    auto w9=1.4142 * 3.1416 * (rotate_speed/60) * amplitude_[9]*9 ; //根号2 * pi * 转速/60 *谐波*波次amplitude_;

                    tab_config_.result=QString::number(w9,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));//写入
                    //                    ui->tableWidget->item(i,3)->setText(QString("%1").arg("μm"));
                }

            }


            if(tab_config_.main_criteria=="MRS")
            {
                if(tab_config_.secondary_criteria=="W2")
                {
                    auto w2=1.4142 * 3.1416 * (rotate_speed/60) * amplitude_[2]*2 ; //根号2 * pi * 转速/60 *谐波*波次amplitude_;
                    tab_config_.result=QString::number(w2,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));//写入
                    //                    ui->tableWidget->item(i,3)->setText(QString("%1").arg("μm"));
                }
                if(tab_config_.secondary_criteria=="W3")
                {
                    auto w3=1.4142 * 3.1416 * (rotate_speed/60) * amplitude_[3]*3 ; //根号2 * pi * 转速/60 *谐波*波次amplitude_;

                    tab_config_.result=QString::number(w3,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));//写入
                    //                    ui->tableWidget->item(i,3)->setText(QString("%1").arg("μm"));
                }
                if(tab_config_.secondary_criteria=="W4")
                {
                    auto w4=1.4142 * 3.1416 * (rotate_speed/60) * amplitude_[4]*4 ; //根号2 * pi * 转速/60 *谐波*波次amplitude_;

                    tab_config_.result=QString::number(w4,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));//写入
                    //                    ui->tableWidget->item(i,3)->setText(QString("%1").arg("μm"));
                }
                if(tab_config_.secondary_criteria=="W5")
                {
                    auto w5=1.4142 * 3.1416 * (rotate_speed/60) * amplitude_[5]*5 ; //根号2 * pi * 转速/60 *谐波*波次amplitude_;

                    tab_config_.result=QString::number(w5,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));//写入
                    //                    ui->tableWidget->item(i,3)->setText(QString("%1").arg("μm"));
                }
                if(tab_config_.secondary_criteria=="W6")
                {
                    auto w6=1.4142 * 3.1416 * (rotate_speed/60) * amplitude_[6]*6 ; //根号2 * pi * 转速/60 *谐波*波次amplitude_;

                    tab_config_.result=QString::number(w6,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));//写入
                    //                    ui->tableWidget->item(i,3)->setText(QString("%1").arg("μm"));
                }
                if(tab_config_.secondary_criteria=="W7")
                {
                    auto w7=1.4142 * 3.1416 * (rotate_speed/60) * amplitude_[7]*7 ; //根号2 * pi * 转速/60 *谐波*波次amplitude_;

                    tab_config_.result=QString::number(w7,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));//写入
                    //                    ui->tableWidget->item(i,3)->setText(QString("%1").arg("μm"));
                }
                if(tab_config_.secondary_criteria=="W8")
                {
                    auto w8=1.4142 * 3.1416 * (rotate_speed/60) * amplitude_[8]*8 ; //根号2 * pi * 转速/60 *谐波*波次amplitude_;

                    tab_config_.result=QString::number(w8,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));//写入
                    //                    ui->tableWidget->item(i,3)->setText(QString("%1").arg("μm"));
                }
                if(tab_config_.secondary_criteria=="W9")
                {
                    auto w9=1.4142 * 3.1416 * (rotate_speed/60) * amplitude_[9]*9 ; //根号2 * pi * 转速/60 *谐波*波次amplitude_;

                    tab_config_.result=QString::number(w9,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));//写入
                    //                    ui->tableWidget->item(i,3)->setText(QString("%1").arg("μm"));
                }

            }



            auto item_FL=tab_config_.main_criteria.mid(4,3);
            if(item_FL=="FL0"||item_FL=="FL1"||item_FL=="FL2"||item_FL=="FL3"||item_FL=="FL4"||item_FL=="FL5"
                    ||item_FL=="FL6"||item_FL=="FL7"||item_FL=="FL8"||item_FL=="FL9")
            {
                for(auto item:data)
                {
                    if(item.header==item_FL)
                    {
                        gaussian_data=fft_.filterBand(item.begin_num,item.end_num);
                        dft_data=fft_.dft(gaussian_data,gaussian_data.size());
                    }
                }

                 auto max_=*(std::max_element(dft_data.begin(),dft_data.end()));
                 auto min_=*(std::min_element(dft_data.begin(),dft_data.end()));

                if(tab_config_.secondary_criteria=="△r")
                {
                    auto difference_of_radius_=(max_-fft_.tempPoint_.radius)+(fft_.tempPoint_.radius-min_);
                    tab_config_.result=QString::number(difference_of_radius_,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));//写入
                }
                else if(tab_config_.secondary_criteria=="max")
                {
                    auto max_=*(std::max_element(dft_data.begin(),dft_data.end()));

                    tab_config_.result=QString::number(max_,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));//写入
                }
                else if(tab_config_.secondary_criteria=="Rms")
                {
                    for(int i=0;i<dft_data.size();i++)
                    {
                        auto pow_=pow(dft_data[i],2);
                        sum_pow+=pow_;
                    }
                    auto rms=sqrt(sum_pow/dft_data.size());

                    tab_config_.result=QString::number(rms,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));
                }
                else if(tab_config_.secondary_criteria=="wRms")
                {
                    for(int i=0;i<dft_data.size();i++)
                    {
                        auto w=1.4142 * 3.1416 * (rotate_speed/60) * dft_data[i]*i ; //根号2 * pi * 转速/60 *谐波*波次amplitude_;
                        auto pow_w=pow(w,2);
                        waviness_sum+=w;
                    }
                   auto waviness_sqrt=sqrt(waviness_sum/dft_data.size());

                   tab_config_.result=QString::number(waviness_sqrt,'f',3);
                   ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));

                }
                else if(tab_config_.secondary_criteria=="wMax")
                {
                    for(int i=0;i<dft_data.size();i++)
                    {
                        auto w=1.4142 * 3.1416 * (rotate_speed/60) * dft_data[i]*i ; //根号2 * pi * 转速/60 *谐波*波次amplitude_;
                        waviness_data.push_back(w);
                    }
                    auto max_=*(std::max_element(waviness_data.begin(),waviness_data.end()));

                    tab_config_.result=QString::number(max_,'f',3);
                    ui->tableWidget->item(i,2)->setText(QString("%1").arg(tab_config_.result));//写入
                }
            }

            QProgressBar* progressBar=new QProgressBar();

            auto upper_limit_num=ui->tableWidget->item(i,4)->text();
            auto upper_waring=formdoc_tablewidget_data_[i].upper_limit_of_warning.toFloat();
            int percentage=float(tab_config_.result.toFloat()/upper_limit_num.toFloat())*100;

            progressBar->setMinimum(0);
            progressBar->setMaximum(100);

            if(tab_config_.result.toFloat()<upper_waring)
            {
                progressBar->setValue(percentage);
                progressBar->setTextVisible(false);
                progressBar->setStyleSheet("QProgressBar::chunk{background:  rgb(42, 121, 42)}"); //绿色
            }
            else if(tab_config_.result.toFloat()>upper_waring && tab_config_.result.toFloat()<upper_limit_num.toFloat())
            {
                progressBar->setValue(percentage);
                progressBar->setTextVisible(false);
                progressBar->setStyleSheet("QProgressBar::chunk{background:   rgb(229, 232, 31)}"); //黄色
            }
            else if(tab_config_.result.toFloat()>upper_limit_num.toFloat())
            {
                progressBar->setValue(100);
                progressBar->setTextVisible(false);
                progressBar->setStyleSheet("QProgressBar::chunk{background: rgb(179, 8, 34)}");
            }
            ui->tableWidget->setCellWidget(i,5,progressBar);
            vec_tab_config.push_back(tab_config_);

        }

    }

}

void PolarPlotShow::maindowThreadData(QVector<double> displacement_data,QString range)
{

    qDebug("polar polt show 已接收到\n");
    qDebug()<<displacement_data.size();

//    displacement_data.clear();

//    std::ifstream ifs;							//创建流对象

//    ifs.open("C:\\Users\\gaohuan\\Desktop\\7200.txt", std::ios::in);	//打开文件

//    if (!ifs.is_open())						//判断文件是否打开
//    {
//        qDebug() << "打开文件失败！！！";
//    }

//    std::vector<std::string> item;		//用于存放文件中的一行数据

//    std::string temp;				//临时存储文件中的一行数据

//    while (std::getline(ifs,temp))  //利用 getline（）读取文件中的每一行，并放入到 item 中
//    {
//        item.push_back(temp);
//    }
//    float lat;

//    QVector<float> vec;

//    //遍历文件中的每一行数据
//    for (auto it = item.begin(); it != item.end(); it++)
//    {
//        QString str=QString::fromStdString(*it);

//        lat = std::stod(*it);
//        str=QString::number(lat,'f',4);
//        lat=str.toFloat();


//        displacement_data.push_back(lat);
//    }

    //    /*清屏重绘*/
//        ui->qwtPlot->detachItems();
//        ui->qwtPlot->replot();
    int len=displacement_data.size();

    amplitude_=fft_.dft(displacement_data,len);

    fft_.gaussianBand(displacement_data);

    num_min_=*(std::min_element(displacement_data.begin(),displacement_data.end()));
    num_max_=*(std::max_element(displacement_data.begin(),displacement_data.end()));

    qDebug()<<"min:"<<std::to_string(num_min_).c_str();
    qDebug()<<"max:"<<std::to_string(num_max_).c_str();

    polar_diagram(range);
    histogramShow(amplitude_);
    waviness(amplitude_);
    poleDiagramDisplay(displacement_data,range);

    QwtPolarMagnifier* d_zoomer = new QwtPolarMagnifier(plot->canvas());

    d_zoomer->setEnabled(true);

     //鼠标左键平移

     QwtPolarPanner*  d_panner = new QwtPolarPanner(plot->canvas());

     d_panner->setEnabled(true);

    curveGraphShow(displacement_data);

}

//谐波柱形图
void PolarPlotShow::barChart()
{

    if(histogram_chart_==nullptr)
    {
        histogram_chart_ = std::make_unique<histogram>();

        histogram_chart_->setAxisScale(QwtPlot::xBottom, 0, 2048);
        histogram_chart_->setAxisMaxMajor(QwtPlot::xBottom, 30);
        histogram_chart_->setAxisMaxMinor(QwtPlot::xBottom, 0);
        histogram_chart_->setAxisScale(QwtPlot::yLeft, 0, 1);
        //    d_chart->setAxisMaxMajor(QwtPlot::yLeft, 6);
        //    d_chart->setAxisMaxMinor(QwtPlot::yLeft, 2);

        QFont fontX;
        fontX.setFamily(QStringLiteral("微软雅黑"));
        fontX.setPointSize(5);
        histogram_chart_->setAxisFont(QwtPlot::xBottom, fontX);
        histogram_chart_->setTitle("谐波");

        QwtPlotGrid *grid = new QwtPlotGrid;
        grid->setMajorPen(QColor(193, 193, 193), 1, Qt::DashLine);
        grid->attach(histogram_chart_.get());


        ui->horizontalLayout_6->addWidget(histogram_chart_.get());


    }

}

//波纹度柱形图
void PolarPlotShow::wavinessChart()
{
    if(waviness_chart_==nullptr)
    {
        waviness_chart_ = std::make_unique<histogram>();

        waviness_chart_->setAxisScale(QwtPlot::xBottom, 0, 2048);
        waviness_chart_->setAxisMaxMajor(QwtPlot::xBottom, 30);
        waviness_chart_->setAxisMaxMinor(QwtPlot::xBottom, 0);
        waviness_chart_->setAxisScale(QwtPlot::yLeft, 0, 10);


        QFont fontX;
        fontX.setFamily(QStringLiteral("微软雅黑"));
        fontX.setPointSize(5);
        waviness_chart_->setAxisFont(QwtPlot::xBottom, fontX);
        waviness_chart_->setTitle("波纹度");


        ui->horizontalLayout_6->addWidget(waviness_chart_.get());


    }
}

//线性曲线
void PolarPlotShow::curveBightChart()
{
    /*清屏重绘*/
//        ui->qwtPlot->detachItems();
//        ui->qwtPlot->replot();

    /*setAxisScale四个参数的含义分别是：坐标轴->坐标轴最小值->坐标轴最大值->步进*/
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom,0,100,10);
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft,0,10,2);
    ui->qwtPlot->setAxisTitle(QwtPlot::xBottom,"x 轴(data)");
    ui->qwtPlot->setAxisTitle(QwtPlot::yLeft,"y 轴(data)");



}

//显示极坐标图
void PolarPlotShow::polar_diagram(QString range)
{


    if(plot==nullptr)
    {
        plot=std::make_unique<QwtPolarPlot>();

        plot->setScale( QwtPolar::ScaleAzimuth, 0, 360, 30);
        if(range=="+-50μm")
        {
             plot->setScale( QwtPolar::ScaleRadius, -50,50, 0.5);
        }else if(range=="+-500μm")
        {
             plot->setScale( QwtPolar::ScaleRadius, -500,500, 0.5);
        }
        else if(range=="")
        {
            plot->setScale( QwtPolar::ScaleRadius, -2,2, 0.5);
         }
//        plot->setScale( QwtPolar::ScaleRadius, -2,2, 0.5);
        QwtPolarGrid* grid=new QwtPolarGrid();
        //    grid->setFont (QFont("Times", 16, QFont::Bold));
        grid->setPen ( QPen(Qt::lightGray,1, Qt::DashDotLine) );
        grid->setAxisPen ( QwtPolar::AxisAzimuth, QPen(Qt::black, 1) );
        //grid.setAxisPen ( QwtPolar::AxisLeft, QPen(Qt::black, 1) );

        grid->showGrid (QwtPolar::AxisAzimuth, true);
        grid->showGrid (QwtPolar::AxisLeft, true);
        grid->showGrid (QwtPolar::AxisRight, false);
        grid->showGrid (QwtPolar::AxisTop, false);
        grid->showGrid (QwtPolar::AxisBottom, false);
        grid->attach(plot.get());

                ui->verticalLayout_3->addWidget(plot.get());

    }

    if(range=="+-50μm")
    {
         plot->setScale( QwtPolar::ScaleRadius, -50,50, 0.5);
    }else if(range=="+-500μm")
    {
         plot->setScale( QwtPolar::ScaleRadius, -500,500, 0.5);
    }
    else if(range=="")
    {
        plot->setScale( QwtPolar::ScaleRadius, -2,2, 0.5);
     }


}

//谐波添加数据
void PolarPlotShow::histogramShow(QVector<double> value)
{

    barChart();

    //    /*清屏重绘*/
    histogram_chart_->detachItems();
    histogram_chart_->replot();

    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->setMajorPen(QColor(193, 193, 193), 1, Qt::DashLine);
    grid->attach(histogram_chart_.get());

    histogram_chart_->populate(value);
    histogram_chart_->replot();
    //    for(int i=0;i<value.size();i++)
    //    {

    //        histogram_x_.push_back(i);
    //        histogram_y_.push_back(value[i]);

    //        bars_->setData(histogram_x_, histogram_y_);
    //    }

    //    ui->histogram_->replot();

}

//波纹度添加数据
void PolarPlotShow::waviness(QVector<double> value)
{
    //    /*清屏重绘*/
    waviness_chart_->detachItems();
    waviness_chart_->replot();


    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->setMajorPen(QColor(193, 193, 193), 1, Qt::DashLine);
    grid->attach(waviness_chart_.get());

    waviness_chart_->waviness(value);
    waviness_chart_->replot();
    //    for(int i=0;i<fft_.amplitude_.size();i++)
    //    {

    //        waviness_x_.push_back(i);

    //        double Individual_harmonic_waviness=51.833*fft_.amplitude_[i]*i;

    //        waviness_y_.push_back(Individual_harmonic_waviness);

    //        waviness_bars_->setData(waviness_x_, waviness_y_);
    //    }
    //    ui->waviness->replot();
}


//显示曲线图
void PolarPlotShow::curveGraphShow(QVector<double> value)
{

    auto value_float=float(value.size());
    //    /*清屏重绘*/
    ui->qwtPlot->detachItems();
    ui->qwtPlot->replot();

    /*setAxisScale四个参数的含义分别是：坐标轴->坐标轴最小值->坐标轴最大值->步进*/
    //    ui->qwtPlot->setAxisScale(QwtPlot::xBottom,0,360,10);
    //    ui->qwtPlot->setAxisScale(QwtPlot::yLeft,0,10,2);
    ui->qwtPlot->setTitle("线性曲线");
    //    /*添加网格*/
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->enableX( true );//设置网格线
    grid->enableY( true );
    grid->setMajorPen( Qt::black, 0, Qt::DotLine );
    grid->attach(ui->qwtPlot);

    /*点*/
    QPolygonF point;
    for(int i=0;i<value.size();i++)
    {
        point<<QPointF(i/value_float*360.0,value[i]);
    }

    /*绘制曲线*/
    QwtPlotCurve* curve = new QwtPlotCurve("Curve 1");   //设置曲线
    curve->setSamples(point);
    curve->setPen(QColor(160, 36,48),2,Qt::SolidLine);    //设置画笔(颜色,粗细,线条样式)
    curve->attach(ui->qwtPlot);   //把曲线附加到qwtPlot上
    curve->setCurveAttribute(QwtPlotCurve::Fitted, true);   //曲线光滑

    /*ui界面显示曲线*/
    ui->qwtPlot->replot();
    ui->qwtPlot->setAutoReplot(true);   //自动更新

    //    for(int i=0;i<value.size()-1;i++)
    //    {
    //        curve_x_.push_back(phase[i]);
    //        curve_y_.push_back(value[i]);

    //        ui->curve_graph_->graph(0)->setData(curve_x_, curve_y_);

    //        ui->curve_graph_->xAxis->setLabel("x");
    //        ui->curve_graph_->yAxis->setLabel("y");
    //    }

    //    ui->curve_graph_->replot();

}

//显示极图
void PolarPlotShow::poleDiagramDisplay(QVector<double> value,QString range)
{
    //    /*清屏重绘*/
        plot->detachItems();
//        plot->replot();

        polar_diagram(range);

        QwtPolarGrid* grid=new QwtPolarGrid();

        grid->setPen ( QPen(Qt::lightGray, 1, Qt::DashDotLine) );
        grid->setAxisPen ( QwtPolar::AxisAzimuth, QPen(Qt::black, 1) );

        grid->showGrid (QwtPolar::AxisAzimuth, true);
        grid->showGrid (QwtPolar::AxisLeft, true);
        grid->showGrid (QwtPolar::AxisRight, false);
        grid->showGrid (QwtPolar::AxisTop, false);
        grid->showGrid (QwtPolar::AxisBottom, false);
        grid->attach(plot.get());

        fft_.polarCoordinatesToCartesianCoordinates(value,0,0);


        QwtPolarCurve* curve_max=new QwtPolarCurve();
        curve_max->setPen (QPen(Qt::red, 2));

        //最大内切圆
        GetPolarData* data_max=new GetPolarData();
        QVector<double> data_radius_max;
        for(int i=0;i<value.size();i++)
        {
            data_radius_max.push_back(/*fft_.incircle_radius*/num_min_);
        }
//        qDebug()<<"data_radius_max:"<<fft_.incircle_radius;
        data_max->get_data(data_radius_max);
        curve_max->setData (data_max);
        curve_max->attach(plot.get());


        QwtPolarCurve* curve=new QwtPolarCurve();
        curve->setPen (QPen(Qt::blue, 1));

//        plot.get()->setScale(QwtPolar::ScaleRadius, 5, 20, 1);     // 半径范围

        //测量数据
        GetPolarData* data=new GetPolarData();
        data->get_data(value);
        curve->setData(data);
        curve->attach(plot.get());

//        auto num=angularAxis_->center();
//        qDebug()<<num;

//        auto x=num.x();
//        auto y=num.y();

        QwtPolarCurve* curve_=new QwtPolarCurve();
        curve_->setPen (QPen(Qt::red, 2));

        //最小二乘圆
        GetPolarData* data_=new GetPolarData();
        QVector<double> data_radius;
        for(int i=0;i<value.size();i++)
        {
            data_radius.push_back(fft_.tempPoint_.radius);
        }
        qDebug()<<fft_.tempPoint_.radius;
        data_->get_data(data_radius);
        curve_->setData (data_);
        curve_->attach(plot.get());



        QwtPolarCurve* curve_min=new QwtPolarCurve();
        curve_min->setPen (QPen(Qt::blue, 2));

        //最小外切圆
        GetPolarData* data_min=new GetPolarData();
        QVector<double> data_radius_min;
        float min_radius_coverage=0;
        if(num_max_>fft_.min_coverage_circle_.radius)
        {
            min_radius_coverage=num_max_;
        }
        else
        {
            min_radius_coverage=fft_.min_coverage_circle_.radius;
        }
        qDebug()<<"min_radius_coverage:"<<min_radius_coverage;
        for(int i=0;i<value.size();i++)
        {
            data_radius_min.push_back(fft_.min_coverage_circle_.radius);
        }
        qDebug()<<fft_.min_coverage_circle_.radius;
        data_min->get_data(data_radius_min);
        curve_min->setData (data_min);
        curve_min->attach(plot.get());

//        plot->replot();

//        data_radius.clear();



        ui->verticalLayout_3->addWidget(plot.get());


//    auto value_float=float(value.size());

//    for(int i=0;i<value.size();i++)
//    {
//        auto num_=i/value_float*360.0;

//        g1_->addData(/*(i/value.size())*360*//*value[i]**/i/value_float*360.0/*(i/value.size())*360*/,value[i]);
//    }

//        auto num=angularAxis_->center();
//        qDebug()<<num;

//        auto x=num.x();
//        auto y=num.y();
//        fft_.polarCoordinatesToCartesianCoordinates(value,x,y);

//    //    qDebug()<<angularAxis_->radius();


//        for(int i=0;i<value.size();i++)
//        {
//            g2_->addData(/*(i/value.size())*360*//*value[i]**//*phase_angle[i]*/i/value_float*360.0,fft_.tempPoint_.radius);

//        }




    //    ui->verticalLayout_3->addWidget(plot);

    //    for(int i=0;i<value.size();i++)
    //    {
    //        g3_->addData(/*(i/value.size())*360*//*value[i]**//*phase_angle[i]*/i/value_float*360.0,fft_.min_coverage_circle_.radius/*qSin(i/100.0*M_PI*8)*8+1*/);

    //    }

    //    for(int i=0;i<value.size();i++)
    //    {
    //        g4_->addData(/*(i/value.size())*360*//*value[i]**//*phase_angle[i]*/i/value_float*360.0,num_min_);

    //    }

    //    qDebug()<<fft_.tempPoint_.radius;
    //    qDebug()<<fft_.min_coverage_circle_.radius;
    //    qDebug()<<angularAxis->bottomRight();

    //     LeastSquaresCircle();


}


void PolarPlotShow::startToPrepare()
{

    //曲线图
    ui->curve_graph_->addGraph();

    ui->curve_graph_->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);  //用户可以通过拖拽坐标轴和滑动滚轮的方式来缩放图像

    //波纹度
    //    waviness_xAxis_ = ui->waviness->xAxis;
    //    waviness_yAxis_ = ui->waviness->yAxis;
    //    waviness_bars_ = new QCPBars( ui->waviness->xAxis,  ui->waviness->yAxis);

    //    waviness_bars_->setAntialiased(false); // 为了更好的边框效果，关闭抗齿锯
    //    waviness_bars_->setPen(QPen(QColor(0, 160, 140).lighter(130))); // 设置柱状图的边框颜色
    //    waviness_bars_->setBrush(QColor(20,68,106));  // 设置柱状图的画刷颜色

    //    ui->waviness->xAxis->setTickLength(0,1);          // 轴内外刻度的长度分别是0,1,也就是轴内的刻度线不显示

    //    ui->waviness->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    //    ui->waviness->yAxis->setPadding(35);             // 轴的内边距
    //    ui->waviness->yAxis->setLabel("y");
    //    ui->waviness->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    //柱形图
    //    xAxis_ = ui->histogram_->xAxis;
    //    yAxis_ = ui->histogram_->yAxis;
    //    bars_ = new QCPBars( ui->histogram_->xAxis,  ui->histogram_->yAxis);

    //    bars_->setAntialiased(false); // 为了更好的边框效果，关闭抗齿锯
    //    bars_->setPen(QPen(QColor(0, 160, 140).lighter(130))); // 设置柱状图的边框颜色
    //    bars_->setBrush(QColor(20,68,106));  // 设置柱状图的画刷颜色

    //    ui->histogram_->xAxis->setTickLength(0,1);          // 轴内外刻度的长度分别是0,1,也就是轴内的刻度线不显示

    //    ui->histogram_->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    //    ui->histogram_->yAxis->setPadding(35);             // 轴的内边距
    //    ui->histogram_->yAxis->setLabel("y");
    //    ui->histogram_->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);


    //极图显示
//    ui->circle_graph->plotLayout()->clear();


//    /*QCPPolarAxisAngular **/angularAxis_ = new QCPPolarAxisAngular(ui->circle_graph);
//    ui->circle_graph->plotLayout()->addElement(0, 0, angularAxis_);

//    ui->circle_graph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);  //用户可以通过拖拽坐标轴和滑动滚轮的方式来缩放图像


//    angularAxis_->setTickLabelMode(QCPPolarAxisAngular::lmUpright);

//    angularAxis_->radialAxis()->setTickLabelMode(QCPPolarAxisRadial::lmUpright);


//    /* QCPPolarGraph **/g1_ = new QCPPolarGraph(angularAxis_, angularAxis_->radialAxis());
//    g2_ = new QCPPolarGraph(angularAxis_, angularAxis_->radialAxis());

//    g3_ = new QCPPolarGraph(angularAxis_, angularAxis_->radialAxis());

//    g4_ = new QCPPolarGraph(angularAxis_, angularAxis_->radialAxis());

//    g2_->setPen(QPen(QColor(255, 0, 0)/*.lighter(130)*/));

//    g3_->setPen(QPen(QColor(255, 0, 0)/*.lighter(130)*/));

//    g4_->setPen(QPen(QColor(255, 0, 0)/*.lighter(130)*/));
//    //g1->setScatterStyle(/*QCPScatterStyle::ssCross*//*ssDisc*/QCPScatterStyle(QCPScatterStyle::ssCircle, 2));

//    angularAxis_->radialAxis()->setRange(0, 2000);





}

void PolarPlotShow::LeastSquaresCircle()
{
    QCPItemEllipse * qcpitemellipse=new QCPItemEllipse(ui->circle_graph);

    qcpitemellipse->topLeft->setCoords(fft_.center_x_+fft_.radius_,fft_.center_y_+fft_.radius_);

    qcpitemellipse->bottomRight->setCoords(fft_.center_x_+fft_.radius_,fft_.center_y_-fft_.radius_);//圆右下角位置
    qcpitemellipse->setBrush(QBrush(QColor(0,255,255)));//填充圆的颜色


    ui->circle_graph->replot();


}

//识别区域属性
void PolarPlotShow::identifyAreaAttributesShow()
{
    if(config_identify==nullptr)
    {
        config_identify=std::make_unique<identify_area_attributes>();

        config_identify->setWindowModality(Qt::ApplicationModal);

        connect(config_identify.get(), SIGNAL(signal_type_of_part(bool)),this, SLOT(recive_type_of_part(bool)));

        connect(config_identify.get(), SIGNAL(signal_attribute(bool)),this, SLOT(recive_attribute(bool)));

        connect(config_identify.get(), SIGNAL(signal_amount(bool)),this, SLOT(recive_amount(bool)));

        connect(config_identify.get(), SIGNAL(signal_order(bool)),this, SLOT(recive_order(bool)));

        connect(config_identify.get(), SIGNAL(signal_client(bool)),this, SLOT(recive_client(bool)));

        connect(config_identify.get(), SIGNAL(signal_channel(bool)),this, SLOT(recive_channel(bool)));

        connect(config_identify.get(), SIGNAL(signal_operator(bool)),this, SLOT(recive_operator(bool)));

        connect(config_identify.get(), SIGNAL(signal_tor_number(bool)),this, SLOT(recive_tor_number(bool)));

        connect(config_identify.get(), SIGNAL(signal_factory(bool)),this, SLOT(recive_factor(bool)));

        connect(config_identify.get(), SIGNAL(signal_branch(bool)),this, SLOT(recive_branch(bool)));

        connect(config_identify.get(), SIGNAL(signal_machine(bool)),this, SLOT(recive_machine(bool)));

        connect(config_identify.get(), SIGNAL(signal_operate(bool)),this, SLOT(recive_operate(bool)));

    }
    config_identify->show();
}

void PolarPlotShow::recive_type_of_part(bool flag)
{
    if(flag==true)
    {
        ui->label_13->show();
        ui->part_type_label->show();
    }
    else if(flag==false)
    {
        ui->label_13->hide();
        ui->part_type_label->hide();
    }
}
void PolarPlotShow::recive_attribute(bool flag)
{
    if(flag==true)
    {
        ui->label_14->show();
        ui->attribute_label->show();
    }
    else if(flag==false)
    {
        ui->label_14->hide();
        ui->attribute_label->hide();
    }
}

void PolarPlotShow::recive_amount(bool flag)
{
    if(flag==true)
    {
        ui->label_15->show();
        ui->number_label->show();
    }
    else if(flag==false)
    {
        ui->label_15->hide();
        ui->number_label->hide();
    }
}
void PolarPlotShow::recive_order(bool flag)
{
    if(flag==true)
    {
        ui->label_16->show();
        ui->order_label->show();
    }
    else if(flag==false)
    {
        ui->label_16->hide();
        ui->order_label->hide();
    }

}
void PolarPlotShow::recive_client(bool flag)
{
    if(flag==true)
    {
        ui->label_17->show();
        ui->client_label->show();
    }
    else if(flag==false)
    {
        ui->label_17->hide();
        ui->client_label->hide();
    }

}
void PolarPlotShow::recive_channel(bool flag)
{

    if(flag==true)
    {
        ui->label_18->show();
        ui->channel_label->show();
    }
    else if(flag==false)
    {
        ui->label_18->hide();
        ui->channel_label->hide();
    }
}
void PolarPlotShow::recive_operator(bool flag)
{

    if(flag==true)
    {
        ui->label_19->show();
        ui->operator_label->show();
    }
    else if(flag==false)
    {
        ui->label_19->hide();
        ui->operator_label->hide();
    }

}
void PolarPlotShow::recive_tor_number(bool flag)
{
    if(flag==true)
    {
        ui->label_->show();
        ui->operator_num_label->show();
    }
    else if(flag==false)
    {
        ui->label_->hide();
        ui->operator_num_label->hide();
    }

}
void PolarPlotShow::recive_factor(bool flag)
{
    if(flag==true)
    {
        ui->label_21->show();
        ui->factory_label->show();
    }
    else if(flag==false)
    {
        ui->label_21->hide();
        ui->factory_label->hide();
    }
}
void PolarPlotShow::recive_branch(bool flag)
{
    if(flag==true)
    {
        ui->label_23->show();
        ui->branch_label->show();
    }
    else if(flag==false)
    {
        ui->label_23->hide();
        ui->branch_label->hide();
    }
}
void PolarPlotShow::recive_machine(bool flag)
{
    if(flag==true)
    {
        ui->label_22->show();
        ui->machine_label->show();
    }
    else if(flag==false)
    {
        ui->label_22->hide();
        ui->machine_label->hide();
    }

}
void PolarPlotShow::recive_operate(bool flag)
{
    if(flag==true)
    {
        ui->label_24->show();
        ui->operate_label->show();
    }
    else if(flag==false)
    {
        ui->label_24->hide();
        ui->operate_label->hide();
    }
}
//结果区域属性
void PolarPlotShow::resultAreaAttributesShow()
{
    Result_area_attribute *config_result=new Result_area_attribute;
    config_result->setWindowModality(Qt::ApplicationModal);
    config_result->show();

    connect(config_result,SIGNAL(signal_tolerance(bool)),this,SLOT(recive_tolerance(bool)));
    connect(config_result,SIGNAL(signal_limit_value(bool)),this,SLOT(recive_limit_value(bool)));
    connect(config_result,SIGNAL(signal_header(bool)),this,SLOT(recive_header(bool)));
    connect(config_result,SIGNAL(signal_control_decimal_place(int)),this,SLOT(recive_control_decimal_place(int)));


}

void PolarPlotShow::recive_tolerance(bool flag)
{
    if(flag==true)
    {
        for(int i=0;i<ui->tableWidget->rowCount();i++)
        {
            ui->tableWidget->setColumnHidden(5, false); // 隐藏第一列
        }
    }
    else if(flag==false)
    {
        for(int i=0;i<ui->tableWidget->rowCount();i++)
        {
            ui->tableWidget->setColumnHidden(5, true); // 隐藏第一列
        }
    }
}

void PolarPlotShow::recive_limit_value(bool flag)
{
    if(flag==true)
    {
        for(int i=0;i<ui->tableWidget->rowCount();i++)
        {
            ui->tableWidget->setColumnHidden(4, false); // 隐藏第一列
        }
    }
    else if(flag==false)
    {
        for(int i=0;i<ui->tableWidget->rowCount();i++)
        {
            ui->tableWidget->setColumnHidden(4, true); // 隐藏第一列
        }
    }
}

void PolarPlotShow::recive_header(bool flag)
{
    if(flag==true)
    {
        for(int i=0;i<ui->tableWidget->rowCount();i++)
        {
            ui->tableWidget->horizontalHeader()->setVisible(true); // 隐藏第一行
        }
    }
    else if(flag==false)
    {
        for(int i=0;i<ui->tableWidget->rowCount();i++)
        {
            ui->tableWidget->horizontalHeader()->setVisible(false); // 隐藏第一列
        }
    }
}

void PolarPlotShow::recive_control_decimal_place(int arg)
{

    for(int i=0;i<ui->tableWidget->rowCount();i++)
    {
        QString str_result= ui->tableWidget->item(i,2)->text();
        float float_result=str_result.toFloat();
        str_result=QString::number(float_result,'f',arg);
        ui->tableWidget->item(i,2)->setText(QString("%1").arg(str_result));//写入

        QString str_limit= ui->tableWidget->item(i,4)->text();
        float float_limit=str_limit.toFloat();
        str_limit=QString::number(float_limit,'f',arg);
        ui->tableWidget->item(i,4)->setText(QString("%1").arg(str_limit));//写入
    }
}

//备注区域属性
void PolarPlotShow::areaOfRemarksShow()
{
    //    config_remarks_=new property_of_curve();
    if(config_remarks_==nullptr)
    {
        config_remarks_=std::make_unique<property_of_curve>();
        connect(config_remarks_.get(),SIGNAL(display_Remarks(bool)),this,SLOT(recive_Display_Remarks(bool)));
        connect(config_remarks_.get(),SIGNAL(display_Time(bool)),this,SLOT(recive_Display_Time(bool)));

        config_remarks_->setWindowModality(Qt::ApplicationModal);

    }

    config_remarks_->show();

}

void PolarPlotShow::recive_Display_Remarks(bool flag)
{
    if(flag==true)
    {
        ui->label_30->show();
        ui->label_5->show();
    }
    else if(flag==false)
    {
        ui->label_30->hide();
        ui->label_5->hide();
    }
    //控件隐藏 布局不变
    QSizePolicy size_hint = ui->label_5->sizePolicy();
    size_hint.setRetainSizeWhenHidden(true);
    ui->label_5->setSizePolicy(size_hint);
}

void PolarPlotShow::recive_Display_Time(bool flag)
{
    if(flag==true)
    {
        ui->label->show();
        ui->label_28->show();
        ui->remark_time->show();
        ui->remark_speed->show();
    }
    else if(flag==false)
    {
        ui->label->hide();
        ui->label_28->hide();
        ui->remark_speed->hide();
        ui->remark_time->hide();

        QSizePolicy size_hint = ui->remark_speed->sizePolicy();
        size_hint.setRetainSizeWhenHidden(true);
        ui->remark_speed->setSizePolicy(size_hint);

        QSizePolicy size_hint_ = ui->remark_time->sizePolicy();
        size_hint_.setRetainSizeWhenHidden(true);
        ui->remark_time->setSizePolicy(size_hint_);

    }

}

//备注区域属性
void PolarPlotShow::custplotPropertyShow()
{
    property_of_curve *config_window=new property_of_curve;
    config_window->setWindowModality(Qt::ApplicationModal);
    config_window->show();
}

//图表属性
void PolarPlotShow::custplotChartShow()
{
    chart *config_chart=new chart;


    connect(config_chart, SIGNAL(signal_show_type_of_spectrum(QString)),this, SLOT(recive_show_type_of_spectrum(QString)));
    connect(config_chart,SIGNAL(signal_show_x(int)),this,SLOT(recive_show_x_barchart(int)));
    connect(config_chart,SIGNAL(signal_show_y(QString)),this,SLOT(recive_show_y_barchart(QString)));

    config_chart->setWindowModality(Qt::ApplicationModal);
    config_chart->show();
}



//极图属性
void PolarPlotShow::circlePropertyShow()
{
    //    config_circle_=new circle_property_;
    if(config_circle_==nullptr)
    {
        config_circle_=std::make_unique<circle_property_>();

        connect(config_circle_.get(),SIGNAL(display_annotation(bool)),this,SLOT(recive_display_annotation(bool)));
        connect(config_circle_.get(),SIGNAL(display_time(bool)),this,SLOT(recive_display_time_speed(bool)));

        config_circle_->setWindowModality(Qt::ApplicationModal);
    }

    config_circle_->show();
}

void PolarPlotShow::recive_display_annotation(bool flag)
{
    if(flag==true)
    {
        ui->label_35->show();
        ui->label_36->show();
        ui->label_39->show();
        ui->label_40->show();
    }
    else if(flag==false)
    {
        ui->label_35->hide();
        ui->label_36->hide();
        ui->label_39->hide();
        ui->label_40->hide();
    }

}


void PolarPlotShow::recive_display_time_speed(bool flag)
{
    if(flag==true)
    {
        ui->label_34->show();
        ui->label_37->show();
        ui->label_time_circle->show();
        ui->label_speed_circle->show();
    }
    else if(flag==false)
    {
        ui->label_34->hide();
        ui->label_37->hide();
        ui->label_time_circle->hide();
        ui->label_speed_circle->hide();
    }


}

void PolarPlotShow::recive_show_type_of_spectrum(QString type)
{
    if(type=="waviness")
    {
        //        ui->waviness->show();
        //        ui->histogram_->hide();
        histogram_chart_.get()->hide();
        wavinessChart();
        waviness_chart_.get()->show();

        //        ui->curve_graph_->hide();
        ui->qwtPlot->hide();
    }
    else if(type=="harmonic_wave")
    {
        //        ui->waviness->hide();
        //        ui->histogram_->show();
        waviness_chart_.get()->hide();
        histogram_chart_.get()->show();
        //        ui->curve_graph_->hide();
        ui->qwtPlot->hide();
    }
    else if(type=="linearity_curve")
    {
        //        ui->waviness->hide();
        //        ui->histogram_->hide();
        waviness_chart_.get()->hide();
        histogram_chart_.get()->hide();
        //        ui->curve_graph_->show();
        ui->qwtPlot->show();

    }
}

void PolarPlotShow::recive_show_x_barchart(int data)
{
    barChart();

    qDebug()<<data;
    histogram_chart_->setAxisScale(QwtPlot::xBottom, 0, data);

    histogram_chart_->setAxisMaxMinor(QwtPlot::xBottom, 0);
    histogram_chart_->setAxisScale(QwtPlot::yLeft, 0, 1);

    histogram_chart_->replot();


}

void PolarPlotShow::recive_show_y_barchart(QString data)
{
    barChart();

    qDebug()<<data;

    auto num=data.toFloat();

    histogram_chart_->setAxisScale(QwtPlot::yLeft, 0, 1,num);

    histogram_chart_->replot();
}

//按钮显示柱状图
void PolarPlotShow::on_chart_pushButton_clicked()
{
    barChart();
    wavinessChart();

    if(spectrogram_count_ ==0)
    {
        ui->graph_pushButton_2->setEnabled(true);
        //        ui->histogram_->show();
        //        ui->curve_graph_->hide();
        //        ui->waviness->hide();


        histogram_chart_.get()->show();
        waviness_chart_.get()->hide();
        ui->qwtPlot->hide();

        spectrogram_count_++;
    }
    else if(spectrogram_count_ ==1)
    {
        //        ui->curve_graph_->show();
        //        ui->waviness->hide();
        //        ui->histogram_->hide();

        waviness_chart_.get()->hide();
        histogram_chart_.get()->hide();
        ui->qwtPlot->show();

        spectrogram_count_++;
    }
    else if(spectrogram_count_ ==2)
    {

        //        ui->waviness->show();
        //        ui->histogram_->hide();
        //        ui->curve_graph_->hide();

        histogram_chart_.get()->hide();
        waviness_chart_.get()->show();
        ui->qwtPlot->hide();

        spectrogram_count_++;
    }
    if(spectrogram_count_==3)
    {
        ui->chart_pushButton->setEnabled(false);
        spectrogram_count_=0;
    }

    histogramMenu();
}

//按钮显示曲线图
void PolarPlotShow::on_graph_pushButton_2_clicked()
{
    barChart();
    wavinessChart();

    if(spectrogram_count_ ==0)
    {
        ui->chart_pushButton->setEnabled(true);
        //        ui->curve_graph_->show();
        //        ui->histogram_->hide();
        //        ui->waviness->hide();

        waviness_chart_.get()->hide();
        histogram_chart_.get()->hide();
        ui->qwtPlot->show();

        spectrogram_count_++;
    }
    else if(spectrogram_count_==1)
    {
        //        ui->histogram_->show();
        //        ui->waviness->hide();
        //        ui->curve_graph_->hide();

        histogram_chart_.get()->show();
        waviness_chart_.get()->hide();
        ui->qwtPlot->hide();
        spectrogram_count_++;
    }
    else if(spectrogram_count_==2)
    {
        //        ui->waviness->show();
        //        ui->histogram_->hide();
        //        ui->curve_graph_->hide();

        histogram_chart_.get()->hide();
        waviness_chart_.get()->show();
        ui->qwtPlot->hide();
        spectrogram_count_++;
    }
    if(spectrogram_count_==3)
    {
        ui->graph_pushButton_2->setEnabled(false);
        spectrogram_count_=0;
    }

    curveGraphMenu();
}
//曲线区域子菜单
void PolarPlotShow::curveGraphMenu()
{
    ui_customplot_chart_=new QAction("属性",this);
    ui_customplot_export_data_=new QAction("导出数据",this);

    customplot_menu_ = new QMenu(this);  //初始化菜单

    //customplot_menu_->addAction(ui_customplot_property_);//动作添加到菜单
    customplot_menu_->addAction(ui_customplot_chart_);//动作添加到菜单
    customplot_menu_->addAction(ui_customplot_export_data_);//动作添加到菜单

    //给图表动作设置信号槽
    connect( ui_customplot_chart_, &QAction::triggered, [=]()
    {
        qDebug()<<"I'm chart";
    });

    //给导出数据动作设置信号槽
    connect( ui_customplot_export_data_, &QAction::triggered, [=]()
    {
        qDebug()<<"I'm export data";
    });

    connect(ui_customplot_chart_,&QAction::triggered,this,&PolarPlotShow::custplotChartShow);

    ui->curve_graph_->setContextMenuPolicy(Qt::CustomContextMenu);

    connect( ui->curve_graph_,&QCustomPlot::customContextMenuRequested,[=](const QPoint &pos)
    {
        qDebug()<<pos;//参数pos用来传递右键点击时的鼠标的坐标，这个坐标一般是相对于控件左上角而言的

        customplot_menu_->exec(QCursor::pos());
    });

}

//柱形图曲线图区域子菜单
void PolarPlotShow::histogramMenu()
{

    ui_customplot_chart_=new QAction("属性",this);
    ui_customplot_export_data_=new QAction("导出数据",this);


    customplot_menu_ = new QMenu(this);  //初始化菜单

    customplot_menu_->addAction(ui_customplot_chart_);//动作添加到菜单
    customplot_menu_->addAction(ui_customplot_export_data_);//动作添加到菜单

    //给图表动作设置信号槽
    connect( ui_customplot_chart_, &QAction::triggered, [=]()
    {
        qDebug()<<"I'm chart";
    });

    //给导出数据动作设置信号槽
    connect( ui_customplot_export_data_, &QAction::triggered, [=]()
    {
        qDebug()<<"I'm export data";
    });

    connect(ui_customplot_chart_,&QAction::triggered,this,&PolarPlotShow::custplotChartShow);


    barChart();

    wavinessChart();




    //给控件设置上下文菜单策略
    histogram_chart_.get()->setContextMenuPolicy(Qt::CustomContextMenu);
    waviness_chart_.get()->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->qwtPlot->setContextMenuPolicy(Qt::CustomContextMenu);

    //    //鼠标右键点击控件时会发送一个void QWidget::customContextMenuRequested(const QPoint &pos)信号
    //    //给信号设置相应的槽函数
    connect(histogram_chart_.get(),&QCustomPlot::customContextMenuRequested,[=](const QPoint &pos)
    {
        qDebug()<<pos;//参数pos用来传递右键点击时的鼠标的坐标，这个坐标一般是相对于控件左上角而言的
        customplot_menu_->exec(QCursor::pos());
    });

    connect(waviness_chart_.get(),&QCustomPlot::customContextMenuRequested,[=](const QPoint &pos)
    {
        qDebug()<<pos;//参数pos用来传递右键点击时的鼠标的坐标，这个坐标一般是相对于控件左上角而言的
        customplot_menu_->exec(QCursor::pos());
    });

    connect(ui->qwtPlot,&QCustomPlot::customContextMenuRequested,[=](const QPoint &pos)
    {
        qDebug()<<pos;//参数pos用来传递右键点击时的鼠标的坐标，这个坐标一般是相对于控件左上角而言的
        customplot_menu_->exec(QCursor::pos());
    });




}

//极图区域子菜单
void PolarPlotShow::poleDiagramDisplayMenu()
{

    ui_customplot_property_ = new QAction("属性",this);//初始化动作
    ui_customplot_export_data_=new QAction("导出数据",this);


    pole_figure_menu_ = new QMenu(this);  //初始化菜单

    pole_figure_menu_->addAction(ui_customplot_property_);//动作添加到菜单

    pole_figure_menu_->addAction(ui_customplot_export_data_);//动作添加到菜单


    //给属性动作设置信号槽
    connect( ui_customplot_property_, &QAction::triggered, [=]()
    {
        qDebug()<<"I'm property";
    });

    //给导出数据动作设置信号槽
    connect( ui_customplot_export_data_, &QAction::triggered, [=]()
    {
        qDebug()<<"I'm export data";
    });



    connect(ui_customplot_property_,&QAction::triggered,this,&PolarPlotShow::circlePropertyShow);

    polar_diagram("10");
    plot.get()->setContextMenuPolicy(Qt::CustomContextMenu);

    //给控件设置上下文菜单策略
    ui->circle_graph->setContextMenuPolicy(Qt::CustomContextMenu);
    //鼠标右键点击控件时会发送一个void QWidget::customContextMenuRequested(const QPoint &pos)信号
    //给信号设置相应的槽函数
    connect(ui->circle_graph,&QCustomPlot::customContextMenuRequested,[=](const QPoint &pos)
    {
        qDebug()<<pos;//参数pos用来传递右键点击时的鼠标的坐标，这个坐标一般是相对于控件左上角而言的
        pole_figure_menu_->exec(QCursor::pos());
    });

    connect(plot.get(),&QCustomPlot::customContextMenuRequested,[=](const QPoint &pos)
    {
        qDebug()<<pos;//参数pos用来传递右键点击时的鼠标的坐标，这个坐标一般是相对于控件左上角而言的
        pole_figure_menu_->exec(QCursor::pos());
    });

}

//识别区域子菜单
void PolarPlotShow::identifyTheMenu()
{

    ui_customplot_property_ = new QAction("属性",this);//初始化动作

    identification_menu_ = new QMenu(this);  //初始化菜单

    identification_menu_->addAction(ui_customplot_property_);//动作添加到菜单


    //给属性动作设置信号槽
    connect( ui_customplot_property_, &QAction::triggered, [=]()
    {
        qDebug()<<"I'm property";
    });

    connect(ui_customplot_property_,&QAction::triggered,this,&PolarPlotShow::identifyAreaAttributesShow);

    //给控件设置上下文菜单策略
    ui->identifiable_area_group->setContextMenuPolicy(Qt::CustomContextMenu);
    //鼠标右键点击控件时会发送一个void QWidget::customContextMenuRequested(const QPoint &pos)信号
    //给信号设置相应的槽函数
    connect(ui->identifiable_area_group,&QCustomPlot::customContextMenuRequested,[=](const QPoint &pos)
    {
        qDebug()<<pos;//参数pos用来传递右键点击时的鼠标的坐标，这个坐标一般是相对于控件左上角而言的
        identification_menu_->exec(QCursor::pos());
    });
}

//结果区域子菜单
void PolarPlotShow::resultsTheMenu()
{

    ui_customplot_property_ = new QAction("属性",this);//初始化动作

    result_menu_ = new QMenu(this);  //初始化菜单

    result_menu_->addAction(ui_customplot_property_);//动作添加到菜单


    //给属性动作设置信号槽
    connect( ui_customplot_property_, &QAction::triggered, [=]()
    {
        qDebug()<<"I'm property";
    });

    connect(ui_customplot_property_,&QAction::triggered,this,&PolarPlotShow::resultAreaAttributesShow);

    //给控件设置上下文菜单策略
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    //鼠标右键点击控件时会发送一个void QWidget::customContextMenuRequested(const QPoint &pos)信号
    //给信号设置相应的槽函数
    connect(ui->tableWidget,&QCustomPlot::customContextMenuRequested,[=](const QPoint &pos)
    {
        qDebug()<<pos;//参数pos用来传递右键点击时的鼠标的坐标，这个坐标一般是相对于控件左上角而言的
        result_menu_->exec(QCursor::pos());
    });


}

//备注区域子菜单
void PolarPlotShow::areaOfRemarksMenu()
{

    ui_customplot_property_ = new QAction("属性",this);//初始化动作

    remark_menu_ = new QMenu(this);  //初始化菜单

    remark_menu_->addAction(ui_customplot_property_);//动作添加到菜单


    //给属性动作设置信号槽
    connect( ui_customplot_property_, &QAction::triggered, [=]()
    {
        qDebug()<<"I'm property";
    });

    connect(ui_customplot_property_,&QAction::triggered,this,&PolarPlotShow::areaOfRemarksShow);

    //给控件设置上下文菜单策略
    ui->widget->setContextMenuPolicy(Qt::CustomContextMenu);
    //鼠标右键点击控件时会发送一个void QWidget::customContextMenuRequested(const QPoint &pos)信号
    //给信号设置相应的槽函数
    connect(ui->widget,&QCustomPlot::customContextMenuRequested,[=](const QPoint &pos)
    {
        qDebug()<<pos;//参数pos用来传递右键点击时的鼠标的坐标，这个坐标一般是相对于控件左上角而言的
        remark_menu_->exec(QCursor::pos());
    });


}

void PolarPlotShow::show_time()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy.MM.dd hh:mm:ss dddd");

    ui->label_time_circle->setText(str);
    //    show_time_label_->setText(str);

    //    ui->statusbar->addPermanentWidget(show_time_label_);
    //    ui->statusbar->setSizeGripEnabled(true);

}



void PolarPlotShow::on_pushButton_clicked()
{

    //    ui->histogram_->xAxis->setRangeLower(500);  //x  0-2000 改变 500-2000

    //    ui->histogram_->replot();

}

void PolarPlotShow::on_pushButton_2_clicked()
{
    ui->circle_graph->xAxis->setRange(-1,4);
    ui->circle_graph->replot();

}
