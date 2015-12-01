#include "oiwebsocketstreamer.h"

using namespace oi;

/*!
 * \brief OiWebSocketStreamer::OiWebSocketStreamer
 * \param parent
 */
OiWebSocketStreamer::OiWebSocketStreamer(QObject *parent){

}

/*!
 * \brief OiWebSocketStreamer::getJob
 * \return
 */
const QPointer<OiJob> &OiWebSocketStreamer::getJob() const{
    return this->currentJob;
}

/*!
 * \brief OiWebSocketStreamer::setJob
 * \param job
 */
void OiWebSocketStreamer::setJob(const QPointer<OiJob> &job){
    if(!job.isNull()){
        this->currentJob = job;
    }
}
