#ifndef NOMINALPROPERTIESDIALOG_H
#define NOMINALPROPERTIESDIALOG_H

#include <QDialog>
#include <QCloseEvent>
#include <QMap>
#include <QLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtNetwork/QNetworkInterface>
#include <QShowEvent>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QClipboard>
#include <QPointer>

#include "modelmanager.h"

using namespace oi;

namespace Ui {
class NominalPropertiesDialog;
}

/*!
 * \brief The NominalPropertiesDialog class
 */
class NominalPropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NominalPropertiesDialog(QWidget *parent = 0);
    ~NominalPropertiesDialog();

private:

    //##################################
    //methods to initialize GUI elements
    //##################################

    void showEvent(QShowEvent *event);

    void initGUI();
    void initModels();

private:
    Ui::NominalPropertiesDialog *ui;
};

#endif // NOMINALPROPERTIESDIALOG_H
