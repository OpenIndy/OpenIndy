#ifndef IMPORTNOMINALGEOMETRYDIALOG_H
#define IMPORTNOMINALGEOMETRYDIALOG_H

#include <QDialog>
#include <QStringListModel>
#include "oidataexchanger.h"
#include "oidataimporter.h"
#include <QFileDialog>
#include "configuration.h"
#include <QShowEvent>
#include <QDesktopWidget>
#include "oifeaturestate.h"

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
    void setUpCoordinateSystems(QList<CoordinateSystem*> coordSystems);

    void showEvent(QShowEvent *event);

private:
    Ui::importNominalGeometryDialog *ui;
    oiExchangeObject ExchangeData;

    void setUpSupportedElements(QString format);
    void setUpSupportedUnits(QString format);
    void setUpDescription();
};

#endif // IMPORTNOMINALGEOMETRYDIALOG_H
