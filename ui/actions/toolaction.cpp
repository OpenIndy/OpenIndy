#include "toolaction.h"

/*!
 * \brief ToolAction::ToolAction
 * \param parent
 */
ToolAction::ToolAction(QObject *parent) : QAction(parent){
    QObject::connect(this, SIGNAL(triggered()), this, SLOT(clickCallback()));
}

/*!
 * \brief ToolAction::setPluginName
 * \param pName
 */
void ToolAction::setPluginName(const QString &pName){
    this->pluginName = pName;
}

/*!
 * \brief ToolAction::setToolName
 * \param tName
 */
void ToolAction::setToolName(const QString &tName){
    this->toolName = tName;
}

/*!
 * \brief ToolAction::getPluginName
 * \return
 */
const QString &ToolAction::getPluginName() const{
    return this->pluginName;
}

/*!
 * \brief ToolAction::getToolName
 * \return
 */
const QString &ToolAction::getToolName() const{
    return this->toolName;
}

/*!
 * \brief ToolAction::clickCallback
 */
void ToolAction::clickCallback(){
    emit this->openToolWidget(this->pluginName,this->toolName);
}
