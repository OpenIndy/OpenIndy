#ifndef PI_OITOOL_H
#define PI_OITOOL_H

#include <QWidget>
#include <QDomElement>
#include "pluginmetadata.h"
#include "oijob.h"

class OiTool : public QWidget
{
    Q_OBJECT
public:
    explicit OiTool(QWidget *parent = 0):QWidget(parent){}
    virtual ~OiTool(){}

    virtual PluginMetaData* getMetaData() const = 0;

signals:
    void sendCustomXMLResponse(QDomElement response);

public slots:
    virtual void customXMLRequest(QDomElement request) = 0;


};

#define OiTool_iidd "de.openIndy.Plugin.OiTool.v001"

#endif // PI_OITOOL_H
