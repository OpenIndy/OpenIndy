#ifndef MEASUREBEHAVIORDIALOG_H
#define MEASUREBEHAVIORDIALOG_H

#include <QDialog>

namespace Ui {
class MeasureBehaviorDialog;
}

class MeasureBehaviorDialog : public QDialog
{
    Q_OBJECT

public:

    explicit MeasureBehaviorDialog(QWidget *parent = 0);
    ~MeasureBehaviorDialog();

    enum Decision {
        eNotSet,
        eSearchSMR,
        eTryAgain,
        eSkip,
    };

    void resetDecision();

    bool useDecision();
    bool skip();
    bool searchSMR();
    bool tryAgain();

private slots:
    void on_pushButton_searchSMR_clicked();
    void on_pushButton_tryAgain_clicked();
    void on_pushButton_skip_clicked();

private:
    Ui::MeasureBehaviorDialog *ui;

    void clicked(Decision decision);

    Decision decision;
};


#endif // MEASUREBEHAVIORDIALOG_H
