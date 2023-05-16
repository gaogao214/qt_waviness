#ifndef MOUSE_RELEASE_EVENT_H
#define MOUSE_RELEASE_EVENT_H

#include <QObject>
#include <QPushButton>
#include <QMouseEvent>
#include "qcustomplot.h"

class mouse_release_event:public QCustomPlot
{
    Q_OBJECT


public:
     mouse_release_event();
     ~mouse_release_event();

signals:
     void right_mouse();

protected:
     void  mouse_event(QMouseEvent *event);

public:
         QMenu *m_contextMenu_;
         QAction *m_addAction_;
         QAction *m_delAction_;

};

#endif // MOUSE_RELEASE_EVENT_H
