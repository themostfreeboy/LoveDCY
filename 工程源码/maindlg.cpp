#include "maindlg.h"
#include "ui_maindlg.h"
//#include <QMessageBox>
//#include <QDebug>

MainDlg::MainDlg(QWidget *parent) :
    CSelfTitleDlg(parent),
    ui(new Ui::MainDlg)
{
    ui->setupUi(this);
    init();
    loginUserInfo = getLoginUserInfoObj();
    if(!loginUserInfo)
    {
        return;//获取登陆用户信息对象失败
    }
    if(loginUserInfo->role == 0)//管理员
    {
        ui->tabWidget->addTab(&m_wNote, "备忘录");
        ui->tabWidget->addTab(&m_wLog, "日志");
        ui->tabWidget->addTab(&m_wUser, "用户设置");
    }
    else//普通用户
    {
        ui->tabWidget->addTab(&m_wNote, "备忘录");
        ui->tabWidget->addTab(&m_wUser, "用户设置");
    }
}

MainDlg::~MainDlg()
{
    while(ui->tabWidget->count() > 0)
    {
        ui->tabWidget->removeTab(0);
    }
    delete ui;
}

void MainDlg::on_tabWidget_currentChanged(int index)
{
    if(index == -1)
    {
        return;
    }
    QWidget *pWidget = ui->tabWidget->widget(index);
    if((dynamic_cast<XlWidget *>(pWidget)) != nullptr)
    {
       emit(dynamic_cast<XlWidget *>(pWidget))->load_msg();
    }
}
