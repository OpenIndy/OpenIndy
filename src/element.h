#ifndef ELEMENT_H
#define ELEMENT_H

#include <QObject>
#include <QtXml>

#include "elementdependencies.h"

/*!
 * \brief The Element class
 */
class Element : public QObject
{
    Q_OBJECT
public:
    explicit Element(QObject *parent = 0);
    Element(const Element &copy, QObject *parent = 0);
    Element &operator=(const Element &copy);

    virtual ~Element();

    int getId() const;
    bool setId(int id);

    virtual QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    virtual ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml) = 0;

signals:
    void elementIdChanged();

protected:
    int id;
};

#endif // ELEMENT_H
