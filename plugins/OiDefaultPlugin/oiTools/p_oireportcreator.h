#ifndef P_OIREPORTCREATOR_H
#define P_OIREPORTCREATOR_H

#include "pi_oitool.h"

#include "oirequestresponse.h"

class OiReportCreator : public OiTool
{ 
public:
    PluginMetaData* getMetaData() const;

public slots:
    void customXMLRequest(OiRequestResponse *request);
    void watchWindowKeyPressed(Qt::Key);

};

#endif // P_OIREPORTCREATOR_H
