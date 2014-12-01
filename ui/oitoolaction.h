#ifndef OITOOLACTION_H
#define OITOOLACTION_H

#include <QAction>

class OiToolAction : public QAction
{
    Q_OBJECT

public:
    explicit OiToolAction(QObject *parent = 0);

    void setPluginName(QString pName);
    void setToolName(QString tName);

    QString getPluginName();
    QString getToolName();


signals:
    void openToolWidget(QString pluginName,QString toolName);

private:
    QString toolName;
    QString pluginName;

private slots:
    void clickCallback();

};

#endif // OITOOLACTION_H
