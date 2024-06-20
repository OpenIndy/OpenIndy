#include "bundleoverviewdelegate.h"

BundleOverviewDelegate::BundleOverviewDelegate(QObject * parent)
    : QAbstractItemDelegate(parent)
{
}
void BundleOverviewDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {

    const QString stationName = index.data(Qt::DisplayRole).toString();

    const BundleGeometriesModel *model = static_cast<const BundleGeometriesModel*>(index.model());
    QPointer<Station> station = model->getCurrentJob()->getFeaturesByName(stationName).first()->getStation();

    QString scaleS = "";
    double scale = 0;
    bool isOk = false; // show green or red bullet point
    bool isSolved = false;
    for(QPointer<TrafoParam> tp : model->getCurrentJob()->getTransformationParametersList()) {
        if( isBundleStationTrafo(tp)
            && tp->getDestinationSystem()->getFeatureName() == stationName
            && tp->getStartSystem()->getFeatureName() == "Bundle01" ) {
                scaleS = tp->getDisplayScaleX(6);
                scale = tp->getScale().getAt(0); // X
                isSolved = tp->getIsSolved();
                break;
        }
    }
    isOk = isSolved;


    int numberOfCommonGeometries = 0;
    double maxError = 0; // max v
    double stdDev = 0.0;
    if(isSolved) {
        double sum_vv = 0.0;
        int count_v = 0;

        for(QPointer<Geometry> tg : station->getTargetGeometries()) {
            if(tg->getIsCommon()) {
                numberOfCommonGeometries++;

                OiVec actual = tg->getPosition().getVector();
                for(QPointer<Observation> observation : tg->getObservations()) {
                    if(observation->getStation()->getId() == station->getId()) {
                        count_v++;
                        OiVec xyz = observation->getXYZ();
                        xyz.removeLast();
                        double v = (xyz - actual).length();
                        maxError = v > maxError ? v : maxError;
                        sum_vv += v * v;
                    }
                }
            }
        }
        stdDev = sqrt(sum_vv / (count_v - 1));
    }


/* TODO later
    bool hasTransformationBundleToPart = false;
    const double TEMPERATURE_NOT_SET = -1000.;
    double actualTemperature = TEMPERATURE_NOT_SET;
    double referenceTemperature = TEMPERATURE_NOT_SET;
    QString material;
    for(QPointer<TrafoParam> tp : model->getCurrentJob()->getTransformationParametersList()) {
        if( !isBundleStationTrafo(tp)
            && tp->getDestinationSystem()->getFeatureName() == "PART"
            && tp->getStartSystem()->getFeatureName() == "Bundle01"
            && tp->getIsSolved()
            && tp->getIsUsed()) {
                hasTransformationBundleToPart = true;

                for(QPointer<Function> f : tp->getFunctions()) { // search for alignment with temperature
                    material = f->getStringParameter().value("material");
                    actualTemperature = f->getDoubleParameter().value("actual temperature", TEMPERATURE_NOT_SET);
                    referenceTemperature = f->getDoubleParameter().value("reference temperature", TEMPERATURE_NOT_SET);
                    if(actualTemperature > TEMPERATURE_NOT_SET) {
                        break;
                    }
                }
                break;
        }
    }
*/

    isOk = convertFromDefault(maxError, eUnitMilliMeter) <= this->maxError      // convert from [m] to [mm]
            && convertFromDefault(stdDev, eUnitMilliMeter) <= this->maxStdDev   // convert from [m] to [mm]
            && abs(1. - scale) <= this->maxScaleDev
            && numberOfCommonGeometries >= this->minCommonPoints
            ? isOk : false;

    /*
     *  UI
     */
    painter->setPen(Qt::black);
    painter->drawText(option.rect.topLeft() += QPoint(5, 20),
                      QString("%1")
                      .arg(stationName));
    if(isSolved) {
        painter->drawText(option.rect.topLeft() += QPoint(5, 40),
                          QString("Max. Error:\t\t%1 [mm]")
                          .arg(QString::number(convertFromDefault(maxError, eUnitMilliMeter), 'f', 3)));

        painter->drawText(option.rect.topLeft() += QPoint(5, 60),
                          QString("StdDev:\t\t%1 [mm]")
                          .arg(QString::number(convertFromDefault(stdDev, eUnitMilliMeter), 'f', 3)));

        /* TODO later
        QString temperature;
        if(hasTransformationBundleToPart && actualTemperature > TEMPERATURE_NOT_SET) {
            temperature = QString(" (%2°C %3)").arg(actualTemperature).arg(material);
        }*/
        painter->drawText(option.rect.topLeft() += QPoint(5, 80),
                          QString("Scale:\t\t%1%2")
                          .arg(scaleS).arg("" /* TODO later temperature */));

        painter->drawText(option.rect.topLeft() += QPoint(5, 100),
                          QString("Common Geometries:\t%1")
                          .arg(numberOfCommonGeometries));
    }
    painter->setPen(isOk ? Qt::green : Qt::red);
    painter->drawText(option.rect.topLeft() += QPoint(5, 20), QString("\t\t%1").arg("⬤"));

}
// BundleParameterTableProxyModel::isBundleStationTrafo
bool BundleOverviewDelegate::isBundleStationTrafo(const QPointer<TrafoParam> &param) const
{
    return (param->getStartSystem()->getIsStationSystem() && param->getDestinationSystem()->getIsBundleSystem())
            || (param->getStartSystem()->getIsBundleSystem() && param->getDestinationSystem()->getIsStationSystem());
}
QSize BundleOverviewDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
   return QSize(100, 120);
}

QWidget* BundleOverviewDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const {
   return nullptr;
}

void BundleOverviewDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
}

void BundleOverviewDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
}

void BundleOverviewDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const {
}
