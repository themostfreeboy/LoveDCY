#include "note.h"
#include "ui_note.h"
#include <QDatetime>
#include "noteadddlg.h"
#include "notemodifydlg.h"
#include <QMessageBox>
//#include <QDebug>

Note::Note(QWidget *parent) :
    XlWidget(parent),
    ui(new Ui::Note)
{
    ui->setupUi(this);
    aes = new myAES("dcylovejxl");
    ui->tableNote->clear();//清空
    ui->tableNote->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不可编辑
    ui->tableNote->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选择行为时每次选择一行
    //ui->tableNote->setSelectionMode(QAbstractItemView::SingleSelection);//设置为只能选中单个目标(不能多选)
    ui->tableNote->setSelectionMode(QAbstractItemView::ExtendedSelection);//设置可以多选
    ui->tableNote->setRowCount(0);
    ui->tableNote->setColumnCount(6);
    QStringList header;
    header<<"id"<<"登录用户名"<<"内容"<<"登录的源IP"<<"创建时间"<<"修改时间";
    ui->tableNote->setHorizontalHeaderLabels(header);
    ui->tableNote->setColumnHidden(0, true);//隐藏第0列(id列)
    ui->tableNote->setColumnHidden(1, true);//隐藏第1列(登录用户名列)
    ui->tableNote->setColumnWidth(2,500);//设置初始列宽
    ui->tableNote->setAutoScroll(true);//设置自动滚动条
    enable_table_auto_size();
    ui->tableNote->show();
    mysql = getMysqlObj();
    loginUserInfo = getLoginUserInfoObj();
}

Note::~Note()
{
    if(aes)
    {
        delete aes;
        aes = NULL;
    }
    delete ui;
}

void Note::load()
{
    refresh();
}

void Note::refresh()
{
    if(!mysql)
    {
        return;//数据库连接失败
    }
    if(!loginUserInfo)
    {
        return;//获取登录用户数据对象失败
    }
    bool sqlResult = false;
    QSqlQuery sqlquery = mysql->query("select * from note where username='" + loginUserInfo->name + "' order by modify_time desc,create_time desc;",&sqlResult);
    if(!sqlResult)
    {
        return;//sql语句执行出错
    }
    ui->tableNote->clearContents();//清空内容
    ui->tableNote->setRowCount(sqlquery.size());
    disable_table_auto_size();
    if(sqlquery.size() > 0)
    {
        int id = 0;
        QString username = "";
        QString content = "";
        QString source_ip = "";
        int create_time = 0;
        int modify_time = 0;
        int index = 0;
        for(index=0;sqlquery.next();index++)
        {
            id = sqlquery.value("id").toInt();
            username = sqlquery.value("username").toString();
            content = aes->myDecrypt(sqlquery.value("content").toString());
            if(content.isNull())
            {
                content = "";
            }
            source_ip = sqlquery.value("source_ip").toString();
            create_time = sqlquery.value("create_time").toInt();
            modify_time = sqlquery.value("modify_time").toInt();
            ui->tableNote->setItem(index,0,new QTableWidgetItem(QString::number(id,10)));
            ui->tableNote->setItem(index,1,new QTableWidgetItem(username));
            ui->tableNote->setItem(index,2,new QTableWidgetItem(content));
            ui->tableNote->setItem(index,3,new QTableWidgetItem(source_ip));
            ui->tableNote->setItem(index,4,new QTableWidgetItem(QDateTime::fromSecsSinceEpoch(create_time).toString("yyyy-MM-dd HH:mm:ss")));
            ui->tableNote->setItem(index,5,new QTableWidgetItem(QDateTime::fromSecsSinceEpoch(modify_time).toString("yyyy-MM-dd HH:mm:ss")));
        }
    }
    enable_table_auto_size();
}

