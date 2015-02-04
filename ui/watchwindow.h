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
#include "oifeaturestate.h"
#include <QCheckBox>
#include "watchwindowlistener.h"
#include "featureupdater.h"
#include <QLineEdit>

namespace Ui {
class WatchWindow;
}

/*!
 * \brief The WatchWindow class handles displaying of differences between active feature and its nominal value.
 */
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
    void addMeasurement();
    void doSelfDefinedAction(QString key);
    void keyPressed(Qt::Key key);

    void sendSettingsReady(bool);
    void sendCheckBoxReady(bool);
    void sendGUIReady(bool);
    void closeWatchWindow();

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

    void stopStream();
    void startStream();

    QMap<QString,QLabel*> streamData;

    QList<QCheckBox*> checkboxes;
    QList<QLabel*> attributeLabels;
    QMap<QString, QLineEdit*> attributeTolerance;
    QList<QLayout*> attributeLayout;

    QList<QLabel*> widgets;
    QList<QLayout*> layouts;

    int activeReadingType;

    bool checkFeatureValid();

    QString getUnitString(QString attribute);

    bool isVisible;

};

#endif // WATCHWINDOW_H
