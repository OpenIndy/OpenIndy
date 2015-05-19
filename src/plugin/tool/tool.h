#ifndef TOOL_H
#define TOOL_H

#include <QObject>
#include <QWidget>
#include <QCloseEvent>

#include "types.h"
#include "oirequestresponse.h"
#include "pluginmetadata.h"
#include "oijob.h"

class Tool : public QWidget
{
    Q_OBJECT

public:
    explicit Tool(QWidget *parent = 0);

    virtual ~Tool();

    //##########################
    //tool initialization method
    //##########################

    virtual void init();

    //#########################################
    //methods to get or set further information
    //#########################################

    const PluginMetaData &getMetaData() const;

    const ToolTypes &getToolType() const;

    const QPointer<OiJob> &getJob() const;
    void setJob(const QPointer<OiJob> &job);

    //#########################
    //override QWidget methods
    //#########################

    void closeEvent(QCloseEvent *event);

public slots:

    //#####################################
    //special events a tool could listen on
    //#####################################

    virtual void customXmlRequest(const QPointer<OiRequestResponse> &request);
    virtual void watchWindowKeyPressed(const Qt::Key &key);

signals:

    //############################################
    //signals to inform OpenIndy about tool issues
    //############################################

    void sendMessage(const QString &msg);
    void customXmlResponse(const QPointer<OiRequestResponse> &response);

protected:

    //###########################
    //input and output parameters
    //###########################

    PluginMetaData metaData;

    ToolTypes toolType;

    QPointer<OiJob> job; //the OpenIndy job that this tool works on

};

#define Tool_iidd "de.openIndy.plugin.tool.v001"

#endif // TOOL_H
