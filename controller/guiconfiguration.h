#ifndef GUICONFIGURATION_H
#define GUICONFIGURATION_H

#include <QObject>
#include <QStringList>
#include "unitconverter.h"
#include "displayattribute.h"
#include "configuration.h"

/*!
 * \brief The GUIConfiguration class configures and sets up gui elements.
 * It handles the displayed column attributes and all existing attributes that
 * can be displayed.
 */
class GUIConfiguration : public QObject
{
    Q_OBJECT

public:
    explicit GUIConfiguration(QObject *parent = 0);
    ~GUIConfiguration();

    static void generateLists();

    //! functions and lists for main view and trafoParam
    static QStringList allAttributes;
    static QList<DisplayAttribute*> featureAttributes;
    static QList<DisplayAttribute*> trafoParamAttributes;

    //user defined column order
    //static QStringList userDefFeatOrder;
    //static QStringList userDefTrafoOrder;

    //!functions and lists for observation model
    static QStringList allObsAttributes;
    static QList<DisplayAttribute*> obsAttributes;

    //!returns attributes to display. valid for all functions above
    static QList<int> displayAttributes(QList<DisplayAttribute*> selectedAttributes, QStringList attributes);

    //!special for reading model

    static QString readingType;

    static QStringList allReadAttributes;
    static QList<DisplayAttribute*> polarAttributes;
    static QList<DisplayAttribute*> cartAttributes;
    static QList<DisplayAttribute*> distAttributes;
    static QList<DisplayAttribute*> dirAttributes;
    static QList<DisplayAttribute*> tempAttributes;
    static QList<DisplayAttribute*> levelAttributes;
    static QList<DisplayAttribute*> undefAttributes;

    static QList<int> displayReadingAttributes();

private:

    //! functions and lists for main view and trafoParam
    static void generateAllAttributes();
    static void generateFeatureAttributes();
    static void generateTrafoParamAttributes();

    //!functions and lists for observation model
    static void generateAllObsAttributes();
    static void generateObsAttributes();

    //!special for reading model
    static void generateAllReadAttr();
    static void generatePolarAttr();
    static void generateCartAttr();
    static void generateDistAttr();
    static void generateDirAttr();
    static void generateTempAttr();
    static void generateLevelAttr();
    static void generateUndefAttr();
};

#endif // GUICONFIGURATION_H
