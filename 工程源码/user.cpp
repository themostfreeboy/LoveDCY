#include "user.h"
#include "ui_user.h"
#include <QMessageBox>
//#include <QDebug>

User::User(QWidget *parent) :
    XlWidget(parent),
    ui(new Ui::User)
{
    ui->setupUi(this);
    mysql = getMysqlObj();
    loginUserInfo = getLoginUserInfoObj();
    if(!loginUserInfo)
    {
        return;//获取登录用户信息对象失败
    }
    ui->lineName->setText(loginUserInfo->name);
    if(QString::compare(loginUserInfo->name,"jxl",Qt::CaseInsensitive) == 0)
    {
        ui->linePassword->setEnabled(false);
        ui->lineNewPassword->setEnabled(false);
        ui->lineNewPasswordConfirm->setEnabled(false);
        ui->btnModify->setEnabled(false);
        ui->btnReset->setEnabled(false);
        ui->labelTip->setText("jxl为超级管理员，不允许修改密码");
    }
}

User::~User()
{
    delete ui;
}

void User::load()
{
    ui->linePassword->clear();
    ui->lineNewPassword->clear();
    ui->lineNewPasswordConfirm->clear();
}

void User::on_btnModify_clicked()
{
    if(!mysql)
    {
        ui->linePassword->clear();
        ui->lineNewPassword->clear();
        ui->lineNewPasswordConfirm->clear();
        QMessageBox::about(NULL,"错误","数据库连接失败");//数据库连接失败
        return;
    }
    if(!loginUserInfo)
    {
        ui->linePassword->clear();
        ui->lineNewPassword->clear();
        ui->lineNewPasswordConfirm->clear();
        QMessageBox::about(NULL,"错误","修改失败");
        return;//获取登录用户信息对象失败
    }
    if(QString::compare(ui->lineNewPassword->text().trimmed(),ui->lineNewPasswordConfirm->text().trimmed(),Qt::CaseInsensitive) != 0)
    {
        ui->linePassword->clear();
        ui->lineNewPassword->clear();
        ui->lineNewPasswordConfirm->clear();
        insert_log("修改密码，但由于两次输入的新密码不一致，修改失败");
        QMessageBox::about(NULL,"错误","两次输入的新密码不一致，修改失败");
        return;
    }
    bool result = false;
    QString sql = "select * from user where id=" + QString::number(loginUserInfo->id,10) + " and password='" + calMD5(ui->linePassword->text().trimmed()) + "';";
    QSqlQuery sqlquery = mysql->query(sql,&result);
    if(!result)
    {
        ui->linePassword->clear();
        ui->lineNewPassword->clear();
        ui->lineNewPasswordConfirm->clear();
        insert_log("修改密码，但修改失败");
        QMessageBox::about(NULL,"错误","修改失败");//sql语句执行出错
        return;
    }
    if(sqlquery.size()!=1)
    {
        ui->linePassword->clear();
        ui->lineNewPassword->clear();
        ui->lineNewPasswordConfirm->clear();
        insert_log("修改密码，但由于原密码输入错误，修改失败");
        QMessageBox::about(NULL,"错误","原密码输入错误，修改失败");
        return;
    }
    sql = "update user set password='" + calMD5(ui->lineNewPassword->text().trimmed()) + "' where id=" + QString::number(loginUserInfo->id,10) + ";";
    mysql->query(sql,&result);
    if(!result)
    {
        ui->linePassword->clear();
        ui->lineNewPassword->clear();
        ui->lineNewPasswordConfirm->clear();
        insert_log("修改密码，但修改失败");
        QMessageBox::about(NULL,"错误","修改失败");//sql语句执行出错
        return;
    }
    ui->linePassword->clear();
    ui->lineNewPassword->clear();
    ui->lineNewPasswordConfirm->clear();
    insert_log("修改密码，且修改成功");
    QMessageBox::about(NULL,"错误","修改成功");
}

void User::on_btnReset_clicked()
{
    ui->linePassword->clear();
    ui->lineNewPassword->clear();
    ui->lineNewPasswordConfirm->clear();
}
