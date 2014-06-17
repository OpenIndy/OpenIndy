#ifndef ELEMENT_H
#define ELEMENT_H

#include <QObject>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "elementdependencies.h"

/*!
 * \brief The Element class
 */
class Element : public QObject
{
    Q_OBJECT
public:
    explicit Element(QObject *parent = 0);
    virtual ~Element();

    int getId() const;
    bool setId(int id);

    virtual bool toOpenIndyXML(QXmlStreamWriter& stream) = 0;
    virtual ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml) = 0;

signals:
    void elementIdChanged();

protected:
    int id;
};

#endif // ELEMENT_H
