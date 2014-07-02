#ifndef OIPROJECTDATA_H
#define OIPROJECTDATA_H

#include <QObject>
#include <QIODevice>

#include "featurewrapper.h"
#include "console.h"
#include "oifeaturestate.h"
#include "oiprojectexchanger.h"

/*!
 * \brief The oiProjectData class
 * exchange object for a whole openindy project.
 * Used by oiprojectexchanger for generating a openindyXML
 * or generating a oiProjectData Object form a openindyXML
 */
class OiProjectData : public QObject
{
    Q_OBJECT
public:
    explicit OiProjectData(QObject *parent = 0);
    ~OiProjectData();

    QIODevice *getDevice();
    bool setDevice(QIODevice *device);

    QString getProjectName();
    void setProjectName(QString name);

    bool getIsValid();

    void activate(OiFeatureState *myState);
    void deactivate();

    bool save();
    bool getIsSaved();

    /*CoordinateSystem *activeCoordSystem;
    Station *activeStation;

    QList<FeatureWrapper*> features;
    QList<CoordinateSystem*> coordSystems;
    QList<Station*> stations;*/

private:
    QString projectName; //name of the project
    QIODevice *device; //device where the project is saved
    bool isSaved; //state of the project: true iv it was saved; false if there are unsaved changes

signals:
    void unsavedChangesAvailable(); //is emitted when there were changes made to the project
    void projectSaved(); //emitted when the project was saved successfully

public slots:

};

#endif // OIPROJECTDATA_H
