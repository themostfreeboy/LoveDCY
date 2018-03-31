#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>
#include "cselftitledlg.h"
#include "common.h"

namespace Ui {
class LoginDlg;
}

class LoginDlg : public CSelfTitleDlg
{
    Q_OBJECT

public:
    explicit LoginDlg(QWidget *parent = 0);
    ~LoginDlg();

private slots:
    void on_btnLogin_clicked();

    void on_btnReset_clicked();

private:
    Ui::LoginDlg *ui;
    mysqlObj* mysql;
    loginUserInfoObj* loginUserInfo;
};

#endif // LOGINDLG_H
