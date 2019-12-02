#include "console.h"

//initialize static variables
QPointer<Console> Console::myInstance;

/*!
 * \brief Console::Console
 * \param parent
 */
Console::Console(QObject *parent) : QObject(parent){
    this->output.setStringList(this->log);

    //create and open file
    outFile.setFileName("oiLogFile.log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);


    // "compress" lineAdded signals
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(lineAddedIfRequested()));
    timer->start(500);
}

Console::~Console() {
    outFile.close();
}

void Console::lineAddedIfRequested() {
    if(this->lineAddedRequested) {
        QMutexLocker locker(&addMutex);

        this->lineAddedRequested = false;

        foreach(QString text, buffer) {
            if(output.insertRow(output.rowCount())) {
                QModelIndex index = output.index(output.rowCount() - 1, 0);
                output.setData(index, text);
            }
        }
        buffer.clear();

        emit this->lineAdded();

    }
}

/*!
 * \brief Console::getInstance
 * \return
 */
const QPointer<Console> &Console::getInstance(){
    if(Console::myInstance.isNull()){
        Console::myInstance = new Console();
    }

    return Console::myInstance;
}

/*!
 * \brief Console::getConsoleModel
 * \return
 */
QStringListModel &Console::getConsoleModel(){
    return this->output;
}

/*!
 * \brief Console::addLine
 * \param msg
 * \param msgType
 */
void Console::addLine(const QString &msg, const MessageTypes &msgType){
   this->add(msg, msgType);

}
void Console::add(const QString &msg, const MessageTypes &msgType, const QString &value){

    QMutexLocker locker(&addMutex);

    //update entries list and model
    QString text = QString("[%1] {%2} : %3 %4")
            .arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss"))
            .arg(getMessageTypeName(msgType))
            .arg(msg)
            .arg(value);

    this->buffer.append(text);
    //write the new entry to the log file
    this->writeToLogFile(text);

    //inform about the new line
    lineAddedRequested = true;
}

/*!
 * \brief Console::addLine
 * \param msg
 * \param msgType
 * \param value
 */
void Console::addLine(const QString &msg, const MessageTypes &msgType, const bool &value){
    this->add(msg, msgType, value?"true":"false");
}

/*!
 * \brief Console::addLine
 * \param msg
 * \param msgType
 * \param value
 */
void Console::addLine(const QString &msg, const MessageTypes &msgType, const double &value){
    this->add(msg, msgType, QString::number(value, 'f', 6));
}

/*!
 * \brief Console::addLine
 * \param msg
 * \param msgType
 * \param value
 */
void Console::addLine(const QString &msg, const MessageTypes &msgType, const int &value){
    this->add(msg, msgType, QString::number(value));
}

/*!
 * \brief Console::writeToLogFile
 * \param msg
 */
void Console::writeToLogFile(const QString &msg){
    QTextStream textStream(&outFile);
    textStream << msg << endl;
}
