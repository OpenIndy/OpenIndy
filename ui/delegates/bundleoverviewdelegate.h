#ifndef BUNDLEOVERVIEWDELEGATE_H
#define BUNDLEOVERVIEWDELEGATE_H

#include <QPainter>
#include <QAbstractItemDelegate>
#include <atomic>

#include "oijob.h"
#include "bundlegeometriesmodel.h"
#include "featurewrapper.h"
#include "station.h"
#include "bundleadjustment.h"

using namespace oi;

class BundleOverviewDelegate : public QAbstractItemDelegate {
public:
    BundleOverviewDelegate(QObject * parent);
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    bool isBundleStationTrafo(const QPointer<TrafoParam> &param) const;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;

    void setEditorData(QWidget* editor, const QModelIndex& index) const;

    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;

public:
    atomic<double> maxError;
    atomic<double> maxStdDev;
    atomic<double> maxScaleDev;
    atomic<int> minCommonPoints;
};

#endif // BUNDLEOVERVIEWDELEGATE_H
