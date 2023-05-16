#ifndef CURVE_GRAP_SHOW_H
#define CURVE_GRAP_SHOW_H

#include <QWidget>

namespace Ui {
class curve_grap_show;
}

class curve_grap_show : public QWidget
{
    Q_OBJECT

public:
    explicit curve_grap_show(QWidget *parent = nullptr);
    ~curve_grap_show();

private:
    Ui::curve_grap_show *ui;
};

#endif // CURVE_GRAP_SHOW_H
