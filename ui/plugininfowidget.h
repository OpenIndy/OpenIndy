#ifndef PLUGININFOWIDGET_H
#define PLUGININFOWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTextBrowser>
#include <QLabel>

#include "systemdbmanager.h"

class PluginInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PluginInfoWidget(QWidget *parent = 0);
    
signals:
    
public slots:
    void displayFunction(FunctionPlugin myFunction);
    void displaySensor(SensorPlugin mySensor);

private:
    QVBoxLayout *myLayout;

    QLabel *label_description;
    QTextBrowser *txt_description;
    QHBoxLayout *layout_applicableFor;
    QHBoxLayout *layout_neededElements;

    void setUpGui();
    void clearLayout(QLayout *layout);
    
};

#endif // PLUGININFOWIDGET_H
