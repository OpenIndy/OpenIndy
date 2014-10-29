#ifndef OIPROJECTDATA_H
#define OIPROJECTDATA_H

#include <QObject>
#include <QIODevice>

#include "featurewrapper.h"
#include "console.h"
#include "oifeaturestate.h"
//#include "oiprojectexchanger.h"

/*!
 * \brief The oiProjectData class
 * Used to hold the active OpenIndy project
 */
class OiProjectData : public QObject
{
    Q_OBJECT
private:
    explicit OiProjectData(QObject *parent = 0);

public:
    static OiProjectData *getInstance();

    static const QIODevice *getDevice();
    static QString getProjectName();
    static bool getIsValid();
    static bool getIsSaved();

    static bool setActiveProject(QString name, QIODevice *device = NULL);

private:
    static OiProjectData *activeProject;

    QString projectName; //name of the project
    QIODevice *device; //device where the project is saved
    bool isSaved; //state of the project: true if it was saved; false if there are unsaved changes

signals:
    void unsavedChangesAvailable(); //is emitted when there were changes made to the project
    void projectSaved(); //emitted when the project was saved successfully

public slots:

};

#endif // OIPROJECTDATA_H
