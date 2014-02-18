#include "console.h"

Console *Console::c = NULL;
QStringList Console::Log;


Console::Console(QObject *parent) :
    QObject(parent)
{

    output = new QStringListModel(Console::Log);
}

void Console::writeToLogFile(QString s){
    QFile outFile("oiLogFile.log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);

    QTextStream textStream(&outFile);
    textStream << s<< endl;
    outFile.close();
}


void Console::addLine(QString s){

    QString dt = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
    QString txt = QString("[%1] ").arg(dt);
    txt.append("{openIndyLog} ");
    txt += QString(":  %1").arg(s);


    Console::Log.append(txt);

    if(c!=NULL){
       c->output->setStringList(Console::Log);
       emit c->changedList();
    }

    c->writeToLogFile(txt);
}

void Console::addLine(QString s, bool b){

    QString dt = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
    QString txt = QString("[%1] ").arg(dt);

    QString qs;
    if (b){
        qs = s + " true";
    } else{
        qs = s + " false";
    }

    txt.append("{openIndyLog} ");
    txt += QString(":  %1").arg(qs);

    Console::Log.append(txt);

    if(c!=NULL){
       c->output->setStringList(Console::Log);
       emit c->changedList();
    }

    c->writeToLogFile(txt);
}

void Console::addLine(QString s, double d){

    QString dt = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
    QString txt = QString("[%1] ").arg(dt);

    QString qs;
    qs = s + " " + QString::number(d);

    txt.append("{openIndyLog} ");
    txt += QString(":  %1").arg(qs);

    Console::Log.append(txt);


    if(c!=NULL){
       c->output->setStringList(Console::Log);
       emit c->changedList();
    }

    c->writeToLogFile(txt);
}

void Console::addLine(QString s, int i){

    QString dt = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
    QString txt = QString("[%1] ").arg(dt);

    QString qs;
    qs = s + " " + QString::number(i);

    txt.append("{openIndyLog} ");
    txt += QString(":  %1").arg(qs);

    Console::Log.append(txt);

    if(c!=NULL){
       c->output->setStringList(Console::Log);
       emit c->changedList();
    }

    c->writeToLogFile(txt);
}
