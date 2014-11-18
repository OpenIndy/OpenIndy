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

private:
    QString toolName;
    QString pluginName;

};

#endif // OITOOLACTION_H
