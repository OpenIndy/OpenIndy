#ifndef FUNCTIONTREEITEM_H
#define FUNCTIONTREEITEM_H

#include <QObject>
#include <QPointer>
#include <QList>

#include "types.h"
#include "util.h"
#include "featurewrapper.h"
#include "observation.h"
#include "reading.h"

using namespace oi;

/*!
 * \brief The FeatureTreeItem class
 * Class used to hold single items of the ActiveFeatureFunctionModel
 */
class FunctionTreeItem : public QObject
{
    Q_OBJECT

public:
    explicit FunctionTreeItem(const QVariant &display, QObject *parent = 0);
    ~FunctionTreeItem();

    //##########################################
    //get or set information about the tree item
    //##########################################

    //get or set children of this tree item
    int getChildCount() const;
    void appendChild(const QPointer<FunctionTreeItem> &child);
    QPointer<FunctionTreeItem> getChild(const int &index);
    void deleteChildren();

    //get the index of this tree item in the children list of its parent
    const int &getIndex() const;

    //get parent of this tree item
    bool hasParent() const;
    const QPointer<FunctionTreeItem> &getParent() const;

    //get the value to display in the tree view
    const QVariant &getDisplayValue() const;

private:

    //####################################################
    //attributes that hold information about the tree item
    //####################################################

    //parent and child items
    QPointer<FunctionTreeItem> parent;
    QList< QPointer<FunctionTreeItem> > children;

    //index of this tree item in the children list of its parent
    int index;

    //number of children of this tree item
    int childCount;

    //value that is displayed in tree view for this item
    QVariant displayValue;

};

#endif // FUNCTIONTREEITEM_H
