#ifndef OBSERVATIONDELEGATE_H
#define OBSERVATIONDELEGATE_H

#include <QItemDelegate>
#include <QCheckBox>
#include <QComboBox>

#include "observationproxymodel.h"

/*!
 * \brief The ObservationDelegate class that handles interactive communications with observations.
 * You can set the use state of the observation in the table view and depending on that the
 * observation will be used in the fit function or not.
 */
class ObservationDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ObservationDelegate(QObject *parent = 0);

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

signals:

public slots:

};

#endif // OBSERVATIONDELEGATE_H
