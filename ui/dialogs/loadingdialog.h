#ifndef LOADINGDIALOG_H
#define LOADINGDIALOG_H

#include <QDialog>
#include <QCloseEvent>

namespace Ui {
class LoadingDialog;
}

/*!
 * \brief The LoadingDialog class
 * Display a loading screen when performing time consuming tasks
 */
class LoadingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoadingDialog(QWidget *parent = 0);
    ~LoadingDialog();

public slots:

    //###################################
    //update progress and display message
    //###################################

    void updateProgress(const int &progress, const QString &msg) const; //progress (0-100)

protected:

    //##################################
    //methods to initialize GUI elements
    //##################################

    void closeEvent(QCloseEvent *e);

private:
    Ui::LoadingDialog *ui;

};

#endif // LOADINGDIALOG_H
