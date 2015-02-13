#ifndef PLUGINTREEITEM_H
#define PLUGINTREEITEM_H

#include <QVariant>
#include <QList>

#include "configuration.h"
#include "systemdbmanager.h"

class PluginTreeItem
{
public:
    PluginTreeItem(QVariant val);
    ~PluginTreeItem();

    QVariant getDisplayValue();

    int getChildCount();
    PluginTreeItem* getChild(int row);
    bool hasParent();
    PluginTreeItem* getParent();
    int getIndex();

    void addChild(PluginTreeItem *child);

    void deleteChildren();
    void deleteChild(PluginTreeItem *child);

    bool getIsPlugin();
    bool getIsFunction();
    bool getIsSensor();
    Plugin getPlugin();
    FunctionPlugin getFunction();
    SensorPlugin getSensor();
    void setPlugin(Plugin myPlugin);
    void setFunction(FunctionPlugin myFunction);
    void setSensor(SensorPlugin mySensor);

    void setParent(PluginTreeItem *parent);

private:
    QVariant displayValue;

    PluginTreeItem *myParent;
    QList<PluginTreeItem*> myChildren;

    bool isPlugin; //true if this represents a plugin
    bool isFunction; //true if this represents a function plugin (fit, construct, ...)
    bool isSensor; //true if this represents a sensor plugin (lasertracker, tachymeter, ...)

    Plugin myPlugin;
    FunctionPlugin myFunction;
    SensorPlugin mySensor;
};

#endif // PLUGINTREEITEM_H
