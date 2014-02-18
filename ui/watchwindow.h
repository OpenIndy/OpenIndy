#ifndef WATCHWINDOW_H
#define WATCHWINDOW_H

#include <QDialog>
#include "station.h"
#include "pluginloader.h"
#include <QMap>

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
    double az;

private slots:
    void on_pushButton_clicked();
    void setLCDNumber(QVariantMap*);

private:
    Ui::WatchWindow *ui;

};

#endif // WATCHWINDOW_H
