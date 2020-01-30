#include "watchwindowutil.h"

using namespace oi;


WatchWindowUtil::WatchWindowUtil()
{

}

Result WatchWindowUtil::getPosition(QPointer<FeatureWrapper> feature, OiVec trackerXYZ) {
    Result result;
    result.position = Position::NullObject;
    result.radius = Radius();
    result.delta = OiVec(trackerXYZ.getSize());
    result.d3D = -1.0;

    if(feature.isNull()){
        return result;
    //check if current feature is a solved geometry with position
    }else if(!feature->getGeometry().isNull() && feature->getGeometry()->hasPosition()
             && feature->getGeometry()->getIsSolved()){

        // get radius
        switch(feature->getFeatureTypeEnum()){
        case eCircleFeature:
            result.radius = feature->getCircle()->getRadius();
            break;
        case eCylinderFeature:
            result.radius = feature->getCylinder()->getRadius();
            break;
        default:
            result.radius = Radius();
        }

        // get position
        QPointer<Geometry> geometry = feature->getGeometry();
        switch(feature->getFeatureTypeEnum()){
        case ePlaneFeature: {
            double dot;
            OiVec::dot(dot, trackerXYZ - geometry->getPosition().getVectorH(), geometry->getDirection().getVectorH());
            OiVec point = trackerXYZ - dot * geometry->getDirection().getVectorH();
            result.position = Position();
            result.position.setVector(point);
            break;
        }
        case eCircleFeature:
        case eCylinderFeature:
        case eLineFeature: {
            // projection on axis
            double dot;
            OiVec::dot(dot, trackerXYZ - geometry->getPosition().getVectorH(), geometry->getDirection().getVectorH());
            OiVec point =  geometry->getPosition().getVectorH() + dot * geometry->getDirection().getVectorH();
            result.position = Position();
            result.position.setVector(point);
            break;
        }
        default:
            result.position = geometry->getPosition();
        }

    //check if active feature is a coordinate system
    }else if(!feature->getCoordinateSystem().isNull()){

        result.position = feature->getCoordinateSystem()->getOrigin();

    //check if active feature is a station
    }else if(!feature->getStation().isNull()){

         result.position = feature->getStation()->getPosition()->getPosition();

    }

    if(!result.position.isNull()) {
        result.delta = result.position.getVectorH() - trackerXYZ;
        result.d3D = qSqrt(result.delta.getAt(0)*result.delta.getAt(0)+result.delta.getAt(1)*result.delta.getAt(1)+result.delta.getAt(2)*result.delta.getAt(2)) - result.radius.getRadius();
    }
    return result;

}
