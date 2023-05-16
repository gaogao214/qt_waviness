#include "curve_grap_show.h"
#include "ui_curve_grap_show.h"

curve_grap_show::curve_grap_show(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::curve_grap_show)
{
    ui->setupUi(this);
}

curve_grap_show::~curve_grap_show()
{
    delete ui;
}