void Note::on_btnAdd_clicked()
{
    if(!mysql)
    {
        QMessageBox::about(NULL,"错误","新增失败");//数据库连接失败
        return;
    }
    if(!loginUserInfo)
    {
        QMessageBox::about(NULL,"错误","新增失败");//获取登录用户信息对象失败
        return;
    }
    NoteAddDlg wNoteAddDlg;
    int dlgResult = wNoteAddDlg.exec();
    if (dlgResult == QDialog::Accepted)
    {
        bool sqlResult = false;
        unsigned int temp_time = (unsigned int)QDateTime::currentSecsSinceEpoch();
        QString sql_str = "insert into note (username,content,source_ip,create_time,modify_time) values ('";
        sql_str += loginUserInfo->name + "','";
        sql_str += aes->myEncrypt(wNoteAddDlg.content.trimmed()) + "','";
        sql_str += mysql->localIP + "',";
        sql_str += QString::number(temp_time,10) + ",";
        sql_str += QString::number(temp_time,10) + ");";
        mysql->query(sql_str,&sqlResult);
        if(!sqlResult)
        {
            insert_log("新增备忘录，但新增失败");
            QMessageBox::about(NULL,"错误","新增失败");//sql语句执行出错
            return;
        }
        refresh();
        insert_log("新增备忘录，且新增成功");
        QMessageBox::about(NULL,"成功","新增成功");
    }
}

void Note::on_btnModify_clicked()
{
    QList<int> selectRows = getSelectedRowsIndex(ui->tableNote);
    if(selectRows.count() == 0)
    {
        QMessageBox::about(NULL,"错误","未选中任何行");
        return;
    }
    if(selectRows.count() != 1)
    {
        QMessageBox::about(NULL,"错误","查看与修改只能选中一行");
        return;
    }
    if(!mysql)
    {
        QMessageBox::about(NULL,"错误","查看与修改失败");
        return;
    }
    if(!loginUserInfo)
    {
        QMessageBox::about(NULL,"错误","查看与修改失败");//获取登录用户信息对象失败
        return;
    }
    int selectRow = selectRows.at(0);
    int selectId = ui->tableNote->item(selectRow, 0)->text().toInt();
    QString selectContent =  ui->tableNote->item(selectRow, 2)->text();
    NoteModifyDlg wNoteModifyDlg;
    wNoteModifyDlg.content = selectContent;
    wNoteModifyDlg.init_data();
    int dlgResult = wNoteModifyDlg.exec();
    if(dlgResult == QDialog::Accepted)
    {
        bool sqlResult = false;
        unsigned int temp_time = (unsigned int)QDateTime::currentSecsSinceEpoch();
        QString sql_str = "update note set content='" + aes->myEncrypt(wNoteModifyDlg.content) + "'";
        sql_str += ",source_ip='"+ mysql->localIP +"'";
        sql_str += ",modify_time=" + QString::number(temp_time,10);
        sql_str += " where id=" + QString::number(selectId,10) + ";";
        mysql->query(sql_str,&sqlResult);
        if(!sqlResult)
        {
            insert_log("修改备忘录，但修改失败");
            QMessageBox::about(NULL,"错误","修改失败");//sql语句执行出错
            return;
        }
        refresh();
        insert_log("修改备忘录，且修改成功");
        QMessageBox::about(NULL,"成功","修改成功");
    }
}

void Note::on_btnDelete_clicked()
{
    QList<int> selectRows = getSelectedRowsIndex(ui->tableNote);
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
    QMessageBox box(QMessageBox::Warning,"确认","是否确认删除所选备忘录");
    box.setStandardButtons (QMessageBox::Ok|QMessageBox::Cancel);
    box.setButtonText (QMessageBox::Ok,QString("确定"));
    box.setButtonText (QMessageBox::Cancel,QString("取消"));
    int boxResult = box.exec();
    if(boxResult == QMessageBox::Ok)
    {
        QString sql_str = "delete from note where";
        bool sqlResult = false;
        bool isFirst = true;
        int selectId = 0;
        foreach(int selectRow,selectRows)
        {
            selectId = ui->tableNote->item(selectRow, 0)->text().toInt();
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
            insert_log("删除备忘录，但删除失败");
            QMessageBox::about(NULL,"错误","删除失败");//sql语句执行出错
            return;
        }
        insert_log("删除备忘录，且删除成功");
        refresh();
        QMessageBox::about(NULL,"成功","删除成功");
    }
}

void Note::enable_table_auto_size()
{
    ui->tableNote->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//设置根据内容调整列宽
    ui->tableNote->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Interactive);//设置可手动调整列宽
}

void Note::disable_table_auto_size()
{
    ui->tableNote->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);//设置可手动调整列宽
}
