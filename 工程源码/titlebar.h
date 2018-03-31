#ifndef TITLE_BAR
#define TITLE_BAR

#include <QWidget>
#include <QToolButton>
#include <QPoint>

class QLabel;
class QPushButton;

#define TITLE_MIN_FLAG          0x01
#define TITLE_MAX_FLAG          0x02
#define TITLE_DEF_FLAG          0xff

class TitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit TitleBar(QWidget *parent = 0);
    ~TitleBar();

protected:

    //双击标题栏进行界面的最大化/还原
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);

    //设置界面标题与图标
    virtual bool eventFilter(QObject *obj, QEvent *event);

private slots:

    //进行最小化、最大化/还原、关闭操作
    void onClicked();

private:

    //最大化/还原
    void updateMaximize();


protected:
    int m_iHeight;
    int m_iIconSize;
    int m_iInterval;

private:
    QLabel *m_pIconLabel;
    QLabel *m_pTitleLabel;
    QToolButton *m_pMinimizeButton;
    QToolButton *m_pMaximizeButton;
    QToolButton *m_pCloseButton;
    QWidget* m_pParent;
    QPoint posMouseOrigin;

public:
    int get_icon_size()
    {
        return m_iIconSize;
    }

    int get_interval()
    {
        return m_iInterval;
    }

    void init(int iFlag = TITLE_DEF_FLAG);
};

#endif // TITLE_BAR
