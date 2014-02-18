#ifndef NOMINALDATADIALOG_H
#define NOMINALDATADIALOG_H

#include <QDialog>
#include "featurewrapper.h"
#include "configuration.h"

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
    void sendNominalValues(double nomX, double nomY, double nomZ, double nomI, double nomJ, double nomK, double nomR, double nomSDE, double nomSAE, double nomSTE, double nomSMSE);

public slots:

    void getActiveFeature(FeatureWrapper *aF);

private slots:

    void fillGUI();

    void on_toolButton_ok_clicked();

    void on_toolButton_cancel_clicked();

    void setLabelText();

private:
    FeatureWrapper *selectedFeature;
    Ui::NominalDataDialog *ui;
};

#endif // NOMINALDATADIALOG_H
