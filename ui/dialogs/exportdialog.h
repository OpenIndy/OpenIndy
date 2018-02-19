#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>
#include <QPointer>
#include <QDialog>
#include <QStringListModel>
#include <QFileDialog>
#include <QShowEvent>
#include <QDesktopWidget>
#include <QStringListModel>
#include <QMessageBox>

#include "types.h"
#include "util.h"
#include "modelmanager.h"
#include "exchangeParams.h"

using namespace oi;

namespace Ui {
class ExportDialog;
}

/*!
 * \brief The ExportDialog class
 */
class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(QWidget *parent = 0);
    ~ExportDialog();

signals:

    void startExport(const ExchangeParams &params);

private slots:
    void on_pushButton_export_clicked();

    void on_pushButton_Path_clicked();

    void on_comboBox_plugin_currentIndexChanged(const QString &arg1);

private:

    //##################################
    //methods to initialize GUI elements
    //##################################

    void showEvent(QShowEvent *event);

    void initModels();

    Ui::ExportDialog *ui;
};

#endif // EXPORTDIALOG_H
