#ifndef OBSERVATIONIMPORTER_H
#define OBSERVATIONIMPORTER_H

#include <QString>
#include <QStringList>
#include <QObject>
#include <QApplication>

#include "oijob.h"
#include "console.h"

/*!
 * \brief The ObservationImporter class
 * Imports observations into the active actual geometry
 */
class ObservationImporter : public QObject
{
    Q_OBJECT

public:
    explicit ObservationImporter(QObject *parent = 0);

    ~ObservationImporter();

    //#############################
    //get or set general attributes
    //#############################

    const QPointer<OiJob> &getCurrentJob() const;
    void setCurrentJob(const QPointer<OiJob> &job);

    const QString &getFileName() const;
    void setFileName(const QString &filename);

public slots:

    //######################
    //start import or export
    //######################

    bool importObservations();

signals:

    //##############################################
    //signals to inform OpenIndy about import issues
    //##############################################

    void sendMessage(const QString &msg);
    void updateProgress(const int &progress, const QString &msg); // 0 <= progress <= 100
    void importFinished(const bool &success);

private:

    //##################
    //general attributes
    //##################

    QPointer<OiJob> currentJob;

    QString filename;

};

#endif // OBSERVATIONIMPORTER_H
