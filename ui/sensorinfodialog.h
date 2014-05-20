#ifndef SENSORINFODIALOG_H
#define SENSORINFODIALOG_H

#include <QDialog>
#include <QShowEvent>
#include <QDesktopWidget>

namespace Ui {
class SensorInfoDialog;
}

class SensorInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SensorInfoDialog(QWidget *parent = 0);
    ~SensorInfoDialog();

public slots:
    void showInfo(QString info);
    void hideInfo(bool check);
    void showEvent(QShowEvent *event);

private slots:
    void on_pushButton_cancel_clicked();

private:
    Ui::SensorInfoDialog *ui;
};

#endif // SENSORINFODIALOG_H
