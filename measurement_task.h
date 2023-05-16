#ifndef MEASUREMENT_TASK_H
#define MEASUREMENT_TASK_H

#include <QWidget>
#include <QListWidgetItem>

#include "configuration.h"
namespace Ui {
class measurement_task;
}

class measurement_task : public QWidget
{
    Q_OBJECT

public:
    explicit measurement_task(QWidget *parent = nullptr);
    ~measurement_task();

    void inputCharComboBox(const QString &str);

    void inputCharListWidget(const QString &str);


private slots:
    void on_pushButton_clicked();

    void on_file_pashbutton_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_listWidget_currentTextChanged(const QString &currentText);

    void on_listWidget_2_currentTextChanged(const QString &currentText);

    void on_pushButton_3_clicked();

    void on_lineEdit_textEdited(const QString &arg1);

    void on_listWidget_2_itemDoubleClicked(QListWidgetItem *item);

signals:
    void down_ok();

    void signal_profile_file(QString );

private:
    Ui::measurement_task *ui;

    QString path_name_;
    QString pitch_on_listwidget_;
    QVector<QString> listwidget_str_;

public:
    std::unique_ptr<configuration> config_configure_=nullptr;
};

#endif // MEASUREMENT_TASK_H
