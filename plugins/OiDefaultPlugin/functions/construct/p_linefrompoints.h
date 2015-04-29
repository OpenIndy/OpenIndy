#ifndef P_LINEFROMPOINTS_H
#define P_LINEFROMPOINTS_H

#include <QObject>
#include <QPointer>
#include <QtMath>

#include "constructfunction.h"
#include "oivec.h"
#include "oimat.h"

using namespace std;

/*!
 * \brief The LineFromPoints class
 */
class LineFromPoints : public ConstructFunction
{
    Q_OBJECT

public:

    //##############################
    //function initialization method
    //##############################

    void init();

protected:

    //############
    //exec methods
    //############

    bool exec(Line &line);

private:

    //##############
    //helper methods
    //##############

    bool setUpResult(Line &line);

};

#endif // P_LINEFROMPOINTS_H
