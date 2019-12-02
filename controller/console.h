#ifndef CONSOLE_H
#define CONSOLE_H

#include <atomic>
#include <QObject>
#include <QPointer>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include <QDateTime>
#include <QTextStream>
#include <QFile>
#include <QMutex>
#include <QMutexLocker>
#include <QTimer>

#include "types.h"
#include "util.h"

using namespace oi;

class Console : public QObject
{
    Q_OBJECT

private:
    explicit Console(QObject *parent = 0);

public:
    ~Console();

    static const QPointer<Console> &getInstance();

signals:

    void appendMessageToConsole(QString text);

public slots:

    //#########################
    //write messages to console
    //#########################

    void addLine(const QString &msg, const MessageTypes &msgType);
    void addLine(const QString &msg, const MessageTypes &msgType, const bool &value);
    void addLine(const QString &msg, const MessageTypes &msgType, const double &value);
    void addLine(const QString &msg, const MessageTypes &msgType, const int &value);

private slots:
    void flushToConsoleView();

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

    QFile outFile;

    std::atomic<bool> flushToConsoleViewRequested;
    // synchronize add call & buffer access
    QMutex addMessageBufferMutex;
    QStringList messageBuffer;
};

#endif // CONSOLE_H
