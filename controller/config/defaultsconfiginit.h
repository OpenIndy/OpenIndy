#ifndef DEFAULTSCONFIGINIT_H
#define DEFAULTSCONFIGINIT_H

#include <QObject>
#include "types.h"

#if defined(OI_MAIN_LIB)
#  define OI_MAIN_EXPORT Q_DECL_EXPORT
#else
#  define OI_MAIN_EXPORT Q_DECL_IMPORT
#endif

using namespace oi;

class OI_MAIN_EXPORT DefaultsConfigInit : public QObject
{
    Q_OBJECT

public:
    explicit DefaultsConfigInit(QObject *parent = 0);

    void init(QString path);

signals:
    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage) const;
};

#endif // DEFAULTSCONFIGINIT_H
