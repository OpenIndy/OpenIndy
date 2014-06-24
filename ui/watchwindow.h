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

namespace Ui {
class WatchWindow;
}

class WatchWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit WatchWindow(QWidget *parent = 0);
    ~WatchWindow();

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

    void initSuppReadings();

    void getAttributes(QVariantMap m);

    void on_comboBox_suppReadings_currentIndexChanged(const QString &arg1);

private:
    Ui::WatchWindow *ui;
    QVBoxLayout* masterLayout;
    QVBoxLayout* settingsLayout;

    bool isGUIReady;
    int digitCount;
    bool isSettingsReady;
    bool isCheckboxReady;

    QMap<QString,QLCDNumber*> streamData;

    QList<QCheckBox*> checkboxes;

};

#endif // WATCHWINDOW_H
