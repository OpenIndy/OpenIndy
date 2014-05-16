#ifndef PLUGINTREEITEM_H
#define PLUGINTREEITEM_H

#include <QVariant>
#include <QList>

#include "configuration.h"

class PluginTreeItem
{
public:
    PluginTreeItem(QVariant val);
    ~PluginTreeItem();

    QVariant getDisplayValue();

    int getChildCount();
    PluginTreeItem* getChild(int row);
    PluginTreeItem* getParent();
    int getIndex();

    void addChild(PluginTreeItem *child);

    void deleteChildren();
    void deleteChild(PluginTreeItem *child);

    bool getIsPlugin();
    bool getIsPluginType();
    bool getIsFunctionType();
    bool getIsSensorType();
    void setIsPlugin(bool state);
    void setIsPluginType(bool state);
    void setIsFunctionType(bool state);
    void setIsSensorType(bool state);

    void setParent(PluginTreeItem *parent);

private:
    QVariant displayValue;

    PluginTreeItem *myParent;
    QList<PluginTreeItem*> myChildren;

    bool isPlugin; //true if this represents the name of a plugin
    bool isPluginType; //true if this represents the plugin type (function, sensor)
    bool isFunctionType; //true if this represents the function type of a function plugin (fit, construct, ...)
    bool isSensorType; //true if this represents the sensor type of a sensor plugin (lasertracker, tachymeter, ...)

    QString pluginName;
    QString pluginAuthor;
    QString pluginDescription;
    QString pluginVersion;
    QString pluginFilePath;

    QString functionName;
    QString functionDescription;
    QList<Configuration::FeatureTypes> applicableFor;
};

#endif // PLUGINTREEITEM_H
