#ifndef IMPORTNOMINALGEOMETRYDIALOG_H
#define IMPORTNOMINALGEOMETRYDIALOG_H

#include <QDialog>
#include <QStringListModel>
#include <QFileDialog>
#include <QShowEvent>
#include <QDesktopWidget>
#include <QStringListModel>
#include <QMessageBox>

#include "configuration.h"
#include "oidataexchanger.h"
#include "oidataimporter.h"
#include "oiexchangeobject.h"
#include "oifeaturestate.h"
#include "oiloadingdialog.h"

namespace Ui {
class importNominalGeometryDialog;
}

class importNominalGeometryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit importNominalGeometryDialog(QWidget *parent = 0);
    ~importNominalGeometryDialog();

signals:
    void sendFeature(QList<FeatureWrapper*>);

private slots:

    void on_pushButton_import_clicked();

    void on_toolButton_open_clicked();

    void initGUI();

    void on_comboBox_format_currentIndexChanged(const QString &arg1);

    void on_comboBox_typeOfElement_currentIndexChanged(const QString &arg1);

public slots:
    void setUpCoordinateSystems();

    void showEvent(QShowEvent *event);

private slots:
    void updateProgress(int progress, QString msg); //progress (0-100)
    void exchangeFinished(bool success, oiExchangeObject exchangeData);

private:
    Ui::importNominalGeometryDialog *ui;
    oiExchangeObject ExchangeData;

    oiDataExchanger &myExchanger;

    const OiLoadingDialog *myLoadingDialog;

    void setUpSupportedElements(QString format);
    void setUpSupportedUnits(QString format);
    void setUpDescription();
};

#endif // IMPORTNOMINALGEOMETRYDIALOG_H
