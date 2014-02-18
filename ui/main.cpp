#include <QApplication>
#include <QDebug>
#include <QList>
#include <QSplashScreen>

#include "mainwindow.h"
#include "controller.h"
#include "pluginloader.h"
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
const QString Company = "Fachhochschule Mainz";
const QString AppName = "OpenIndy";
const QString AppUrl  = "www.fh-mainz.de";
const QString AppAuthor = "B.Sc. Martin Lux, B.Sc. Benedikt Rauls, B.Sc. Jens Wambach, Prof. Dr.-Ing. Fredie kern";
const QString AppAuthorMail = ",,,fredie.kern@fh-mainz.de";
const QString AppVersionNoDebug = "0.1";
const QString AppMinorVersion = ".2";
const QString AppDate = "2014 (" + _date + "," + _time + ")";

#ifdef QT_NO_DEBUG
const QString AppVersion = AppVersionNoDebug + AppMinorVersion;
#else
const QString AppVersion = AppVersionNoDebug + AppMinorVersion + " [DEBUG]";
#endif



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

    OiVec c(3);
    OiVec b(3);
    c.setAt(0, 1.0);
    c.setAt(1, 1.1);
    c.setAt(2, 1.2);
    b.setAt(0, 2.0);
    b.setAt(1, 2.1);
    b.setAt(2, 2.2);
    qDebug() << "test";
    OiVec d = c + b;
    qDebug() << "test2";
    Console::addLine("test ", d.getAt(1));

    OiMat tt(3,3);
    tt.setAt(1,1,2.2);

    OiMat neutt = tt * (2.2 * 2.2);
    OiMat neuneu = tt * (1.1);
    QString test = "";

    //Arma lib Test
    //arma::mat A;
    //A.print("A:");

    //PluginLoader p;

    /*FitFunction *ff = p.loadFitFunctionPlugin();
    if(ff != NULL){
        Station *s = new Station();
        CoordinateSystem *cs = new CoordinateSystem();
        for(int i = 0; i < 5; i++){
            Observation *obs = new Observation(NULL, s);
            arma::vec4 v;
            v(0) = 3+i*0.1;
            v(1) = 4+i*0.1;
            v(2) = 5+i*0.1;
            v(3) = 1;
            obs->myXyz = v;
            cs->observations.append(obs);
        }
        s->coordSys = cs;

        ff->addStation(s);
        Point *p = new Point();
        p->functionList.append(ff);
        p->recalc();

        p->xyz.print();
    }*/

    a.setOrganizationName(Company);
    a.setOrganizationDomain(Company);
    a.setApplicationName(AppName);
    a.setApplicationVersion(AppVersion);
    a.setApplicationDisplayName("_"+AppName+"_");


    QSplashScreen *splash;
    if (!isSilent)
    {
      QPixmap pixmap( "splash.png" );  // splash.png muss da liegen, wo *.exe liegt
      QString tmp;

      splash = new QSplashScreen(pixmap);

      splash->show();
      tmp = a.applicationName() + " " + a.applicationVersion() + "\n"
                                      + a.organizationName() + "\n"
                                      + AppUrl + "\n"
                                      + AppAuthor + "\n"
                                      + "main window starts up ...";
      splash->showMessage(tmp,Qt::AlignBottom | Qt::AlignHCenter);

      mySleep(3); // nur so, damit der SplashScreen etwas länger angezeigt wird
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
