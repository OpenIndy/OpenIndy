#ifndef GLVIEWWIDGET_H
#define GLVIEWWIDGET_H

#include <QWidget>
#include <QWindow>
#include <QScreen>


#include "glwindow.h"


class GlViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GlViewWidget(QWidget *parent = 0);

    QPointer<OiJob> getCurrentJob() const;
    void setCurrentJob(const QPointer<OiJob> &value);

signals:

protected:

public slots:

private:
    GlWindow *view;



};

#endif // GLVIEWWIDGET_H
