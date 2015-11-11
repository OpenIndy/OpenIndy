#include "console.h"

//initialize static variables
QPointer<Console> Console::myInstance;

/*!
 * \brief Console::Console
 * \param parent
 */
Console::Console(QObject *parent) : QObject(parent){
    this->output.setStringList(this->log);
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

    //update entries list and model
    this->log.append(QString("[%1] {%2} : %3")
                     .arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss"))
                     .arg(getMessageTypeName(msgType))
                     .arg(msg));
    this->output.setStringList(this->log);

    //inform about the new line
    emit this->lineAdded();

    //write the new entry to the log file
    this->writeToLogFile(this->log.last());

}

/*!
 * \brief Console::addLine
 * \param msg
 * \param msgType
 * \param value
 */
void Console::addLine(const QString &msg, const MessageTypes &msgType, const bool &value){

    //update entries list and model
    this->log.append(QString("[%1] {%2} : %3 %4")
                     .arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss"))
                     .arg(getMessageTypeName(msgType))
                     .arg(msg)
                     .arg(value?"true":"false"));
    this->output.setStringList(this->log);

    //inform about the new line
    emit this->lineAdded();

    //write the new entry to the log file
    this->writeToLogFile(this->log.last());

}

/*!
 * \brief Console::addLine
 * \param msg
 * \param msgType
 * \param value
 */
void Console::addLine(const QString &msg, const MessageTypes &msgType, const double &value){

    //update entries list and model
    this->log.append(QString("[%1] {%2} : %3 %4")
                     .arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss"))
                     .arg(getMessageTypeName(msgType))
                     .arg(msg)
                     .arg(QString::number(value, 'f', 6)));
    this->output.setStringList(this->log);

    //inform about the new line
    emit this->lineAdded();

    //write the new entry to the log file
    this->writeToLogFile(this->log.last());

}

/*!
 * \brief Console::addLine
 * \param msg
 * \param msgType
 * \param value
 */
void Console::addLine(const QString &msg, const MessageTypes &msgType, const int &value){

    //update entries list and model
    this->log.append(QString("[%1] {%2} : %3 %4")
                     .arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss"))
                     .arg(getMessageTypeName(msgType))
                     .arg(msg)
                     .arg(QString::number(value)));
    this->output.setStringList(this->log);

    //inform about the new line
    emit this->lineAdded();

    //write the new entry to the log file
    this->writeToLogFile(this->log.last());

}

/*!
 * \brief Console::writeToLogFile
 * \param msg
 */
void Console::writeToLogFile(const QString &msg){

    //create and open file
    QFile outFile("oiLogFile.log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);

    QTextStream textStream(&outFile);
    textStream << msg << endl;
    outFile.close();

}
