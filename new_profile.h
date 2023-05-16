#ifndef NEW_PROFILE_H
#define NEW_PROFILE_H

#include <QWidget>
#include "qcustomplot.h"
#include "property_of_curve.h"

static constexpr int measurement_=100;
namespace Ui {
class new_profile;
}

class new_profile : public QWidget
{
    Q_OBJECT

public:
    explicit new_profile(QWidget *parent = nullptr);
    ~new_profile();


    //显示曲线图
    void curve_Graph_Show(double value);

    //显示柱形图
    void histogram_Show(double value);

    //曲线区域子菜单
    void curve_Graph_Menu();

    //柱状图区域子菜单
    void histogram_Menu();

    //属性
    void custplot_Property_Show();

private slots:
    void on_his_button_clicked();

    void on_curve_button_clicked();

private:
    Ui::new_profile *ui;


    QCPAxis *xAxis_ ;
    QCPAxis *yAxis_ ;
    QCPBars *bars_ ;

    //声明动作
    QAction * ui_customplot_property_;
    QAction * ui_customplot_chart_;
    QAction * ui_customplot_export_data_;

    //声明菜单
    QMenu * customplot_menu_;
    QMenu * identification_menu_;
    QMenu * pole_figure_menu_;
    QMenu * result_menu_;
    QMenu * remark_menu_;



    QVector<double> curve_x_,curve_y_; //初始化向量x和y
    QVector<double> histogram_x_,histogram_y_; //初始化向量x和y
    QCPPolarGraph *g1_;
    QCPPolarAxisAngular *angularAxis_;

    QLabel* show_time_label_;
    QLabel* show_part_count_label_;

    int part_count_=0;// 零件计数

    int curve_count_=0;   //曲线 点 计数
    int columns_count_=0; //柱形 点 计数
    int circle_count_=0; //极图 圆形 点 计数


    property_of_curve *config_remarks_;


};

#endif // NEW_PROFILE_H
