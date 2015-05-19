#ifndef TOOLACTION_H
#define TOOLACTION_H

#include <QAction>

/*!
 * \brief The ToolAction class
 */
class ToolAction : public QAction
{
    Q_OBJECT

public:
    explicit ToolAction(QObject *parent = 0);

    void setPluginName(const QString &pName);
    void setToolName(const QString &tName);

    const QString &getPluginName() const;
    const QString &getToolName() const;

signals:
    void openToolWidget(const QString &pluginName, const QString &toolName);

private:
    QString toolName;
    QString pluginName;

private slots:
    void clickCallback();

};

#endif // TOOLACTION_H
