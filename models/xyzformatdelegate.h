#ifndef XYZFORMATDELEGATE_H
#define XYZFORMATDELEGATE_H

#include <QStyledItemDelegate>
#include "types.h"
#include "util.h"

using namespace oi;
class XYZFormatDelegate : public QStyledItemDelegate
{
public:
    explicit XYZFormatDelegate(QObject *parent = 0);

    virtual QString displayText(const QVariant &value, const QLocale &locale) const;

private:
    UnitType type;
    int digits;
};

#endif // XYZFORMATDELEGATE_H
