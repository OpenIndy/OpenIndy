#ifndef WATCHWINDOW_H
#define WATCHWINDOW_H

#include <QDialog>
#include <QKeyEvent>
#include "station.h"
#include "sensorlistener.h"
#include "pluginloader.h"
#include "featurewrapper.h"
#include <QMap>
#include <QLabel>
#include <QLCDNumber>
#include <QVBoxLayout>
#include "unitconverter.h"


namespace Ui {
class WatchWindow;
}

class WatchWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit WatchWindow(QWidget *parent = 0);
    ~WatchWindow();

    Station *myStation;
    FeatureWrapper *activeFeature;
    CoordinateSystem *activeCoordinateSystem;

    double az;


signals:
    void startMeasure();

private slots:
    void setLCDNumber(QVariantMap);
    void iniGUI(QVariantMap m);

    //reimplemented
    void keyPressEvent(QKeyEvent * e);
    void closeEvent(QCloseEvent * e);
    void showEvent(QShowEvent * event);


private:
    Ui::WatchWindow *ui;
    QVBoxLayout* masterLayout;

    bool isGUIReady;
    int digitCount;

    QMap<QString,QLCDNumber*> streamData;

};

#endif // WATCHWINDOW_H
