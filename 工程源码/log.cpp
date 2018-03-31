#include "log.h"
#include "ui_log.h"
#include <QDateTime>
#include <QMessageBox>
//#include <QDebug>

//#define SELF_MODE//自己调试时使用(发布时注释掉此行)

Log::Log(QWidget *parent) :
    XlWidget(parent),
    ui(new Ui::Log)
{
    ui->setupUi(this);

#ifdef SELF_MODE//自己调试时使用
    ui->btnDel->setVisible(true);
#else//发布时使用
    ui->btnDel->setVisible(false);
#endif

    ui->tableLog->clear();//清空
    ui->tableLog->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不可编辑
    ui->tableLog->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选择行为时每次选择一行

#ifdef SELF_MODE//自己调试时使用
    ui->tableLog->setSelectionMode(QAbstractItemView::ExtendedSelection);//设置可以多选
#else//发布时使用
    ui->tableLog->setSelectionMode(QAbstractItemView::SingleSelection);//设置为只能选中单个目标(不能多选)
#endif

    ui->tableLog->setRowCount(0);
    ui->tableLog->setColumnCount(5);
    QStringList header;
    header<<"id"<<"日志内容"<<"登录的源IP"<<"登录用户名"<<"日志时间";
    ui->tableLog->setHorizontalHeaderLabels(header);
    ui->tableLog->setColumnHidden(0, true);//隐藏第0列(id列)
    ui->tableLog->setColumnWidth(1,500);//设置初始列宽
    ui->tableLog->setAutoScroll(true);//设置自动滚动条
    enable_table_auto_size();
    ui->tableLog->show();
    mysql = getMysqlObj();
    //loginUserInfo = getLoginUserInfoObj();
}

Log::~Log()
{
    delete ui;
}

void Log::load()
{
    refresh();
}

void Log::on_btnRefresh_clicked()
{
    if(refresh())
    {
        QMessageBox::about(NULL,"成功","刷新成功");
    }
    else
    {
        QMessageBox::about(NULL,"错误","刷新失败");
    }
}

bool Log::refresh()
{
    if(!mysql)
    {
        return false;//数据库连接失败
    }
    bool sqlResult = false;
    QSqlQuery sqlquery = mysql->query("select * from log order by datetime desc;",&sqlResult);
    if(!sqlResult)
    {
        return false;//sql语句执行出错
    }
    ui->tableLog->clearContents();//清空内容
    ui->tableLog->setRowCount(sqlquery.size());
    disable_table_auto_size();
    if(sqlquery.size() > 0)
    {
        int id = 0;
        QString content = "";
        QString source_ip = "";
        QString username = "";
        int datetime = 0;
        int index = 0;
        for(index=0;sqlquery.next();index++)
        {
            id = sqlquery.value("id").toInt();
            content = sqlquery.value("content").toString();
            source_ip = sqlquery.value("source_ip").toString();
            username = sqlquery.value("username").toString();
            datetime = sqlquery.value("datetime").toInt();
            ui->tableLog->setItem(index,0,new QTableWidgetItem(QString::number(id,10)));
            ui->tableLog->setItem(index,1,new QTableWidgetItem(content));
            ui->tableLog->setItem(index,2,new QTableWidgetItem(source_ip));
            ui->tableLog->setItem(index,3,new QTableWidgetItem(username));
            ui->tableLog->setItem(index,4,new QTableWidgetItem(QDateTime::fromSecsSinceEpoch(datetime).toString("yyyy-MM-dd HH:mm:ss")));
        }
    }
    enable_table_auto_size();
    return true;
}

void Log::on_btnDel_clicked()
{
    QList<int> selectRows = getSelectedRowsIndex(ui->tableLog);
    if(selectRows.count() == 0)
    {
        QMessageBox::about(NULL,"错误","未选中任何行");
        return;
    }
    if(!mysql)
    {
        QMessageBox::about(NULL,"错误","删除失败");
        return;
    }
    QMessageBox box(QMessageBox::Warning,"确认","是否确认删除所选日志");
    box.setStandardButtons (QMessageBox::Ok|QMessageBox::Cancel);
    box.setButtonText (QMessageBox::Ok,QString("确定"));
    box.setButtonText (QMessageBox::Cancel,QString("取消"));
    int boxResult = box.exec();
    if(boxResult == QMessageBox::Ok)
    {
        QString sql_str = "delete from log where";
        bool sqlResult = false;
        bool isFirst = true;
        int selectId = 0;
        foreach(int selectRow,selectRows)
        {
            selectId = ui->tableLog->item(selectRow, 0)->text().toInt();
            if(isFirst)
            {
                isFirst = false;
                sql_str += " id=" + QString::number(selectId,10);
            }
            else
            {
                sql_str += " or id=" + QString::number(selectId,10);
            }
        }
        sql_str += ";";
        mysql->query(sql_str,&sqlResult);
        if(!sqlResult)
        {
            QMessageBox::about(NULL,"错误","删除失败");//sql语句执行出错
            return;
        }
        refresh();
        QMessageBox::about(NULL,"成功","删除成功");
    }
}

void Log::enable_table_auto_size()
{
    ui->tableLog->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//设置根据内容调整列宽
    ui->tableLog->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Interactive);//设置可手动调整列宽
}

void Log::disable_table_auto_size()
{
    ui->tableLog->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);//设置可手动调整列宽
}
