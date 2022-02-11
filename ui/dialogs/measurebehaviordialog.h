#ifndef MEASUREBEHAVIORDIALOG_H
#define MEASUREBEHAVIORDIALOG_H

#include <QDialog>
#include "measurebehaviortypes.h"

namespace Ui {
class MeasureBehaviorDialog;
}


class MeasureBehaviorDialog : public QDialog
{
    Q_OBJECT

public:

    explicit MeasureBehaviorDialog(QWidget *parent = 0);
    ~MeasureBehaviorDialog();

private slots:
    void on_pushButton_searchSMR_clicked();
    void on_pushButton_tryAgain_clicked();
    void on_pushButton_skip_clicked();

signals:
    void setDecision(Decision decision);

private:
    Ui::MeasureBehaviorDialog *ui;

    void clicked(Decision decision);

};


#endif // MEASUREBEHAVIORDIALOG_H
