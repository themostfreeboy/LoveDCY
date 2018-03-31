#ifndef XLWIDGET_H
#define XLWIDGET_H

#include <QWidget>

class XlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit XlWidget(QWidget *parent = nullptr);

signals:
    void load_msg();
public slots:
    virtual void load(){};
};

#endif // XLWIDGET_H
