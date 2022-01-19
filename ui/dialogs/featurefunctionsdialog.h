#ifndef FEATUREFUNCTIONSDIALOG_H
#define FEATUREFUNCTIONSDIALOG_H

#include <QDebug>
#include <QDialog>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlRecord>
#include <QStandardItemModel>
#include <QMenu>
#include <QCloseEvent>
#include <QShowEvent>
#include <QDesktopWidget>
#include <QGridLayout>
#include <QCompleter>
#include <QStringList>

#include "modelmanager.h"
#include "oijob.h"
#include "scalarparameterwidget.h"
#include "multiselectionmodel.h"
#include "functionweightdelegate.h"

using namespace oi;

//#include "customparameterwidget.h"
//#include "oimultiselectionmodel.h"

namespace Ui {
class FeatureFunctionsDialog;
}

/*!
 * \brief The FeatureFunctionsDialog class
 */
class FeatureFunctionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FeatureFunctionsDialog(QWidget *parent = 0);
    ~FeatureFunctionsDialog();

    // set data for element filter
    void setFeatureNameCompleter(QCompleter *completer);
    void setGroupNames(QStringList groupNames);
signals:

    //##############################################################
    //recalculate the active feature after its functions were edited
    //##############################################################

    void recalculateActiveFeature();

    void setFunctionPos(int pos);

private slots:

    //#################################
    //scalar parameter changed callback
    //#################################

    void scalarParameterChanged();

    //#########################
    //actions triggered by user
    //#########################

    //functions table view
    void on_tableView_functionPlugins_clicked(const QModelIndex &index);
    void on_tableView_functionPlugins_doubleClicked(const QModelIndex &index);

    //active feature functions tree view
    void on_treeView_functions_clicked(const QModelIndex &index);

    //avaialable elements tree view
    void on_treeView_availableElements_clicked(const QModelIndex &index);
    void on_treeView_availableElements_entered(const QModelIndex &index);

    //used elements tree view
    void on_treeView_usedElements_clicked(const QModelIndex &index);
    void on_treeView_usedElements_entered(const QModelIndex &index);

    //add or remove elements
    void on_cmd_addElement_clicked();
    void on_cmd_removeElement_clicked();

    //active feature functions tree view context menu
    void activeFeatureFunctionsContextMenuRequested(const QPoint &point);
    void removeSelectedFunction();

    //recalculate the active feature
    void on_cmd_ok_clicked();

    //resize table views
    void resizeTableView();

    //check if the function supports weights
    bool checkSupportsWeights();

private:
    Ui::FeatureFunctionsDialog *ui;

    //#################
    //helper attributes
    //#################

    //widget with scalar input parameters
    ScalarParameterWidget *scalarParameterWidget;


    //##################################
    //methods to initialize GUI elements
    //##################################

    void showEvent(QShowEvent *event);

    void initGUI();
    void initModels();

    void setFunctionParameters(const QModelIndex &index);
    void setFunctionElements(const QModelIndex &index);

};

#endif // FEATUREFUNCTIONSDIALOG_H
