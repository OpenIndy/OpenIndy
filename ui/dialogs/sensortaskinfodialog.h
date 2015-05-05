#ifndef SENSORTASKINFODIALOG_H
#define SENSORTASKINFODIALOG_H

#include <QDialog>
#include <QShowEvent>
#include <QDesktopWidget>

namespace Ui {
class SensorTaskInfoDialog;
}

/*!
 * \brief The SensorTaskInfoDialog class
 * Dialog that appears whenever a sensor task is running
 */
class SensorTaskInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SensorTaskInfoDialog(QWidget *parent = 0);

    ~SensorTaskInfoDialog();

    //################
    //set display text
    //################

    void setDisplayMessage(const QString &msg);

private slots:

    //#########################
    //actions triggered by user
    //#########################

    void on_pushButton_cancel_clicked();

private:

    //##################################
    //methods to initialize GUI elements
    //##################################

    void showEvent(QShowEvent *event);

private:
    Ui::SensorTaskInfoDialog *ui;

};

#endif // SENSORTASKINFODIALOG_H
