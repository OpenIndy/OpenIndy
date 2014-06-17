#ifndef NOMINALDATADIALOG_H
#define NOMINALDATADIALOG_H

#include <QDialog>
#include <QShowEvent>
#include <QDesktopWidget>
#include "featurewrapper.h"
#include "configuration.h"
#include "nominalattributeexchange.h"
#include "oifeaturestate.h"

namespace Ui {
class NominalDataDialog;
}

class NominalDataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NominalDataDialog(QWidget *parent = 0);
    ~NominalDataDialog();

signals:
    void sendNominalValues(NominalAttributeExchange nominalValues);

public slots:

private slots:

    void fillGUI();

    void on_toolButton_ok_clicked();

    void on_toolButton_cancel_clicked();

    void setLabelText();

    void showEvent(QShowEvent *event);

private:
    Ui::NominalDataDialog *ui;
};

#endif // NOMINALDATADIALOG_H
