#include "cselftitledlg.h"
#include <QVBoxLayout>
#include <QCoreApplication>
#include <QDir>
#include <QTextStream>
//#include <QMessageBox>
//#include <QDebug>

CSelfTitleDlg::CSelfTitleDlg(QWidget *parent) :
    QDialog(parent)
{
    return;
}

CSelfTitleDlg::~CSelfTitleDlg()
{
    return;
}


void CSelfTitleDlg::init(int iFlag)
{
    QString strQssFile = QCoreApplication::applicationDirPath() + QDir::separator() + "qss" + QDir::separator() + "main.qss";
    if (QFileInfo(strQssFile).exists())
    {
        QFile fQss(strQssFile);
        fQss.open(QFile::ReadOnly);
        QTextStream tsQss(&fQss);
        QString strQss = tsQss.readAll();
        this->setStyleSheet(strQss);
        fQss.close();
    }

    // 去掉问号与标题栏
    Qt::WindowFlags dwFags = this->windowFlags();
    dwFags |= Qt::FramelessWindowHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint;
    this->setWindowFlags(dwFags);

    m_pTitleBar = new TitleBar(this);
    m_pTitleBar->init(iFlag);
    installEventFilter(m_pTitleBar);

    QLayout *pLayout = this->layout();
    if (!pLayout)
    {
        pLayout = new QVBoxLayout();
    }
    pLayout->setMenuBar(m_pTitleBar);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);

    //设置左上角窗口图标和窗口标题
    this->setWindowIcon(QIcon(QCoreApplication::applicationDirPath() + QDir::separator() + "img" + QDir::separator() + "program.ico"));
    this->setWindowTitle("爱瑜");

    if (iFlag & TITLE_MAX_FLAG)
    {
       this->setWindowState(this->windowState() | Qt::WindowMaximized);//让窗体默认为最大化状态
    }
}
