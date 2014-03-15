#ifndef ELEMENT_H
#define ELEMENT_H

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

/*!
 * \brief The Element class
 */
class Element
{
public:
    virtual ~Element(){}
    int id;  

    virtual bool toOpenIndyXML(QXmlStreamWriter& stream) = 0;
    virtual bool fromOpenIndyXML(QXmlStreamReader& xml) = 0;
};

#endif // ELEMENT_H
