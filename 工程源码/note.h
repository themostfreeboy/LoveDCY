#ifndef NOTE_H
#define NOTE_H

#include <QWidget>
#include "xlwidget.h"
#include "common.h"

namespace Ui {
class Note;
}

class Note : public XlWidget
{
    Q_OBJECT

public:
    explicit Note(QWidget *parent = 0);
    ~Note();

private slots:
    virtual void load();

    void on_btnAdd_clicked();

    void on_btnModify_clicked();

    void on_btnDelete_clicked();

private:
    Ui::Note *ui;
    mysqlObj* mysql;
    loginUserInfoObj* loginUserInfo;
    myAES* aes;
    void refresh();
    void enable_table_auto_size();
    void disable_table_auto_size();
};

#endif // NOTE_H
