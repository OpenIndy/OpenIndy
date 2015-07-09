#include "glviewwidget.h"

GlViewWidget::GlViewWidget(QWidget *parent) : QWidget(parent)
{
    view = new GlWindow();
    QWidget *container = this->createWindowContainer(view);
    QSize screenSize = view->screen()->size();
    container->setMinimumSize(QSize(200, 100));
    container->setMaximumSize(screenSize);

    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->addWidget(container, 1);
    this->setLayout(hLayout);

}

QPointer<OiJob> GlViewWidget::getCurrentJob() const
{
    return view->getCurrentJob();
}

void GlViewWidget::setCurrentJob(const QPointer<OiJob> &value)
{
    view->setCurrentJob(value);
}

