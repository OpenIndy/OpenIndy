#ifndef PLUGININFOWIDGET_H
#define PLUGININFOWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTextBrowser>
#include <QLabel>
#include <QPixmap>

#include "configuration.h"
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
    void reset();

private:
    QVBoxLayout *myLayout;

    QLabel *label_description;
    QTextBrowser *txt_description;
    QLabel *label_applicableFor;
    QHBoxLayout *layout_applicableFor;
    QLabel *label_neededElements;
    QHBoxLayout *layout_neededElements;

    QList<QLabel*> applicableFor; //labels with icons of features
    QList<QLabel*> neededElements; //labels with icons of elements

    void initGUI();
    void clearLayout(QLayout *layout);
    
};

#endif // PLUGININFOWIDGET_H
