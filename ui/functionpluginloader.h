#ifndef FUNCTIONPLUGINLOADER_H
#define FUNCTIONPLUGINLOADER_H

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

#include "configuration.h"
#include "featuretreeviewmodel.h"
#include "availableelementstreeviewproxymodel.h"
#include "reading.h"
#include "usedelementsmodel.h"
#include "customparameterwidget.h"
#include "oifeaturestate.h"

namespace Ui {
class FunctionPluginLoader;
}

class FunctionPluginLoader : public QDialog
{
    Q_OBJECT
public:
    explicit FunctionPluginLoader(QWidget *parent = 0);
    ~FunctionPluginLoader();

private slots:
    void on_tableView_functionPlugins_doubleClicked(const QModelIndex &index);
    void on_treeView_functions_clicked(const QModelIndex &index);
    void deleteFunctionContextMenu(QPoint point);

    void on_treeView_availableElements_clicked(const QModelIndex &index);

    void on_cmd_addElement_clicked();

    void closeEvent(QCloseEvent *event);

    void on_tableView_functionPlugins_clicked(const QModelIndex &index);

    void on_cmd_ok_clicked();

    void on_cmd_removeElement_clicked();

    void on_treeView_usedElements_clicked(const QModelIndex &index);

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
    void setActiveFeatureTitle(QString activeFeature);

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
    FeatureTreeItem *selectedAvailableElement;
    FeatureTreeItem *selectedUsedElement;

    QSqlQueryModel *pluginsModel;

    CustomParameterWidget *extraParameterWidget;

};

#endif // FUNCTIONPLUGINLOADER_H
