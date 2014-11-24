#ifndef OILOADINGDIALOG_H
#define OILOADINGDIALOG_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class OiLoadingDialog;
}

class OiLoadingDialog : public QDialog
{
    Q_OBJECT

private:
    explicit OiLoadingDialog(QWidget *parent = 0);
    ~OiLoadingDialog();

public:
    static OiLoadingDialog *getInstance();

    static const OiLoadingDialog *showLoadingDialog();
    static void closeLoadingDialog();

public slots:
    void updateProgress(int progress, QString msg) const; //progress (0-100)

private:
    Ui::OiLoadingDialog *ui;

    static OiLoadingDialog *myDialog;

signals:

};

#endif // OILOADINGDIALOG_H
