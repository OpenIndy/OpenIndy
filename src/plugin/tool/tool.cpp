#include "tool.h"

/*!
 * \brief Tool::Tool
 * \param parent
 */
Tool::Tool(QWidget *parent) : QWidget(parent){

}

/*!
 * \brief Tool::~Tool
 */
Tool::~Tool(){

}

/*!
 * \brief Tool::getMetaData
 * \return
 */
const PluginMetaData &Tool::getMetaData() const{
    return this->metaData;
}

/*!
 * \brief Tool::getToolType
 * \return
 */
const ToolTypes &Tool::getToolType() const{
    return this->toolType;
}

/*!
 * \brief Tool::getJob
 * \return
 */
const QPointer<OiJob> &Tool::getJob() const{
    return this->job;
}

/*!
 * \brief Tool::setJob
 * \param job
 */
void Tool::setJob(const QPointer<OiJob> &job){
    this->job = job;
}

/*!
 * \brief Tool::closeEvent
 * \param event
 */
void Tool::closeEvent(QCloseEvent *event){
    this->deleteLater();
    event->accept();
}

/*!
 * \brief Tool::customXmlRequest
 * \param request
 */
void Tool::customXmlRequest(const QPointer<OiRequestResponse> &request){
    emit this->sendMessage(QString("No custom XML request implementation"));
}

/*!
 * \brief Tool::watchWindowKeyPressed
 * \param key
 */
void Tool::watchWindowKeyPressed(const Qt::Key &key){
    emit this->sendMessage(QString("No watch window key pressed implementation"));
}

/*!
 * \brief Tool::init
 */
void Tool::init(){

    this->toolType = eUnknownTool;

}
