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

#include "modelmanager.h"
#include "oijob.h"
#include "scalarparameterwidget.h"
#include "multiselectionmodel.h"

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







    /*
private slots:
    void on_tableView_functionPlugins_doubleClicked(const QModelIndex &index);
    void on_treeView_functions_clicked(const QModelIndex &index);
    void deleteFunctionContextMenu(QPoint point);

    void on_treeView_availableElements_clicked(const QModelIndex &index);
    void on_treeView_availableElements_entered(const QModelIndex &index);

    void on_treeView_usedElements_clicked(const QModelIndex &index);
    void on_treeView_usedElements_activated(const QModelIndex &index);

    void on_cmd_addElement_clicked();
    void on_cmd_removeElement_clicked();

    void closeEvent(QCloseEvent *event);

    void on_tableView_functionPlugins_clicked(const QModelIndex &index);

    void on_cmd_ok_clicked();

public slots:
    void receivePluginsModel(QSqlQueryModel *sqlModel);
    void receiveFunctionsModel(QStandardItemModel *model);
    void receiveAvailableElementsModel(AvailableElementsTreeViewProxyModel *model);
    void receiveFunctionDescription(QString description);
    void receiveUsedElementsModel(UsedElementsModel *model);
    void receiveExtraParameterForFunction(QMap<QString, int> intParameter, QMap<QString, double> doubleParameter,
                                       QMap<QString, QStringList> stringParameter, FunctionConfiguration config);

    void functionConfigurationChanged();

    void disableFunctionInteractions();
    void deleteFunctionFromFeature(bool);
    //void setActiveFeatureTitle(QString activeFeature);

    void showEvent(QShowEvent *event);

signals:
    void sendPluginID(int ID);
    void createFunction(int index);
    void setSelectedFunction(int functionIndex, int neededElementIndex);
    void deleteFunction(int index);
    void addElement(FeatureTreeItem *element, int functionIndex, int elementIndex);
    void removeElement(FeatureTreeItem *element, int functionIndex, int elementIndex);
    void recalcActiveFeature();
    void sendFunctionConfiguration(int functionIndex, FunctionConfiguration config);

private:
    Ui::FunctionPluginLoader *ui;

    bool openCloseHelper;
    QModelIndex selectedFunctionIndex;
    QModelIndex deleteSelectedFunctionIndex;
    QList<FeatureTreeItem *> selectedAvailableElements;
    FeatureTreeItem *selectedUsedElement;

    QSqlQueryModel *pluginsModel;

    CustomParameterWidget *extraParameterWidget;

    bool validateAvailableElements(QList<FeatureTreeItem*> selectedItems, AvailableElementsTreeViewProxyModel *model);
    bool checkChildren(FeatureTreeItem *item, Configuration::ElementTypes neededType);
    void setUpSelectedElements(QList<FeatureTreeItem*> &result, FeatureTreeItem *item, Configuration::ElementTypes neededType);
*/
};

#endif // FEATUREFUNCTIONSDIALOG_H
