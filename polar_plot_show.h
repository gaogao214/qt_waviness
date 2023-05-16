#ifndef POLAR_PLOT_SHOW_H
#define POLAR_PLOT_SHOW_H

#include <QWidget>

#include "fft.h"
#include "chart.h"

#include <complex>
#include <qcustomplot.h>
#include <QContextMenuEvent>
#include "circle_property.h"
#include "property_of_curve.h"
#include "mouse_release_event.h"

#include <qwt_plot.h>

#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_legend.h>
#include <qwt_legend_label.h>

#include <qwt_plot_layout.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_renderer.h>
#include <qwt_legend.h>
#include <qwt_legend_label.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_histogram.h>
#include <qwt_column_symbol.h>
#include <qwt_series_data.h>
#include <qpen.h>
#include <stdlib.h>

#include <qwt_plot.h>
#include <qstringlist.h>
#include <qwt_plot_barchart.h>

#include <memory>
#include "profile.h"
#include <configuration.h>
#include "identify_area_attributes.h"
#include "histogram.h"
#include <qwt_plot_curve.h>
#include <qwt_polar_plot.h>

#include <qwt_polar_panner.h>
#include <qwt_plot_curve.h>

namespace Ui {
class polar_plot_show;
}

class PolarPlotShow : public QWidget
{
    Q_OBJECT

public:
    explicit PolarPlotShow(QWidget *parent = nullptr);
    ~PolarPlotShow();

    void custplotPropertyShow();

    void curveGraphShow(QVector<double> value);

    void circlePropertyShow();

    void histogramShow(QVector<double> value);

    void waviness(QVector<double> value);

    void custplotChartShow();

    void areaOfRemarksShow();

    void curveGraphMenu();

    void histogramMenu();

    void poleDiagramDisplayMenu();

    void identifyTheMenu();

    void resultsTheMenu();

    void areaOfRemarksMenu();

    void poleDiagramDisplay(QVector<double> value,QString range);

    void identifyAreaAttributesShow();

    void resultAreaAttributesShow();

    void startToPrepare();

    void maindowThreadData(QVector<double> displacement_data,QString range);

    void LeastSquaresCircle();

    void theResultsWereCalculatedAccordingToThePrimaryAndSecondaryCriteria(QVector<table_widget_band>& data);

    void barChart();

    void wavinessChart();

    void curveBightChart();

//    void polar_diagram(QString range);


private slots:

    void show_time();


    void on_chart_pushButton_clicked();

    void on_graph_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();



public slots:
    void recive_Display_Remarks(bool flag);

    void recive_Display_Time(bool flag);

    void recive_display_annotation(bool flag);

    void recive_display_time_speed(bool flag);

    void reciveResultAreaAttribute_(QVector<table_config> tab_config);

    void recive_show_type_of_spectrum(QString type);

    void recive_type_of_part(bool flag);

    void recive_attribute(bool flag);

    void recive_amount(bool flag);

    void recive_order(bool flag);

    void recive_client(bool flag);

    void recive_channel(bool flag);

    void recive_operator(bool flag);

    void recive_tor_number(bool flag);

    void recive_factor(bool flag);

    void recive_branch(bool flag);

    void recive_machine(bool flag);

    void recive_operate(bool flag);

    void recive_recogntion_config(recognition_config config);

    void recive_tolerance(bool flag);

    void recive_limit_value(bool flag);

    void recive_header(bool flag);

    void recive_control_decimal_place(int);

    void recive_show_x_barchart(int data);

    void recive_show_y_barchart(QString data);

    void polar_diagram(QString range);


private:
    Ui::polar_plot_show *ui;

    //声明动作
    QAction * ui_customplot_chart_;
    QAction * ui_customplot_property_;
    QAction * ui_customplot_export_data_;

    //声明菜单
    QMenu * result_menu_;
    QMenu * remark_menu_;
    QMenu * customplot_menu_;
    QMenu * pole_figure_menu_;
    QMenu * identification_menu_;



    QVector<double> curve_x_,curve_y_; //初始化向量x和y
    QVector<double> histogram_x_,histogram_y_; //初始化向量x和y

    QCPPolarGraph *g1_;
    QCPPolarGraph *g2_;
    QCPPolarGraph *g3_;
    QCPPolarGraph *g4_;
    QCPPolarAxisAngular *angularAxis_;

    QLabel* show_time_label_;
    QLabel* show_part_count_label_;

    QCPBars *bars_ ;
    QCPAxis *xAxis_ ;
    QCPAxis *yAxis_ ;


    QCPBars *waviness_bars_ ;
    QCPAxis *waviness_xAxis_ ;
    QCPAxis *waviness_yAxis_ ;

    QProgressBar* progressBar;

    QVector<double> waviness_x_,waviness_y_; //初始化向量x和y


    int part_count_=0;// 零件计数
    int curve_count_=0;   //曲线 点 计数
    int circle_count_=0; //极图 圆形 点 计数
    int columns_count_=0; //柱形 点 计数

    std::unique_ptr<circle_property_> config_circle_=nullptr;
    std::unique_ptr<property_of_curve> config_remarks_=nullptr;
    std::unique_ptr<identify_area_attributes> config_identify =nullptr;
    fft fft_;

    double num_min_;
    double num_max_;

    int spectrogram_count_;

    QString str_show_result_;
    QVector<QString> vec_show_result_;

public:
    double circle_x_=0.0,circle_y_=0.0,circle_r_=0.0;

    //    std::unique_ptr<configuration> configuration_=nullptr;
    //    configuration *configuration_temp;
    std::unique_ptr<histogram> histogram_chart_=nullptr;
    std::unique_ptr<histogram> waviness_chart_=nullptr;
    QVector<double> amplitude_;
    int rotate_speed=700;

    std::unique_ptr<QwtPolarPlot> plot=nullptr;

    QVector<table_config> vec_tab_config;
    QVector<table_config> formdoc_tablewidget_data_;

    recognition_config widget_config_;

};

#endif // POLAR_PLOT_SHOW_H
