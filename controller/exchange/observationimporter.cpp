#include "observationimporter.h"

/*!
 * \brief ObservationImporter::ObservationImporter
 * \param parent
 */
ObservationImporter::ObservationImporter(QObject *parent) : QObject(parent){

}

/*!
 * \brief ObservationImporter::~DataExchanger
 */
ObservationImporter::~ObservationImporter(){

}

/*!
 * \brief ObservationImporter::getCurrentJob
 * \return
 */
const QPointer<OiJob> &ObservationImporter::getCurrentJob() const{
    return this->currentJob;
}

/*!
 * \brief ObservationImporter::setCurrentJob
 * \param job
 */
void ObservationImporter::setCurrentJob(const QPointer<OiJob> &job){
    this->currentJob = job;
}

/*!
 * \brief ObservationImporter::getFileName
 * \return
 */
const QString &ObservationImporter::getFileName() const{
    return this->filename;
}

/*!
 * \brief ObservationImporter::setFileName
 * \param filename
 */
void ObservationImporter::setFileName(const QString &filename){
    this->filename = filename;
}

/*!
 * \brief ObservationImporter::importObservations
 * \return
 */
bool ObservationImporter::importObservations(){

    //check job
    if(this->currentJob.isNull()){
        Console::getInstance()->addLine("No active job");
        emit this->importFinished(false);
        return false;
    }

    //check active station
    if(this->currentJob->getActiveStation().isNull()){
        Console::getInstance()->addLine("No active station");
        emit this->importFinished(false);
        return false;
    }

    //check active feature
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getGeometry().isNull()
            || this->currentJob->getActiveFeature()->getGeometry()->getIsNominal()){
        Console::getInstance()->addLine("You have to select an actual geometry before importing observations");
        emit this->importFinished(false);
        return false;
    }

    //check filename
    if(this->filename.compare("") == 0){
        Console::getInstance()->addLine("No file specified");
        emit this->importFinished(false);
        return false;
    }

    //create and check device
    QPointer<QFile> device = new QFile(this->filename);
    QFileInfo fileInfo(*device.data());
    if(!fileInfo.exists()){
        Console::getInstance()->addLine("The selected file cannot be found in the file system");
        emit this->importFinished(false);
        return false;
    }

    //create list of imported readings
    QList<QPointer<Reading> > readings;

    //open file and create observations
    try{

        //open device
        device->open(QIODevice::ReadOnly | QIODevice::Text);

        //init helper variables
        qint64 fileSize = device->size();
        qint64 readSize = 0;
        qint64 numObservations = 0;

        //read all lines
        QTextStream in(device);
        while (!in.atEnd()){

            //get current line
            QString line = in.readLine();
            readSize += line.size();

            //split the line at whitespaces and check number of columns
            QStringList columns = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
            if(columns.size() != 3){
                continue;
            }

            //check wether the imported values can be parsed to double
            bool parsingSuccess = true;

            //parse the line to xyz coordinates
            double xyz[3];
            for(int i = 0; i < 3; i++){
                xyz[i] = columns.at(i).toDouble(&parsingSuccess);
                if(!parsingSuccess){
                    break;
                }
            }

            //check if there was an error
            if(!parsingSuccess){
                continue;
            }

            //create reading
            ReadingCartesian rCartesian;
            rCartesian.xyz.setAt(0, xyz[0]);
            rCartesian.xyz.setAt(1, xyz[1]);
            rCartesian.xyz.setAt(2, xyz[2]);
            rCartesian.isValid = true;
            QPointer<Reading> reading = new Reading(rCartesian);
            reading->setSensorFace(eFrontSide);
            reading->setMeasuredAt(QDateTime::currentDateTime());

            //add reading to list of imported readings
            readings.append(reading);

            //update import progress
            int progress = (int)(((float)readSize / (float)fileSize) * 100.0);
            if(progress == 100){
                progress = 99;
            }
            numObservations++;
            emit this->updateProgress(progress, QString("%1 observation(s) loaded").arg(numObservations) );
            readSize += 2;

        }

        device->close();
        delete device;

    }catch(const exception &e){
        Console::getInstance()->addLine("Error while opening file.");
        emit this->importFinished(false);
        return false;
    }

    //add observations to the active feature
    this->currentJob->addMeasurementResults(this->currentJob->getActiveFeature()->getGeometry()->getId(), readings);

    emit this->importFinished(true);

    return true;

}
