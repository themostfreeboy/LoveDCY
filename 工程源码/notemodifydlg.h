#ifndef NOTEMODIFYDLG_H
#define NOTEMODIFYDLG_H

#include <QDialog>
#include "cselftitledlg.h"

namespace Ui {
class NoteModifyDlg;
}

class NoteModifyDlg : public CSelfTitleDlg
{
    Q_OBJECT

public:
    explicit NoteModifyDlg(QWidget *parent = 0);
    ~NoteModifyDlg();

private:
    Ui::NoteModifyDlg *ui;

public:
    QString content;
    void init_data();
private slots:
    void on_btnSave_clicked();
    void on_btnCancel_clicked();
};

#endif // NOTEMODIFYDLG_H
