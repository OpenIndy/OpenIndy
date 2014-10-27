#ifndef OIREQUEST_H
#define OIREQUEST_H

#include <QString>
#include <QIODevice>

class OiRequest
{
public:
    OiRequest();

    enum RequestType{
        eGetProject,
        eGetNominals,
        eMove,
        eMeasure
    };


    RequestType myRequest;

    QIODevice *device;

    //int id
    //double x, y, z, i, j, k, radius;
    //QString name, group, comment;
    //bool fs;
};

#endif // OIREQUEST_H
