#include "xyzformatdelegate.h"

using namespace oi;

XYZFormatDelegate::XYZFormatDelegate(QObject *parent) : QStyledItemDelegate(parent), type(eUnitMilliMeter),digits(2)
{

}

QString XYZFormatDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    // return locale.toString(value.toDouble(), 'f', 2);
    bool ok = false;
    double v = value.toDouble(&ok);
    if(ok) {
        return QString::number(convertFromDefault(v, type), 'f', digits);
    }

    return "";
}
