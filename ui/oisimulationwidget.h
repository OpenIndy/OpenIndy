#ifndef OISIMULATIONWIDGET_H
#define OISIMULATIONWIDGET_H

#include <QWidget>
#include <QDebug>
#include <featureupdater.h>
#include <oifeaturestate.h>

namespace Ui {
class OiSimulationWidget;
}

class OiSimulationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OiSimulationWidget(QWidget *parent = 0);
    ~OiSimulationWidget();

private slots:
    void on_pushButton_startSimulation_clicked();

private:
    Ui::OiSimulationWidget *ui;
    void recalcAll();
};

#endif // OISIMULATIONWIDGET_H
