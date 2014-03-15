#include "geometry.h"

Geometry::~Geometry(){


}

void Geometry::insertReadingType(Configuration::ReadingTypes readingType, QString displayName){

    QMap<Configuration::ReadingTypes,QString>::const_iterator i = usedReadingTypes.find(readingType);

    if (i != usedReadingTypes.end() && i.key() != readingType) {
        usedReadingTypes.insert(readingType,displayName);
    }
}







