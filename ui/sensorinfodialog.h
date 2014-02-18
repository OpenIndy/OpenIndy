#ifndef SENSORINFODIALOG_H
#define SENSORINFODIALOG_H

#include <QDialog>

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

private slots:
    void on_pushButton_cancel_clicked();

private:
    Ui::SensorInfoDialog *ui;
};

#endif // SENSORINFODIALOG_H
