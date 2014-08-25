#ifndef PS_LOADINGDIALOG_H
#define PS_LOADINGDIALOG_H

#include <QDialog>

namespace Ui {
class PS_LoadingDialog;
}

class PS_LoadingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PS_LoadingDialog(QWidget *parent = 0);
    ~PS_LoadingDialog();

public slots:
    void reset();
    void setStatus(QString msg, int status);

private:
    Ui::PS_LoadingDialog *ui;
};

#endif // PS_LOADINGDIALOG_H
