#ifndef LOG_H
#define LOG_H

#include <QWidget>
#include "xlwidget.h"
#include "common.h"

namespace Ui {
class Log;
}

class Log : public XlWidget
{
    Q_OBJECT

public:
    explicit Log(QWidget *parent = 0);
    ~Log();

private slots:
    virtual void load();

    void on_btnRefresh_clicked();

    void on_btnDel_clicked();

private:
    Ui::Log *ui;
    mysqlObj* mysql;
    //loginUserInfoObj* loginUserInfo;
    bool refresh();
    void enable_table_auto_size();
    void disable_table_auto_size();
};

#endif // LOG_H
