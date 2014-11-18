#include "oitoolaction.h"

OiToolAction::OiToolAction(QObject *parent) : QAction(parent)
{

}

void OiToolAction::setPluginName(QString pName)
{
    this->pluginName = pName;
}

void OiToolAction::setToolName(QString tName)
{
    this->toolName = tName;
}

QString OiToolAction::getPluginName()
{
    return this->pluginName;
}

QString OiToolAction::getToolName()
{
    return this->toolName;
}
