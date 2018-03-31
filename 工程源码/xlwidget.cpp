#include "xlwidget.h"
//#include <QMessageBox>
//#include <QDebug>

XlWidget::XlWidget(QWidget *parent) : QWidget(parent)
{
    //处理信号
    connect(this, load_msg, this, load);
}
