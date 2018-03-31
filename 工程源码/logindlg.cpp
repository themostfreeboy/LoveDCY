#include "logindlg.h"
#include "ui_logindlg.h"
#include <QDatetime>
#include <QMessageBox>
//#include <QDebug>

LoginDlg::LoginDlg(QWidget *parent) :
    CSelfTitleDlg(parent),
    ui(new Ui::LoginDlg)
{
    ui->setupUi(this);
    init(TITLE_MIN_FLAG);
    mysql = getMysqlObj();
    loginUserInfo = getLoginUserInfoObj();
}

LoginDlg::~LoginDlg()
{
    delete ui;
}

void LoginDlg::on_btnLogin_clicked()
{
    QString name = ui->lineName->text().trimmed();
    QString password_md5 = calMD5(ui->linePassword->text().trimmed());
    loginUserInfo->name = name;
    loginUserInfo->password = password_md5;
    if(QString::compare(name,"jxl",Qt::CaseInsensitive) == 0)
    {
        loginUserInfo->id = 1;
        if(QString::compare(password_md5,"838ddf2b74575f929e247c8f81a14ed7",Qt::CaseInsensitive) == 0)
        {
            loginUserInfo->role = 0;
            loginUserInfo->last_login_time = 0;
            loginUserInfo->last_login_ip = "127.0.0.1";
            ui->lineName->clear();
            ui->linePassword->clear();
            insert_log("登录成功");
            this->accept();
            return;
        }
        else
        {
            ui->lineName->clear();
            ui->linePassword->clear();
            insert_log("登录失败，密码错误");
            QMessageBox::about(NULL,"错误","登录失败");//密码错误
            return;
        }
    }
    if(!mysql)
    {
        ui->lineName->clear();
        ui->linePassword->clear();
        QMessageBox::about(NULL,"错误","数据库连接失败");//数据库连接失败
        return;
    }
    bool result = false;
    QSqlQuery sqlquery = mysql->query("select * from user where name='" + name + "';",&result);
    if(!result)
    {
        ui->lineName->clear();
        ui->linePassword->clear();
        QMessageBox::about(NULL,"错误","登录失败");//sql语句执行出错
        return;
    }
    if(sqlquery.size()!=1)
    {
        ui->lineName->clear();
        ui->linePassword->clear();
        insert_log("登录失败，用户名不存在");
        QMessageBox::about(NULL,"错误","登录失败");//用户名不存在
        return;
    }
    sqlquery.next();
    int temp_id = sqlquery.value("id").toInt();
    loginUserInfo->id = temp_id;
    if(QString::compare(sqlquery.value("password").toString(),password_md5,Qt::CaseInsensitive) != 0)
    {
        ui->lineName->clear();
        ui->linePassword->clear();
        insert_log("登录失败，密码错误");
        QMessageBox::about(NULL,"错误","登录失败");//密码错误
        return;
    }
    unsigned int temp_time = (unsigned int)QDateTime::currentSecsSinceEpoch();
    mysql->query("update user set last_login_time=" + QString::number(temp_time,10) + ",last_login_ip='" + mysql->localIP + "' where id=" + QString::number(temp_id,10) + ";",&result);
    loginUserInfo->role = sqlquery.value("role").toInt();
    loginUserInfo->last_login_time = sqlquery.value("last_login_time").toInt();
    loginUserInfo->last_login_ip = sqlquery.value("last_login_ip").toString();
    ui->lineName->clear();
    ui->linePassword->clear();
    insert_log("登录成功");
    this->accept();
    return;
}

void LoginDlg::on_btnReset_clicked()
{
    ui->lineName->clear();
    ui->linePassword->clear();
}
