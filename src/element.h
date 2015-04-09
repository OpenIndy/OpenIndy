#ifndef ELEMENT_H
#define ELEMENT_H

#include <QObject>
#include <QtXml>

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

    //#############
    //get or set id
    //#############

    int getId() const;
    bool setId(const int &id);

    //################
    //save or load XML
    //################

    virtual QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    virtual bool fromOpenIndyXML(QDomElement &xmlElem);

signals:

    //#######################################
    //signals to inform about element changes
    //#######################################

    void elementIdChanged();

protected:
    int id;

    /*
    //TODO make protected and friend of OiFeatureState
public:
    void (&convertMetricValue)(double&);
    void (&convertAngularValue)(double&);
    void (&convertTemperatureValue)(double&);

    unsigned int (&getMetricDigits)();
    unsigned int (&getAngularDigits)();
    unsigned int (&getTemperatureDigits)();*/
};

#endif // ELEMENT_H
