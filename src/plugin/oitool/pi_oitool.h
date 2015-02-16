#ifndef PI_OITOOL_H
#define PI_OITOOL_H

#include <QWidget>
#include <oirequestresponse.h>
#include <QCloseEvent>

#include "pluginmetadata.h"
//#include "oijob.h"

class OiTool : public QWidget
{
    Q_OBJECT
public:
    explicit OiTool(QWidget *parent = 0):QWidget(parent){}//openIndyJob = NULL;}
    virtual ~OiTool(){}

    virtual PluginMetaData* getMetaData() const = 0;

    /*void setOiJob(OiJob *oiJob){
        this->openIndyJob = oiJob;
    }*/

signals:
    void sendCustomXMLResponse(OiRequestResponse *response);

public slots:
    virtual void customXMLRequest(OiRequestResponse *request) = 0;
    virtual void watchWindowKeyPressed(Qt::Key key) = 0;

protected:
    //OiJob *openIndyJob;

public:
    void closeEvent(QCloseEvent *event){
        this->deleteLater();
        event->accept();
    }


};

#define OiTool_iidd "de.openIndy.Plugin.OiTool.v001"

#endif // PI_OITOOL_H
