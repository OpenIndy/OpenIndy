#ifndef CONSOLE_H
#define CONSOLE_H

#include <QObject>
#include <QPointer>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include <QDateTime>
#include <QTextStream>
#include <QFile>

#include "types.h"
#include "util.h"

using namespace oi;

class Console : public QObject
{
    Q_OBJECT

private:
    explicit Console(QObject *parent = 0);
    ~Console();

public:
    static const QPointer<Console> &getInstance();

    //##############################
    //get model with console entries
    //##############################

    QStringListModel &getConsoleModel();

signals:

    //##################################
    //signal to inform about new entries
    //##################################

    void lineAdded();

public slots:

    //#########################
    //write messages to console
    //#########################

    void addLine(const QString &msg, const MessageTypes &msgType);
    void addLine(const QString &msg, const MessageTypes &msgType, const bool &value);
    void addLine(const QString &msg, const MessageTypes &msgType, const double &value);
    void addLine(const QString &msg, const MessageTypes &msgType, const int &value);

private:

    //##############
    //helper methods
    //##############


    void add(const QString &msg, const MessageTypes &msgType, const QString &value = "");
    void writeToLogFile(const QString &msg);

private:

    static QPointer<Console> myInstance;

    //################
    //console contents
    //################

    QStringList log;
    QStringListModel output;
    QFile outFile;

};

#endif // CONSOLE_H
