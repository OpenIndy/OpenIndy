#ifndef SHOWLICENSESDIALOG_H
#define SHOWLICENSESDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QDir>
#include <QDebug>
#include <QStandardItem>
#include <QStandardItemModel>


namespace Ui {
class ShowLicensesDialog;
}

class ShowLicensesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShowLicensesDialog(QWidget *parent = 0);
    ~ShowLicensesDialog();

public slots:
    void onClick(const QModelIndex&);

private:
    Ui::ShowLicensesDialog *ui;

    QStandardItemModel model;
};

#endif // SHOWLICENSESDIALOG_H
