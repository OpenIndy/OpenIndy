#ifndef P_OIREPORTCREATOR_H
#define P_OIREPORTCREATOR_H

#include "pi_oitool.h"

class OiReportCreator : public OiTool
{ 
public:
    PluginMetaData* getMetaData() const;

public slots:
    void customXMLRequest(QDomElement request);

};

#endif // P_OIREPORTCREATOR_H
