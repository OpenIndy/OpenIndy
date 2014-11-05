#ifndef PLUGINLOADERDIALOG_H
#define PLUGINLOADERDIALOG_H

#include <QDialog>
#include "pluginloader.h"
#include <QFile>
#include <QFileDialog>
//#include <QMessagebox>
#include <QtWidgets/QMessageBox>
#include <QShowEvent>
#include <QDesktopWidget>

namespace Ui {
class PluginLoaderDialog;
}

class PluginLoaderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PluginLoaderDialog(QWidget *parent = 0);
    ~PluginLoaderDialog();

    void showEvent(QShowEvent *event);

private slots:

    void on_toolButton_Path_clicked();

    void on_pushButton_Cancel_clicked();

    void on_pushButton_Ok_clicked();

    void printMsg(QString msg);
    void printErrorMsg(QString msg);
    void copyFinished(bool check);

private:
    Ui::PluginLoaderDialog *ui;

    QString pluginPath;


};

#endif // PLUGINLOADERDIALOG_H
