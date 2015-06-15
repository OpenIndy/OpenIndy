#ifndef ACTUALPROPERTIESDIALOG_H
#define ACTUALPROPERTIESDIALOG_H

#include <QDialog>
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
#include <QKeyEvent>
#include <QClipboard>
#include <QPointer>
#include <QMenu>
#include <QFileDialog>

#include "modelmanager.h"
#include "observationtabledelegate.h"
#include "readingtabledelegate.h"

using namespace oi;

namespace Ui {
class ActualPropertiesDialog;
}

/*!
 * \brief The ActualPropertiesDialog class
 */
class ActualPropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ActualPropertiesDialog(QWidget *parent = 0);
    ~ActualPropertiesDialog();

signals:

    //###################
    //import observations
    //###################

    void importObservations(const QString &filename);

private slots:

    //#########################
    //actions triggered by user
    //#########################

    //import observations
    void on_tableView_observation_customContextMenuRequested(const QPoint &pos);
    void importObservationsMenuClicked(bool checked);

    //resize table views
    void resizeTableView();

private:

    //##################################
    //methods to initialize GUI elements
    //##################################

    void showEvent(QShowEvent *event);

    void initGUI();
    void initModels();

    /*

    ReadingModel *rModel;
    ObservationModel *oModel;
    FunctionStatistic *fModel;
    MatrixModel *qxxModel;
    MatrixModel *sxxModel;

    ObservationProxyModel *observationOverviewModel;
    ReadingProxyModel *readingOverviewModel;

    //void getActiveFeature(FeatureWrapper *activeFeature);

    void showEvent(QShowEvent *event);

public slots:

    void resizeView();

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

    void initGUI();

    void on_comboBox_displayedFunction_currentIndexChanged(const QString &arg1);

    void closeEvent(QCloseEvent *event);

    void displayUsedReadings();

    void on_comboBox_readings_currentTextChanged(const QString &arg1);

    void updateModels();

    void keyPressEvent(QKeyEvent *event);

    void copyValuesFromView();*/

private:
    Ui::ActualPropertiesDialog *ui;
};

#endif // ACTUALPROPERTIESDIALOG_H
