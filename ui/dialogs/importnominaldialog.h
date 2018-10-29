#ifndef IMPORTNOMINALGEOMETRYDIALOG_H
#define IMPORTNOMINALGEOMETRYDIALOG_H

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
#include "projectconfig.h"

using namespace oi;

namespace Ui {
class ImportNominalDialog;
}

/*!
 * \brief The ImportNominalDialog class
 */
class ImportNominalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportNominalDialog(QWidget *parent = 0);
    ~ImportNominalDialog();

signals:

    //############
    //start import
    //############

    void startImport(const ExchangeParams &params);

private slots:

    //#########################
    //actions triggered by user
    //#########################

    void on_comboBox_plugin_sa_currentIndexChanged(const QString &arg1);
    void on_comboBox_exchange_sa_currentIndexChanged(const QString &arg1);
    void on_pushButton_file_sa_clicked();
    void on_pushButton_cancel_sa_clicked();
    void on_pushButton_import_sa_clicked();

private:

    //##################################
    //methods to initialize GUI elements
    //##################################

    void showEvent(QShowEvent *event);

    void initModels();

private:
    Ui::ImportNominalDialog *ui;

};

#endif // IMPORTNOMINALGEOMETRYDIALOG_H
