#ifndef PLUGINTREEITEM_H
#define PLUGINTREEITEM_H

#include <QVariant>
#include <QList>

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
    bool getIsFeatureType();
    bool getIsSensorType();
    void setIsPlugin(bool state);
    void setIsPluginType(bool state);
    void setIsFeatureType(bool state);
    void setIsSensorType(bool state);

    void setParent(PluginTreeItem *parent);

private:
    QVariant displayValue;

    PluginTreeItem *myParent;
    QList<PluginTreeItem*> myChildren;

    bool isPlugin; //true if this represents the name of a plugin
    bool isPluginType; //true if this represents the plugin type (function, sensor)
    bool isFeatureType; //true if this represents the feature type of a function plugin (point, line , plane, ...)
    bool isSensorType; //true if this represents the sensor type of a sensor plugin (lasertracker, tachymeter, ...)
};

#endif // PLUGINTREEITEM_H
