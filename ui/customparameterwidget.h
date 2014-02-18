#ifndef CUSTOMPARAMETERWIDGET_H
#define CUSTOMPARAMETERWIDGET_H

#include <QWidget>
#include <QMap>
#include <QString>
#include <QFormLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QRegExp>
#include <QRegExpValidator>
#include "functionconfiguration.h"

class CustomParameterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CustomParameterWidget(QWidget *parent = 0);
    
signals:
    void functionConfigurationChanged();
    
public slots:
    void setIntParameter(QMap<QString, int> intParameter, FunctionConfiguration config);
    void setDoubleParameter(QMap<QString, double> doubleParameter, FunctionConfiguration config);
    void setStringParameter(QMap<QString, QStringList> stringParameter, FunctionConfiguration config);

    FunctionConfiguration getFunctionConfiguration();

    void clearAll();

    void emitFunctionConfigurationChanged();
    void emitFunctionConfigurationChanged(QString);

private:
    QMap<QString, int> intParameter;
    QMap<QString, double> doubleParameter;
    QMap<QString, QStringList> stringParameter;

    QList<QLabel*> parameterLabel;
    QMap<QString, QLineEdit*> intParameterLineEdit;
    QMap<QString, QLineEdit*> doubleParameterLineEdit;
    QMap<QString, QComboBox*> stringParameterComboBox;

    QFormLayout *myLayout;

    QRegExp validInt;
    QRegExp validDouble;
    QRegExpValidator *intValidator;
    QRegExpValidator *doubleValidator;

    void setUpGui();
    void clearGuiElements();
    
};

#endif // CUSTOMPARAMETERWIDGET_H
