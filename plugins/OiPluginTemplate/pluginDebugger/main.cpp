
#include "AutoTest.h"
#include <QDebug>
#include <iostream>
#include <QFile>
#include <QTextStream>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
   Q_UNUSED(context);

   QString dt = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
   QString txt = QString("[%1] ").arg(dt);

   switch (type)
   {
      case QtDebugMsg:
         txt += QString("{Debug} \t\t %1").arg(msg);
         break;
      case QtWarningMsg:
         txt += QString("{Warning} \t %1").arg(msg);
         break;
      case QtCriticalMsg:
         txt += QString("{Critical} \t %1").arg(msg);
         break;
      case QtFatalMsg:
         txt += QString("{Fatal} \t\t %1").arg(msg);
         abort();
         break;
   }

   QFile outFile("LogFile.log");
   outFile.open(QIODevice::WriteOnly | QIODevice::Append);

   QTextStream textStream(&outFile);
   textStream << txt << endl;
}


/*!
 * \brief main
 * \param argc
 * \param argv
 * \return
 */
int main(int argc, char *argv[])
{
    QFile log("LogFile.log");
    log.remove();

    //if you want to write all debug output to a logfile
    //qInstallMessageHandler(customMessageHandler);

    int failures = AutoTest::run(argc, argv);
    if (failures == 0)
    {
    qDebug() << "ALL TESTS PASSED";

    }
    else
    {
    qDebug() << failures << " TESTS FAILED!";
    }
    return failures;
}
