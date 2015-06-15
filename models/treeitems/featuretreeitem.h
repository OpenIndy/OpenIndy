#ifndef FEATURETREEITEM_H
#define FEATURETREEITEM_H

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
 * Class used to hold single items of the FeatureTreeViewModel
 */
class FeatureTreeItem : public QObject
{
    friend class FeatureTreeItem;
    Q_OBJECT

public:
    explicit FeatureTreeItem(const QVariant &display, QObject *parent = 0);
    explicit FeatureTreeItem(const QPointer<FeatureWrapper> &feature, QObject *parent = 0);
    explicit FeatureTreeItem(const QPointer<Observation> &observation, QObject *parent = 0);
    explicit FeatureTreeItem(const QPointer<Reading> &reading, QObject *parent = 0);
    ~FeatureTreeItem();

    //##########################################
    //get or set information about the tree item
    //##########################################

    //query what kind of tree item this item is
    const bool &getIsHeader() const;
    const bool &getIsFeature() const;
    const bool &getIsObservation() const;
    const bool &getIsReading() const;

    //get pointer to corresponding element
    const QPointer<FeatureWrapper> &getFeature() const;
    const QPointer<Observation> &getObservation() const;
    const QPointer<Reading> &getReading() const;

    //get the type of the corresponding element
    ElementTypes getElementType() const;

    //get information about child items
    bool getHasElement(const ElementTypes &typeOfElement) const;

    //get or set children of this tree item
    int getChildCount() const;
    void appendChild(const QPointer<FeatureTreeItem> &child);
    QPointer<FeatureTreeItem> getChild(const int &index);
    //void removeChild(const int &index);
    void deleteChildren();

    //get the index of this tree item in the children list of its parent
    const int &getIndex() const;

    //get parent of this tree item
    bool hasParent() const;
    const QPointer<FeatureTreeItem> &getParent() const;

    //get the value to display in the tree view
    const QVariant &getDisplayValue() const;

private:

    //#############################################
    //set what kind of tree item this item shall be
    //#############################################

    void setIsHeader();
    void setIsFeature();
    void setIsObservation();
    void setIsReading();

    //#################################
    //set information about child items
    //#################################

    void setHasElement(const bool &hasElement, const ElementTypes &typeOfElement);

    //################################################
    //set up children depending on the type of element
    //################################################

    void setUpFeature();
    void setUpObservation();
    void setUpReading();

    //####################################################
    //attributes that hold information about the tree item
    //####################################################

    //define what kind of tree item this item is
    bool isHeader;
    bool isFeature;
    bool isObservation;
    bool isReading;

    //pointer to the corresponding element
    QPointer<FeatureWrapper> feature;
    QPointer<Observation> observation;
    QPointer<Reading> reading;

    //all elements that are sub elements of this item
    QList<ElementTypes> containedElementTypes;

    //parent and child items
    QPointer<FeatureTreeItem> parent;
    QList< QPointer<FeatureTreeItem> > children;

    //index of this tree item in the children list of its parent
    int index;

    //number of children of this tree item
    int childCount;

    //value that is displayed in tree view for this item
    QVariant displayValue;

};

#endif // FEATURETREEITEM_H
