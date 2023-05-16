#include "new_profile.h"
#include "ui_new_profile.h"
#include "chart.h"

new_profile::new_profile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::new_profile)
{
    ui->setupUi(this);



    ui->histogram_->xAxis->setRange(0, 2048);//x ,y 的坐标
    ui->histogram_->yAxis->setRange(0, 100);

    ui->curve_graph_->xAxis->setRange(0, 360);//x ,y 的坐标
    ui->curve_graph_->yAxis->setRange(-2, 2);

    ui->histogram_->setInteraction(QCP::iRangeZoom, true); //滚轮滑动缩放


    ui->histogram_->hide();

    //曲线图
    ui->curve_graph_->addGraph();

    //柱形图
    xAxis_ = ui->histogram_->xAxis;
    yAxis_ = ui->histogram_->yAxis;
    bars_ = new QCPBars( ui->histogram_->xAxis,  ui->histogram_->yAxis);

    bars_->setAntialiased(false); // 为了更好的边框效果，关闭抗齿锯
    bars_->setPen(QPen(QColor(0, 160, 140).lighter(130))); // 设置柱状图的边框颜色
    bars_->setBrush(QColor(20,68,106));  // 设置柱状图的画刷颜色

    ui->histogram_->xAxis->setTickLength(0,1);          // 轴内外刻度的长度分别是0,1,也就是轴内的刻度线不显示

    ui->histogram_->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    ui->histogram_->yAxis->setPadding(35);             // 轴的内边距
    ui->histogram_->yAxis->setLabel("y");
    ui->histogram_->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

}

new_profile::~new_profile()
{
    delete ui;
}



//显示曲线图
void new_profile::curve_Graph_Show(double value)
{

    if(curve_count_== measurement_)
        curve_count_=0;

    curve_count_=curve_count_+1;

    curve_x_.push_back(curve_count_);
    curve_y_.push_back(value);

    ui->curve_graph_->graph(0)->setData(curve_x_, curve_y_);

    ui->curve_graph_->xAxis->setLabel("x");
    ui->curve_graph_->yAxis->setLabel("y");

    ui->curve_graph_->replot();

}

//显示柱形图
void new_profile::histogram_Show(double value)
{


    if(columns_count_== measurement_)
        columns_count_=0;

    columns_count_=columns_count_+1;
    histogram_x_.push_back(columns_count_);
    histogram_y_.push_back(value);



    bars_->setData(histogram_x_, histogram_y_);

    ui->histogram_->replot();

}

//曲线区域子菜单
void new_profile::curve_Graph_Menu()
{
    ui_customplot_property_ = new QAction("属性",this);//初始化动作
    ui_customplot_export_data_=new QAction("导出数据",this);

    customplot_menu_ = new QMenu(this);  //初始化菜单

    customplot_menu_->addAction(ui_customplot_property_);//动作添加到菜单
    customplot_menu_->addAction(ui_customplot_export_data_);//动作添加到菜单

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

    connect(ui_customplot_property_,&QAction::triggered,this,&new_profile::custplot_Property_Show);

    ui->curve_graph_->setContextMenuPolicy(Qt::CustomContextMenu);

    connect( ui->curve_graph_,&QCustomPlot::customContextMenuRequested,[=](const QPoint &pos)
    {
        qDebug()<<pos;//参数pos用来传递右键点击时的鼠标的坐标，这个坐标一般是相对于控件左上角而言的

        customplot_menu_->exec(QCursor::pos());
    });

}

//柱状图区域子菜单
void new_profile::histogram_Menu()
{

    ui_customplot_property_ = new QAction("属性",this);//初始化动作
    ui_customplot_export_data_=new QAction("导出数据",this);


    customplot_menu_ = new QMenu(this);  //初始化菜单

    customplot_menu_->addAction(ui_customplot_property_);//动作添加到菜单
    customplot_menu_->addAction(ui_customplot_export_data_);//动作添加到菜单

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

    connect(ui_customplot_property_,&QAction::triggered,this,&new_profile::custplot_Property_Show);

    //给控件设置上下文菜单策略
    ui->histogram_->setContextMenuPolicy(Qt::CustomContextMenu);
    //鼠标右键点击控件时会发送一个void QWidget::customContextMenuRequested(const QPoint &pos)信号
    //给信号设置相应的槽函数
    connect(ui->histogram_,&QCustomPlot::customContextMenuRequested,[=](const QPoint &pos)
    {
        qDebug()<<pos;//参数pos用来传递右键点击时的鼠标的坐标，这个坐标一般是相对于控件左上角而言的
        customplot_menu_->exec(QCursor::pos());
    });


}


//属性
void new_profile::custplot_Property_Show()
{
    chart *config_window=new chart;
    config_window->setWindowModality(Qt::ApplicationModal);
    config_window->show();
}


void new_profile::on_his_button_clicked()
{
    ui->curve_graph_->hide();
    ui->histogram_->show();
    histogram_Menu();
}

void new_profile::on_curve_button_clicked()
{
    ui->histogram_->hide();
    ui->curve_graph_->show();
    curve_Graph_Menu();
}
