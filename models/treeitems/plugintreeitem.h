#ifndef PLUGINTREEITEM_H
#define PLUGINTREEITEM_H

#include <QVariant>
#include <QList>

#include "types.h"
#include "util.h"
#include "systemdbmanager.h"
#include "oimetadata.h"

using namespace oi;

class PluginTreeItem : public QObject
{
    Q_OBJECT

public:
    explicit PluginTreeItem(const QVariant &display, QObject *parent = 0);
    explicit PluginTreeItem(const sdb::Plugin &plugin, QObject *parent = 0);
    explicit PluginTreeItem(const sdb::Function &function, QObject *parent = 0);
    explicit PluginTreeItem(const sdb::Sensor &sensor, QObject *parent = 0);
    explicit PluginTreeItem(const sdb::NetworkAdjustment &networkAdjustment, QObject *parent = 0);
    explicit PluginTreeItem(const sdb::Tool &tool, QObject *parent = 0);
    explicit PluginTreeItem(const sdb::Simulation &simulation, QObject *parent = 0);
    explicit PluginTreeItem(const sdb::Exchange &exchange, QObject *parent = 0);
    ~PluginTreeItem();

    //##########################################
    //get or set information about the tree item
    //##########################################

    //get the value to display in the tree view
    const QVariant &getDisplayValue() const;

    //get or set children of this tree item
    int getChildCount() const;
    void appendChild(const QPointer<PluginTreeItem> &child);
    QPointer<PluginTreeItem> getChild(const int &index);
    //void removeChild(const int &index);
    void deleteChildren();

    //get parent of this tree item
    bool hasParent() const;
    const QPointer<PluginTreeItem> &getParent() const;

    //get the index of this tree item in the children list of its parent
    const int &getIndex() const;

    //query what kind of tree item this item is
    const bool &getIsHeader() const;
    const bool &getIsPlugin() const;
    const bool &getIsFunction() const;
    const bool &getIsSensor() const;
    const bool &getIsNetworkAdjustment() const;
    const bool &getIsTool() const;
    const bool &getIsSimulation() const;
    const bool &getIsExchange() const;

    //get object behind tree item
    const sdb::Plugin &getPlugin() const;
    const sdb::Function &getFunction() const;
    const sdb::Sensor &getSensor() const;
    const sdb::NetworkAdjustment &getNetworkAdjustment() const;
    const sdb::Tool &getTool() const;
    const sdb::Simulation &getSimulation() const;
    const sdb::Exchange &getExchange() const;

private:

    //#############################################
    //set what kind of tree item this item shall be
    //#############################################

    void setIsHeader();
    void setIsPlugin();
    void setIsFunction();
    void setIsSensor();
    void setIsNetworkAdjustment();
    void setIsTool();
    void setIsSimulation();
    void setIsExchange();

    //###########################################
    //set up children depending on a given plugin
    //###########################################

    void setUpPlugin();

    //####################################################
    //attributes that hold information about the tree item
    //####################################################

    //value that is displayed in tree view for this item
    QVariant displayValue;

    //number of children of this tree item
    int childCount;

    //index of this tree item in the children list of its parent
    int index;

    //parent and child items
    QPointer<PluginTreeItem> parent;
    QList< QPointer<PluginTreeItem> > children;

    //define what kind of tree item this item is
    bool isHeader;
    bool isPlugin;
    bool isFunction;
    bool isSensor;
    bool isNetworkAdjustment;
    bool isTool;
    bool isSimulation;
    bool isExchange;

    //object behind the tree item
    sdb::Plugin plugin;
    sdb::Function function;
    sdb::Sensor sensor;
    sdb::NetworkAdjustment networkAdjustment;
    sdb::Tool tool;
    sdb::Simulation simulation;
    sdb::Exchange exchange;

};

#endif // PLUGINTREEITEM_H
