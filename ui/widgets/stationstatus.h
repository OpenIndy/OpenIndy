#ifndef STATIONSTATUS_H
#define STATIONSTATUS_H

#include <QWidget>

namespace Ui {
class StationStatus;
}

class StationStatus : public QWidget
{
    Q_OBJECT

public:
    explicit StationStatus(QWidget *parent = 0);
    ~StationStatus();

    void setName(QString name);
    void setTemperature(QString temperature);
private:
    Ui::StationStatus *ui;
};

#endif // STATIONSTATUS_H
