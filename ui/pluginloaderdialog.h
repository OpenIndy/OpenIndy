#ifndef PLUGINLOADERDIALOG_H
#define PLUGINLOADERDIALOG_H

#include <QDialog>
#include "pluginloader.h"
#include <QFile>
#include <QFileDialog>
//#include <QMessagebox>
#include <QtWidgets/QMessageBox>

namespace Ui {
class PluginLoaderDialog;
}

class PluginLoaderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PluginLoaderDialog(QWidget *parent = 0);
    ~PluginLoaderDialog();

private slots:

    void on_toolButton_Path_clicked();

    void on_pushButton_Cancel_clicked();

    void on_pushButton_Ok_clicked();

private:
    Ui::PluginLoaderDialog *ui;

    QString pluginPath;
};

#endif // PLUGINLOADERDIALOG_H
