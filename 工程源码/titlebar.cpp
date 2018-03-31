#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QStyle>
#include "titlebar.h"
#include <QFont>
#include <QDir>
//#include <QMessageBox>
//#include <QDebug>

#ifdef Q_OS_WIN
#pragma comment(lib, "user32.lib")
#include <qt_windows.h>
#endif

TitleBar::TitleBar(QWidget *parent)
    : QWidget(parent)
{
    m_pParent = parent;
    m_pIconLabel = NULL;
    m_pTitleLabel = NULL;
    m_pMinimizeButton = NULL;
    m_pMaximizeButton = NULL;
    m_pCloseButton = NULL;
}

TitleBar::~TitleBar()
{
    if(m_pIconLabel)
    {
        delete m_pIconLabel;
        m_pIconLabel = NULL;
    }
    if(m_pTitleLabel)
    {
        delete m_pTitleLabel;
        m_pTitleLabel = NULL;
    }
    if(m_pMinimizeButton)
    {
        delete m_pMinimizeButton;
        m_pMinimizeButton = NULL;
    }
    if(m_pMaximizeButton)
    {
        delete m_pMaximizeButton;
        m_pMaximizeButton = NULL;
    }
    if(m_pCloseButton)
    {
        delete m_pCloseButton;
        m_pCloseButton = NULL;
    }
}

void TitleBar::init(int iFlag)
{
    m_iHeight = 40;
    m_iIconSize = 20;
    m_iInterval = 5;

    m_pIconLabel = new QLabel(this);
    m_pTitleLabel = new QLabel(this);

    if (iFlag & TITLE_MIN_FLAG)
    {
        m_pMinimizeButton = new QToolButton(this);
        m_pMinimizeButton->setFixedSize(m_iIconSize, m_iIconSize);
        m_pMinimizeButton->setIcon(QIcon(QCoreApplication::applicationDirPath() + QDir::separator() + "img" + QDir::separator() + "min.png"));
        m_pMinimizeButton->setToolTip("最小化");
        m_pMinimizeButton->setAutoRaise(true);
    }
    if (iFlag & TITLE_MAX_FLAG)
    {
        m_pMaximizeButton = new QToolButton(this);
        m_pMaximizeButton->setFixedSize(m_iIconSize, m_iIconSize);
        m_pMaximizeButton->setIcon(QIcon(QCoreApplication::applicationDirPath() + QDir::separator() + "img" + QDir::separator() + "max.png"));
        m_pMaximizeButton->setToolTip("最大化");
        m_pMaximizeButton->setAutoRaise(true);
    }
    m_pCloseButton = new QToolButton(this);

    m_pIconLabel->setFixedSize(m_iIconSize, m_iIconSize);
    m_pIconLabel->setScaledContents(true);

    m_pTitleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pTitleLabel->setText(m_pParent->windowTitle());
    QFont oFont(m_pTitleLabel->font());
    oFont.setPointSize(10);
    m_pTitleLabel->setFont(oFont);

    m_pCloseButton->setFixedSize(m_iIconSize, m_iIconSize);
    if (m_pMinimizeButton)
    {
        connect(m_pMinimizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    }

    if (m_pMaximizeButton)
    {
        connect(m_pMaximizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    }

    m_pCloseButton->setIcon(QIcon(QCoreApplication::applicationDirPath() + QDir::separator() + "img" + QDir::separator() + "close.png"));

    QIcon icon = m_pParent->windowIcon();
    m_pIconLabel->setPixmap(icon.pixmap(m_pIconLabel->size()));

    m_pTitleLabel->setObjectName("whiteLabel");
    m_pCloseButton->setObjectName("closeButton");

    m_pCloseButton->setToolTip("关闭");

    m_pCloseButton->setAutoRaise(true);//设置扁平

    QHBoxLayout *pLayout = new QHBoxLayout(this);
    pLayout->addSpacing(m_iInterval / 2);
    pLayout->addWidget(m_pIconLabel);
    pLayout->addSpacing(m_iInterval / 2);
    pLayout->addWidget(m_pTitleLabel);

    if (m_pMinimizeButton)
    {
        pLayout->addWidget(m_pMinimizeButton);
        pLayout->setSpacing(5);
    }
    if (m_pMaximizeButton)
    {
        pLayout->addWidget(m_pMaximizeButton);
        pLayout->setSpacing(5);
    }

    pLayout->addWidget(m_pCloseButton);
    pLayout->setSpacing(5);
    pLayout->setContentsMargins(m_iInterval, 0, m_iInterval, 0);

    m_pTitleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setFixedHeight(m_iHeight);
    this->setWindowFlags(Qt::FramelessWindowHint);

    connect(m_pCloseButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if (m_pMaximizeButton)
    {
        emit m_pMaximizeButton->clicked();
    }
}

//获取鼠标的按下操作
void TitleBar::mousePressEvent(QMouseEvent *ev)
{
    this->posMouseOrigin = QCursor::pos();//cursor是一个光标类；
}

//获得鼠标移动的操作
void TitleBar::mouseMoveEvent(QMouseEvent *ev)
{
    QPoint ptMouseNow = QCursor::pos();
    QPoint ptDelta = ptMouseNow - this->posMouseOrigin;
    m_pParent->move(m_pParent->pos() + ptDelta);
    posMouseOrigin = ptMouseNow;
}

bool TitleBar::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
        case QEvent::WindowTitleChange:
        {
            QWidget *pWidget = qobject_cast<QWidget *>(obj);
            if (pWidget)
            {
                m_pTitleLabel->setText(pWidget->windowTitle());
                return true;
            }
        }
        case QEvent::WindowIconChange:
        {
            QWidget *pWidget = qobject_cast<QWidget *>(obj);
            if (pWidget)
            {
                QIcon icon = pWidget->windowIcon();
                m_pIconLabel->setPixmap(icon.pixmap(m_pIconLabel->size()));
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

void TitleBar::onClicked()
{
    QToolButton *pButton = qobject_cast<QToolButton *>(sender());
    if(!pButton)
    {
        return;
    }
    QWidget *pWindow = m_pParent;
    if (pWindow->isTopLevel())
    {
        if (pButton == m_pMinimizeButton)
        {
            pWindow->showMinimized();
        }
        else if (pButton == m_pMaximizeButton)
        {
            pWindow->isMaximized() ? pWindow->showNormal() : pWindow->showMaximized();
        }
        if (pButton == m_pCloseButton)
        {
            pWindow->close();
        }
    }
}

void TitleBar::updateMaximize()
{
    if(!m_pMaximizeButton)
    {
        return;
    }
    QWidget *pWindow = m_pParent;
    if (pWindow->isTopLevel())
    {
        bool bMaximize = pWindow->isMaximized();
        if (bMaximize)
        {
            m_pMaximizeButton->setProperty("maximizeProperty", "restore");
            m_pMaximizeButton->setToolTip(tr("最小化"));
        }
        else
        {
            m_pMaximizeButton->setProperty("maximizeProperty", "maximize");
            m_pMaximizeButton->setToolTip(tr("最大化"));
        }
        m_pMaximizeButton->setStyle(QApplication::style());
    }
}
