#ifndef OISTAKEOUTMANAGER_H
#define OISTAKEOUTMANAGER_H

#include <QDialog>
#include <QStringListModel>
#include <QtXml>

#include "pointfeaturefiltermodel.h"

namespace Ui {
class OiStakeOutManager;
}

class OiStakeOutManager : public QDialog
{
    Q_OBJECT

public:
    explicit OiStakeOutManager(QWidget *parent = 0);
    ~OiStakeOutManager();

signals:
    void startStakeOut(QDomDocument request);

public slots:
    void setModels(PointFeatureFilterModel *myPoints, QStringListModel *myGroups);

private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_cancel_clicked();

private:
    Ui::OiStakeOutManager *ui;
};

#endif // OISTAKEOUTMANAGER_H
