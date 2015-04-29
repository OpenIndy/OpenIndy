/*#include <QObject>

#include "chooselalib.h"
#include "plugin.h"
#include "p_factory.h"

#include "sensor.h"

int main(int argc, char *argv[]){

    ChooseLALib::setLinearAlgebra(ChooseLALib::Armadillo);

    QObject *plugin = new OiTemplatePlugin();

    Plugin *factory = qobject_cast<Plugin *>(plugin);

    QList<QPointer<Sensor> > list = factory->createSensors();

    QPointer<Sensor> sensor = new Sensor();

    Sensor *s2 = new Sensor();

    Sensor *s3 = new PseudoTracker();

    qDebug() << "check";

}
*/
