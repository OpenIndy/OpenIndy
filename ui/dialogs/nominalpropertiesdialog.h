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

    //########################################################
    //get or set information about the current nominal feature
    //########################################################

    const int &getId() const;
    void setId(const int &id);

    const QString &getName() const;
    void setName(const QString &name);

    const FeatureTypes &getType() const;
    void setType(const FeatureTypes &type);

    void setCurrentNominal(const int &id, const QString &name, const FeatureTypes &type);

    const QMap<UnknownParameters, QString> &getUnknownNominalParameters() const;
    void setUnknownNominalParameters(const QMap<UnknownParameters, QString> &parameters);

signals:

    //#######################
    //edit nominal parameters
    //#######################

    void nominalParametersChanged(const int &id, const QMap<UnknownParameters, double> &parameters);

private slots:

    //#########################
    //actions triggered by user
    //#########################

    void on_toolButton_cancel_clicked();
    void on_toolButton_ok_clicked();

private:

    //##################################
    //methods to initialize GUI elements
    //##################################

    void showEvent(QShowEvent *event);

    void initGUI();
    void initModels();

    //##############
    //helper methods
    //##############

    void setVisibility(const UnknownParameters &param, const bool &visible);

    void setParametersToGUI(const UnknownParameters &param, const QString &value);
    void getParametersFromGUI(QMap<UnknownParameters, double> &parameters);

private:
    Ui::NominalPropertiesDialog *ui;

    //####################################################
    //attributes that describe the current nominal feature
    //####################################################

    int id;
    QString name;
    FeatureTypes type;
    QMap<UnknownParameters, QString> parameters;

};

#endif // NOMINALPROPERTIESDIALOG_H
