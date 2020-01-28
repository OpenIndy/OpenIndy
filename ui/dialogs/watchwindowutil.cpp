#include "watchwindowutil.h"

using namespace oi;

WatchWindowUtil::WatchWindowUtil()
{

}

QPair<Position, Radius> WatchWindowUtil::getPosition(QPointer<FeatureWrapper> feature, OiVec trackerXYZ) {
    Position position = Position::NullObject;
    Radius radius;
    if(feature.isNull()){
        return qMakePair(Position::NullObject, Radius());
    //check if current feature is a solved geometry with position
    }else if(!feature->getGeometry().isNull() && feature->getGeometry()->hasPosition()
             && feature->getGeometry()->getIsSolved()){

        // get radius
        switch(feature->getFeatureTypeEnum()){
        case eCircleFeature:
            radius = feature->getCircle()->getRadius();
            break;
        case eCylinderFeature:
            radius = feature->getCylinder()->getRadius();
            break;
        default:
            radius = Radius();
        }

        // get position
        QPointer<Geometry> geometry = feature->getGeometry();
        switch(feature->getFeatureTypeEnum()){
        case eCircleFeature:
        case ePlaneFeature: {
            double dot;
            OiVec::dot(dot, trackerXYZ - geometry->getPosition().getVectorH(), geometry->getDirection().getVectorH());
            OiVec point = trackerXYZ - dot * geometry->getDirection().getVectorH();
            position = Position();
            position.setVector(point);
            break;
        }
        case eCylinderFeature:
        case eSphereFeature:
        case eLineFeature: {
            double dot;
            OiVec::dot(dot, trackerXYZ - geometry->getPosition().getVectorH(), geometry->getDirection().getVectorH());
            OiVec point =  geometry->getPosition().getVectorH() + dot * geometry->getDirection().getVectorH();
            position = Position();
            position.setVector(point);
            break;
        }
        default:
            position = geometry->getPosition();
        }

    //check if active feature is a coordinate system
    }else if(!feature->getCoordinateSystem().isNull()){

        position = feature->getCoordinateSystem()->getOrigin();

    //check if active feature is a station
    }else if(!feature->getStation().isNull()){

         position = feature->getStation()->getPosition()->getPosition();

    }

    return qMakePair(position, radius);

}
