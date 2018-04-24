#include <QApplication>
#include <QDebug>
#include <QList>
#include <QSplashScreen>

#include "mainwindow.h"
#include "controller.h"
#include "sensor.h"
#include "console.h"

#include "chooselalib.h"
#include "oivec.h"

#include <ctime>
#include <string.h>

inline void mySleep(clock_t sec) // clock_t is a like typedef unsigned int clock_t. Use clock_t instead of integer in this context
{
  clock_t start_time = clock();
  clock_t end_time = sec * 1000 + start_time;
  while(clock() < end_time);
}

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


/*!
 * \brief main
 * \param argc
 * \param argv
 * \return
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    bool isSilent=false;

    if (argc>=2)
    {
       int i=2;

       while (i<argc && strncmp(argv[i],"-s",2)) i++;
       if (i!=argc) isSilent = true;
    }

    ChooseLALib::setLinearAlgebra(ChooseLALib::Armadillo);

    a.setOrganizationName(Company);
    a.setOrganizationDomain(Company);
    a.setApplicationName(AppName);
    a.setApplicationVersion(AppVersion);
    a.setApplicationDisplayName(AppName);


    QSplashScreen *splash;
    if (!isSilent)
    {
      QPixmap pixmap(":/Images/icons/OpenIndy_splash.png");  // splash.png has to be placed next to the *.exe
      QString tmp;

      splash = new QSplashScreen(pixmap);

      splash->show();
      tmp = AppUrl + "\n"
              + AppAuthorMail + "\n"
              + AppAuthor;

      splash->showMessage(tmp,Qt::AlignBottom | Qt::AlignHCenter,Qt::white);

      mySleep(1.0); // nur so, damit der SplashScreen etwas länger angezeigt wird
    }
    MainWindow w;

    w.showMaximized();

    if (!isSilent)
    {
      splash->finish(&w);
      delete splash;
    }

    return a.exec();
}
