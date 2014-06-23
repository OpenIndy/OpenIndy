#ifndef REALTIMEDATADIALOG_H
#define REALTIMEDATADIALOG_H

#include <QDialog>
#include <QShowEvent>
#include <QCloseEvent>
#include <QStringList>
#include <QStringListModel>
#include <QString>
#include <QDesktopWidget>
#include <QMap>
#include "oifeaturestate.h"
#include "sensorlistener.h"

namespace Ui {
class RealTimeDataDialog;
}

typedef QMap<QString,QString> streamData;

class RealTimeDataDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit RealTimeDataDialog(QWidget *parent = 0);
    ~RealTimeDataDialog();

    QStringList rtDataList;
    QStringListModel * rtDataModel;

    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);

    bool checkSensorValid();
    
private slots:
    void getRTData(QMap<QString,QString> rtData);

private:
    Ui::RealTimeDataDialog *ui;
};

#endif // REALTIMEDATADIALOG_H
