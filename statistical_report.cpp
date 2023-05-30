#include "statistical_report.h"
#include "ui_statistical_report.h"
#include <QDebug>
#include <QPrinter>
#include <QPainter>
#include <QTimer>
#include <QDateTime>
#include <cmath>

statistical_report::statistical_report(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::statistical_report)
{
    ui->setupUi(this);

//    QTableWidgetItem* pItem = ui->tableWidget->item(ui->tableWidget->rowCount(), ui->tableWidget->columnCount());
//    pItem->setFlags(pItem->flags() & (~Qt::ItemIsEditable));
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QTimer *timer_calendar;
    timer_calendar=new QTimer(this);
    connect(timer_calendar,SIGNAL(timeout()),this,SLOT(show_time()));
    timer_calendar->start(1000);
}

statistical_report::~statistical_report()
{
    delete ui;
}

void statistical_report::show_time()
{
    QDateTime time = QDateTime::currentDateTime();
    time_ = time.toString("yyyy-MM-dd hh:mm:ss dddd");




}

//关闭按钮
void statistical_report::on_pushButton_2_clicked()
{
    ui->identifiable_area_group->hide();
    ui->widget_2->hide();
    for(int i=0;i<ui->horizontalLayout->count();i++)
    {
        QWidget* w = ui->horizontalLayout->itemAt(i)->widget();
        if (w != NULL)
            w->setVisible(false);
    }

}

//打印预览
void statistical_report::on_pushButton_clicked()
{

    QPrintPreviewDialog dlg1;//创建打印预览dlg
    connect(&dlg1, SIGNAL(paintRequested(QPrinter*)), SLOT(paintRequestedHandler(QPrinter*)));
    dlg1.setWindowFlag(Qt::WindowMinMaxButtonsHint);//设置窗口属性，支持最大最小化，不设置这个窗口不能双击标题栏放大缩小，也没有最大最小化按钮
    dlg1.exec();//界面显示
}

void statistical_report::paintRequestedHandler(QPrinter *printerPixmap)
{
    //QPageLayout("A4", Landscape, l:0 r:0 t:0 b:0 pt);
    //A4纸张大小；Portrait纵向；Landscape横向；l/r/t/b上下左右页边距；
    //    qDebug()<<"当前页面布局："<<printerPixmap->pageLayout();//下面的绘图参考这几个参数

    QPainter Painter(printerPixmap);

    ui->widget_3->render(&Painter);

//    printerPixmap->newPage();//新建立一个页面
//    Painter.drawText(30,20,"第2页");//页面显示内容

//    printerPixmap->newPage();//新建立一个页面
//    Painter.drawText(30,20,"第3页");//页面显示内容
}

//table添加数据
void statistical_report::tableWidgetAddData(QVector<table_config> &data)
{

    static int count=0;

    QStringList header;  //QString类型的List容器
    QStringList vertical_header;  //QString类型的List容器

    ui->tableWidget->setColumnCount(data.size()+2);

    ui->tableWidget->setRowCount(7+count);

    for(int i=0;i<data.size();i++)
    {
        auto item=qFind(header.begin(),header.end(),data[i].main_criteria+data[i].secondary_criteria+" "+data[i].Harm_count);
        if(item==header.end())
        {
            header<<data[i].main_criteria+data[i].secondary_criteria+" "+data[i].Harm_count;
        }
    }

    header<<"测量时间"<<"加注";
    vertical_header<<"上限"<<"均值"<<"标准偏差"<<"量程"<<"最小"<<"最大"<<"";

    ui->tableWidget->setHorizontalHeaderLabels(header);//设置表头内容

    int RowCont=ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(RowCont);//增加一行

    count++;
    for(int i=1;i<=count;i++)
    {
        vertical_header<<QString::number(i);
    }

    for(int i=0;i<data.size();i++)
    {
        if(ui->tableWidget->horizontalHeaderItem(i)->text()==data[i].main_criteria+data[i].secondary_criteria+" "+data[i].Harm_count)
        {
            ui->tableWidget->setItem(RowCont,i ,new QTableWidgetItem(data[i].result));
        }
    }

    ui->tableWidget->setVerticalHeaderLabels(vertical_header);

    if(ui->tableWidget->horizontalHeaderItem(ui->tableWidget->columnCount()-2)->text()=="测量时间")
    {
        ui->tableWidget->setItem(RowCont,ui->tableWidget->columnCount()-2 ,new QTableWidgetItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
    }

    QVector<double> vec_table;
    float sum_data=0;
    QString str;
    float sum;
    float stddev;
    float stddev_;

    if(ui->tableWidget->rowCount()>7)
    {
        for(int j=0;j<ui->tableWidget->columnCount()-2;j++)
        {
            for(int i=7;i<ui->tableWidget->rowCount();i++)
            {
                str=ui->tableWidget->item(i,j)->text();
                qDebug()<<"i:"<<i<<"j:"<<j<<"str:"<<str<<"\n";

                sum_data+=str.toFloat();

                vec_table.push_back(str.toFloat());

            }

            auto num_min_=*(std::min_element(vec_table.begin(),vec_table.end()));
            ui->tableWidget->setItem(4,j ,new QTableWidgetItem(QString::number(num_min_,'f',3)));

            auto num_max_=*(std::max_element(vec_table.begin(),vec_table.end()));
            ui->tableWidget->setItem(5,j ,new QTableWidgetItem(QString::number(num_max_,'f',3)));

            auto range=num_max_-num_min_;
            ui->tableWidget->setItem(3,j ,new QTableWidgetItem(QString::number(range,'f',3)));

            auto average_value=sum_data/(ui->tableWidget->rowCount()-7);
            ui->tableWidget->setItem(1,j ,new QTableWidgetItem(QString::number(average_value,'f',3)));

            if(ui->tableWidget->rowCount()-7!=0)
            {
                stddev=1/(ui->tableWidget->rowCount()-7);
                for(int i=8;i<ui->tableWidget->rowCount();i++)
                {
                    str=ui->tableWidget->item(i,j)->text();


                    sum+=powf(str.toFloat()-average_value,2);

                }

                stddev_=sqrtf(stddev*sum);

                ui->tableWidget->setItem(2,j ,new QTableWidgetItem(QString::number(stddev_,'f',3)));

            }


            ui->tableWidget->setItem(0,j ,new QTableWidgetItem(data[j].reasonable_upper_limit));

//            qDebug()<<"data[j].reasonable_upper_limit:"<<data[j].reasonable_upper_limit;

            sum_data=0;
            vec_table.clear();
        }
    }

    ui->tableWidget->viewport()->update();

}

void statistical_report::recognition_data(recognition_config &data)
{
    ui->part_type_label->setText(data.part_type);
    ui->attribute_label->setText(data.attribute);
    ui->factory_label->setText(data.factory);
    ui->branch_label->setText(data.branch);

}
