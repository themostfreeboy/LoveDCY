#ifndef NOTEADDDLG_H
#define NOTEADDDLG_H

#include <QDialog>
#include "cselftitledlg.h"

namespace Ui {
class NoteAddDlg;
}

class NoteAddDlg : public CSelfTitleDlg
{
    Q_OBJECT

public:
    explicit NoteAddDlg(QWidget *parent = 0);
    ~NoteAddDlg();

private:
    Ui::NoteAddDlg *ui;

public:
    QString content;
private slots:
    void on_btnSave_clicked();
    void on_btnCancel_clicked();
};

#endif // NOTEADDDLG_H
