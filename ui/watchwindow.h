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
#include "oifeaturestate.h"
#include <QCheckBox>
#include "watchwindowlistener.h"

namespace Ui {
class WatchWindow;
}

class WatchWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit WatchWindow(QWidget *parent = 0);
    ~WatchWindow();

    WatchWindowListener *listener;

    QThread listenerThread;

    double az;

signals:
    void startMeasure();

    void sendSettingsReady(bool);
    void sendCheckBoxReady(bool);
    void sendGUIReady(bool);

private slots:
    void setLCDNumber(QVariantMap);
    void iniGUI();

    //reimplemented
    void keyPressEvent(QKeyEvent * e);
    void closeEvent(QCloseEvent * e);
    void showEvent(QShowEvent * event);

    void initSuppReadings();

    void getAttributes(QStringList l);

    void on_comboBox_suppReadings_currentIndexChanged(const QString &arg1);

private:
    Ui::WatchWindow *ui;
    QVBoxLayout* masterLayout;
    QVBoxLayout* settingsLayout;

    //bool isGUIReady;
    int digitCount;
    //bool isSettingsReady;
    //bool isCheckboxReady;

    QMap<QString,QLCDNumber*> streamData;

    QList<QCheckBox*> checkboxes;

    QList<QWidget*> widgets;
    QList<QLayout*> layouts;

};

#endif // WATCHWINDOW_H
