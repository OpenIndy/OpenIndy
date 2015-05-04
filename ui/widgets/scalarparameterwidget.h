#ifndef SCALARPARAMETERWIDGET_H
#define SCALARPARAMETERWIDGET_H

#include <QWidget>
#include <QMap>
#include <QString>
#include <QFormLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QRegExp>
#include <QRegExpValidator>

class ScalarParameterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ScalarParameterWidget(QWidget *parent = 0);
    
signals:

    //#############################################
    //inform aboud changes of the scalar parameters
    //#############################################

    void scalarParametersChanged();
    
public slots:

    //######################
    //set display parameters
    //######################

    //fill scalar display parameters
    void setIntParameter(const QMap<QString, int> &intParameter);
    void setDoubleParameter(const QMap<QString, double> &doubleParameter);
    void setStringParameter(const QMultiMap<QString, QString> &stringParameter, const QMap<QString, QString> &defaultSelection);

    //get scalar parameter selection (user defined inputs)
    const QMap<QString, int> &getIntParameter();
    const QMap<QString, double> &getDoubleParameter();
    const QMap<QString, QString> &getStringParameter();

    //remove all scalar display parameters
    void clearAll();

private:

    //#################
    //helper attributes
    //#################

    //user defined parameters
    QMap<QString, int> intParameter;
    QMap<QString, double> doubleParameter;
    QMap<QString, QStringList> stringParameter;
    QMap<QString, QString> stringParameterSelection;

    //dynamic GUI elements for each available parameter
    QList<QLabel*> parameterLabel;
    QMap<QString, QLineEdit*> intParameterLineEdit;
    QMap<QString, QLineEdit*> doubleParameterLineEdit;
    QMap<QString, QComboBox*> stringParameterComboBox;

    //layout
    QFormLayout *myLayout;

    //regular expressions to validate user input
    QRegExp validInt;
    QRegExp validDouble;
    QRegExpValidator *intValidator;
    QRegExpValidator *doubleValidator;

    //##############
    //helper methods
    //##############

    void setUpGui();
    void clearGuiElements();
    void getParameterFromGUI();
    
};

#endif // SCALARPARAMETERWIDGET_H
