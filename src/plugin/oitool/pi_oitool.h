#ifndef PI_OITOOL_H
#define PI_OITOOL_H

#include <QWidget>
#include "pluginmetadata.h"
#include "oijob.h"

class OiTool : public QWidget
{
    Q_OBJECT
public:
    explicit OiTool(QWidget *parent = 0):QWidget(parent){}

    //get meta data
    virtual PluginMetaData* getMetaData() const = 0;

signals:

public slots:


};

#define Sensor_iidd "de.openIndy.Plugin.OiTool.v001"

#endif // PI_OITOOL_H
