#ifndef USER_H
#define USER_H

#include <QWidget>
#include "xlwidget.h"
#include "common.h"

namespace Ui {
class User;
}

class User : public XlWidget
{
    Q_OBJECT

public:
    explicit User(QWidget *parent = 0);
    ~User();

private slots:
    virtual void load();

    void on_btnModify_clicked();

    void on_btnReset_clicked();

private:
    Ui::User *ui;
    mysqlObj* mysql;
    loginUserInfoObj* loginUserInfo;
};

#endif // USER_H
