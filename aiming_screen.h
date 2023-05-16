#ifndef AIMING_SCREEN_H
#define AIMING_SCREEN_H

#include <QWidget>

namespace Ui {
class aiming_screen;
}

class aiming_screen : public QWidget
{
    Q_OBJECT

public:
    explicit aiming_screen(QWidget *parent = nullptr);
    ~aiming_screen();

private:
    Ui::aiming_screen *ui;
};

#endif // AIMING_SCREEN_H
