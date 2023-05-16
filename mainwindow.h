#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qcustomplot.h>
#include <QContextMenuEvent>

#include "new_profile.h"
#include "configuration.h"
#include "polar_plot_show.h"
#include "circle_property.h"
#include "property_of_curve.h"
#include "mouse_release_event.h"
#include "statistical_report.h"
#include "calibration.h"
#include "function_key.h"
#include "measurement_task.h"

#ifdef __cplusplus
extern "C"{

#include "modbusclient.h"
#include "addidata_network.h"
#include "msxe371x_modbus_clientlib.h"

#ifdef __cplusplus
}
#endif
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

static constexpr int k_measurement_point=7200;
static constexpr char* k_ip="10.2.1.12";
static constexpr uint16_t k_port=512;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:

    int sampleMX371xTransducerGetAutoRefreshValuesEx(struct modbus * modbus, int NumberOfChannels);

    int sampleMX371xTransducerInitAndStartAutoRefreshEx(struct modbus * modbus);

    int sampleMX371xTransducerStopAndReleaseAutoRefreshEx(struct modbus * modbus);

    static const char * typestr(uint32_t type);

    int sampleMX371xTransducerGetNbrOfTypeEx(struct modbus * modbus, uint32_t* NumberOfTransducerTypes);

    int sampleMX371xGetTransducerDatabaseCursorEx(struct modbus * modbus, uint32_t* TransducerDatabaseCursor);

    int sampleMX371xTransducerGetTypeInformationEx(struct modbus * modbus, struct MX371x__TransducerGetTypeInformationEx_parameters_t* TransducerGetTypeInformation);

    int sampleMX371xSetTransducerDatabaseCursorEx(struct modbus * modbus, uint32_t new_);

    int dumpTransducerDatabase(struct modbus * modbus);

    void connectingDevice();

    void counterInput();

    int sample_MSXE371x__IncCounterInit(struct modbus * modbus);

    int sample_MSXE371x__IncCounterClear(struct modbus * modbus);

    int sample_MSXE371x__IncCounterWrite32BitValue(struct modbus * modbus);

    int sample_MSXE371x__IncCounterRead32BitsValue(struct modbus * modbus, uint32_t *pulValue, uint32_t *pulTimeStampLow, uint32_t *pulTimeStampHigh);

    int sample_MSXE371x__IncCounterRelease(struct modbus * modbus);

    void windowtitle(QString title);

    void threadGetMSXdata();





private slots:

    void on_start_measuring_triggered();

    void show_time();

    void graph_pushButton_clicked();

    void pole_diagram_pushButton_clicked();

    void on_new_profile_triggered();

    void on_measurement_task_triggered();

    void on_exit_triggered();

    void configurePopover();

    void on_config_action1_triggered();

    void configuration_Button_clicked();

    void on_action1_12_triggered();

    void on_action1_13_triggered();

    void on_action1_28_triggered();

    void pushButton_4_clicked();


    void on_action1_25_triggered();

    void on_action1_26_triggered();

    void pushButton_10_clicked();

    void on_action1_9_triggered();

    void on_action12_4_triggered();

    void recive_calibration();

    void on_comboBox_2_activated(const QString &arg1);

    void comboBox_2_show(QString);

    void calibration_quit();

    void on_action1_10_triggered();

    void on_action1_triggered();



private:

    Ui::MainWindow *ui;

    int part_count_=0;// 零件计数

    struct modbus  modbus_;

    QString filename_mwa_=nullptr;

    QLabel* show_time_label_;
    QLabel* show_part_count_label_;
    QLabel* show_connect_information_;

    QVector<double> displacement_data_;

    std::shared_ptr<new_profile> config_graph_=nullptr;

    std::unique_ptr<configuration> config_configure_=nullptr;
    std::unique_ptr<statistical_report> report_=nullptr;

    std::unique_ptr<PolarPlotShow> config_polar_plot_=nullptr;

    std::unique_ptr<calibration> calibration_=nullptr;

    std::unique_ptr<function_key> function_key_=nullptr;

    std::unique_ptr<measurement_task> config_task_=nullptr;

public:
    QString transducer_name_;
    QString profile_fileName_;




};
#endif // MAINWINDOW_H
