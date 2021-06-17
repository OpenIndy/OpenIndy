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
#include "functionstatisticmodel.h"
#include "clipboardutil.h"

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

    void useObservation(bool use, const QModelIndex &index);
    void unUseObservation(bool use, const QModelIndex &index);
    void removeObservationsById(const QList<int> selectedIds);

private slots:

    //#########################
    //actions triggered by user
    //#########################

    //key pressed
    void keyPressEvent(QKeyEvent * e);

    //import observations
    void on_tableView_observation_customContextMenuRequested(const QPoint &pos);
    void importObservationsMenuClicked(bool checked);

    //resize table views
    void resizeTableView();

    //function statistic
    void on_comboBox_displayedFunction_currentIndexChanged(int index);

    //##############
    //helper methods
    //##############

    //copy values from and to clipboard
    void copyToClipboard();

    void on_tabWidget_selectedFeature_customContextMenuRequested(const QPoint &pos);

    void useObservations();
    void unUseObservations();
    void deleteObservations();

private:

    //##################################
    //methods to initialize GUI elements
    //##################################

    void showEvent(QShowEvent *event);

    void initGUI();
    void initModels();
    QModelIndexList getSelection();

    //#################
    //helper attributes
    //#################

    QPointer<FunctionStatisticModel> functionStatisticModel;

    ClipBoardUtil clipBoardUtil;

private:
    Ui::ActualPropertiesDialog *ui;
};

#endif // ACTUALPROPERTIESDIALOG_H
