#include "uiutil.h"

namespace oi{

/*!
 * \brief getDropDownMenuSize
 * \param list
 * \param menuSize
 * \return
 */
const int getDropDownMenuSize(QStringList list, const int menuSize)
{
    //get largest string in list
    QString largestString = "";
    foreach (const QString &filter, list) {
        if(filter.length() > largestString.length()){
            largestString = filter;
        }
    }

    //calculate width of popup dependend of list
    QFont font;
    QFontMetrics fm(font);
    int width = fm.width(largestString);

    //get largest width
    if((width + (0.1 * width)) > menuSize){
        return (width + (0.1 * width));
    }else{
        return menuSize;
    }
    return menuSize;
}

}
