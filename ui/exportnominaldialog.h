#ifndef EXPORTNOMINALDIALOG_H
#define EXPORTNOMINALDIALOG_H

#include <QDialog>

namespace Ui {
class ExportNominalDialog;
}

class ExportNominalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportNominalDialog(QWidget *parent = 0);
    ~ExportNominalDialog();

private:
    Ui::ExportNominalDialog *ui;
};

#endif // EXPORTNOMINALDIALOG_H
