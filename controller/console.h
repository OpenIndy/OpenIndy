#ifndef CONSOLE_H
#define CONSOLE_H

#include <QObject>
#include <QString>
#include <iostream>
#include <QDateTime>
#include <QStringList>
#include <QStringListModel>
#include <QTextStream>
#include <QFile>

class Console : public QObject
{
    Q_OBJECT
public:
    explicit Console(QObject *parent = 0);
    
    static QStringList Log;
    QStringListModel *output;

    static Console *c;

signals:

    void changedList();

public slots:
    
    static void addLine(QString);
    static void addLine(QString, bool);
    static void addLine(QString, double);
    static void addLine(QString, int);

private:

    void writeToLogFile(QString);

};

#endif // CONSOLE_H
