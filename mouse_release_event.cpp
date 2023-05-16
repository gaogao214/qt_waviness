#include "mouse_release_event.h"

mouse_release_event::mouse_release_event()
{
    m_contextMenu_ = new QMenu;
      m_addAction_ = new QAction("add Item",this);
      m_delAction_ = new QAction("del Item",this);
      m_contextMenu_->addAction(m_addAction_);
      m_contextMenu_->addAction(m_delAction_);



}

mouse_release_event::~mouse_release_event()
{


}

void mouse_release_event::mouse_event(QMouseEvent *event)
{
    //确保右键点击，然后跳出菜单.
       if (event->button() == Qt::RightButton)
       {
           emit right_mouse();
           m_contextMenu_->exec(event->globalPos());
       }
       //要继续保留QListWidget原有的点击事件.


}
