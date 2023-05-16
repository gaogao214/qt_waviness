#include "aiming_screen.h"
#include "ui_aiming_screen.h"

aiming_screen::aiming_screen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::aiming_screen)
{
    ui->setupUi(this);
}

aiming_screen::~aiming_screen()
{
    delete ui;
}
