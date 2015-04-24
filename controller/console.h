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

class Console : public QObject
{
    Q_OBJECT

private:
    explicit Console(QObject *parent = 0);

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

    void addLine(const QString &msg);
    void addLine(const QString &msg, const bool &value);
    void addLine(const QString &msg, const double &value);
    void addLine(const QString &msg, const int &value);

private:

    //##############
    //helper methods
    //##############

    void writeToLogFile(const QString &msg);

private:

    static QPointer<Console> myInstance;

    //################
    //console contents
    //################

    QStringList log;
    QStringListModel output;

};

#endif // CONSOLE_H
