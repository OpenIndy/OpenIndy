#ifndef GUICONFIGURATION_H
#define GUICONFIGURATION_H

#include <QObject>
#include <QStringList>
#include "unitconverter.h"
#include "displayattribute.h"

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

    static QStringList allAttributes;
    static QList<DisplayAttribute*> featureAttributes;
    static QList<DisplayAttribute*> trafoParamAttributes;

    static void generateAllAttributes();
    static void generateFeatureAttributes();
    static void generateTrafoParamAttributes();
    static QList<int> displayAttributes(QList<DisplayAttribute*> attributes);
};

#endif // GUICONFIGURATION_H
