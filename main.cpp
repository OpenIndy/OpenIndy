#include <QApplication>
#include <QDebug>
#include <QList>
#include <QSplashScreen>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <iostream>

#include "mainwindow.h"
#include "controller.h"
#include "sensor.h"
#include "console.h"

#include "chooselalib.h"
#include "oivec.h"

#include <ctime>
#include <simplepluginloader.h>

const QString _date = __DATE__;
const QString _time = __TIME__;
const QString Company = "sigma3D GmbH & Hochschule Mainz";
const QString AppName = "OpenIndy";
const QString AppUrl  = "www.openIndy.de";
const QString AppAuthor = "M.Sc. Martin Lux, M.Sc. Benedikt Rauls, M.Sc. Jens Wambach";
const QString AppAuthorMail = "info@openindy.de";
const QString AppDate = "2018 (" + _date + "," + _time + ")";

#ifdef QT_NO_DEBUG
const QString AppVersion = QString(OPENINDY_VERSION);
#else
const QString AppVersion = QString("%1 %2").arg(OPENINDY_VERSION).arg(" [RELEASE]");
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 9, 0)
    extern Q_CORE_EXPORT QBasicAtomicInt qt_qhash_seed;
#endif

/*!
 * \brief main
 * \param argc
 * \param argv
 * \return
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    #if QT_VERSION < QT_VERSION_CHECK(5, 9, 0)
        qt_qhash_seed.store(0); // ensures that xml is written the same way
    #else
        qSetGlobalQHashSeed(0); // ensures that xml is written the same way
    #endif

    ChooseLALib::setLinearAlgebra(ChooseLALib::Armadillo);

    a.setOrganizationName(Company);
    a.setOrganizationDomain(Company);
    a.setApplicationName(AppName);
    a.setApplicationVersion(AppVersion);
    a.setApplicationDisplayName(AppName);

    QCommandLineParser parser;
    QCommandLineOption silentOption("s", "silent / no splash screen");
    parser.addOption(silentOption);
    QCommandLineOption importPluginOption("i", "import plugin from direcotry <dir>", "dir");
    parser.addOption(importPluginOption);
    QCommandLineOption versionOption("v", "print version info");
    parser.addOption(versionOption);
    parser.process(a);

    if(parser.isSet(importPluginOption)) {
        SimplePluginLoader loader(parser.value(importPluginOption));
        loader.importPlugin();
        return 0;
    }

    if(parser.isSet(versionOption)) {
        // always first line
        std::cout << OPENINDY_VERSION << std::endl;
        return 0;
    }

    QPointer<QSplashScreen> splash;
    if (!parser.isSet(silentOption))
    {
      QPixmap pixmap(":/Images/icons/OpenIndy_splash.png");

      splash = new QSplashScreen(pixmap);

      splash->show();
      QString msg = argc == 2
            ? QString("loading: %1").arg(argv[1])
            : QString("%1\n%2\n%3").arg(AppUrl).arg(AppAuthorMail).arg(AppAuthor);

      splash->showMessage(msg,Qt::AlignBottom | Qt::AlignHCenter,Qt::white);
    }

    MainWindow w;

    if(argc == 2) {
        w.loadProjectFile(argv[1]);
    }

    w.showMaximized();

    if (!splash.isNull())
    {
      splash->finish(&w);
      delete splash.data();
    }

    return a.exec();
}
