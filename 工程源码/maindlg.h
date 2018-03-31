#ifndef MAINDLG_H
#define MAINDLG_H

#include <QDialog>
#include "cselftitledlg.h"
#include "common.h"
#include "log.h"
#include "note.h"
#include "user.h"

namespace Ui {
class MainDlg;
}

class MainDlg : public CSelfTitleDlg
{
    Q_OBJECT

public:
    explicit MainDlg(QWidget *parent = 0);
    ~MainDlg();

private slots:
    void on_tabWidget_currentChanged(int index);

private:
    Ui::MainDlg *ui;
    loginUserInfoObj* loginUserInfo;
    Log m_wLog;//日志
    Note m_wNote;//备忘录
    User m_wUser;//用户设置
};

#endif // MAINDLG_H
