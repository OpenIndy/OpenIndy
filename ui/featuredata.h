#ifndef FEATUREDATA_H
#define FEATUREDATA_H

#include <QDialog>
#include "featurewrapper.h"
#include "observationmodel.h"
#include "readingmodel.h"
#include "function.h"
#include "functionstatistic.h"
#include "matrixmodel.h"
#include "unitconverter.h"

#include <QDebug>
#include <QCloseEvent>
#include <QMap>
#include <QLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtNetwork/QNetworkInterface>
#include <QShowEvent>
#include <QDesktopWidget>

namespace Ui {
class FeatureData;
}
/*!
 * \brief The FeatureData class is a gui window, that shows information and attributes about the selected feature.
 */
class FeatureData : public QDialog
{
    Q_OBJECT

public:
    explicit FeatureData(QWidget *parent = 0);
    ~FeatureData();

    ReadingModel *rModel;
    ObservationModel *oModel;
    FunctionStatistic *fModel;
    MatrixModel *qxxModel;
    MatrixModel *sxxModel;

    void getActiveFeature(FeatureWrapper *activeFeature);

    void showEvent(QShowEvent *event);

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

    void initGUI();

    void on_comboBox_displayedFunction_currentIndexChanged(const QString &arg1);

    void closeEvent(QCloseEvent *event);

private:

    FeatureWrapper *selectedFeature;
    Ui::FeatureData *ui;
};

#endif // FEATUREDATA_H
