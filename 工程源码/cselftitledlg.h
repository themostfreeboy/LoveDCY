#ifndef CSELFTITLEDLG_H
#define CSELFTITLEDLG_H

#include <QDialog>
#include "titlebar.h"

class CSelfTitleDlg : public QDialog
{


public:
    explicit CSelfTitleDlg(QWidget *parent = NULL);
    ~CSelfTitleDlg();

protected:
    TitleBar *m_pTitleBar;

protected:
    void init(int iFlag = TITLE_DEF_FLAG);
};

#endif // CSELFTITLEDLG_H
