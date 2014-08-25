#ifndef PS_GENERALMATH_H
#define PS_GENERALMATH_H

#include <random>
#include <QList>
#include <QMap>

const double PS_PI = 3.14159265358979323846;

using namespace std;

class PS_GeneralMath
{
private:
    PS_GeneralMath();

public:
    static unsigned long long computeBinomialCoefficient(const unsigned int &n, const unsigned int &k);

    /*!
     * Draws numSubsets random subsets from values, each containing subsetLength items
     */
    template<class T> static void getRandomSubsets(QMap<int, QList<T> > &result, const unsigned int &numSubsets, const unsigned int &subsetLength, const QList<T> &values){

        //count of possible subsets (drawing subsetLength out of values)
        unsigned long long numAvailableSubsets = PS_GeneralMath::computeBinomialCoefficient(values.size(), subsetLength);

        //qDebug() << numSubsets << " + " << subsetLength << " + " << values.size() << " + " << numAvailableSubsets;

        if(numSubsets <= numAvailableSubsets){ //if drawing numSubsets subsets is possible

            //draw numSubsets subsets
            QList<unsigned long long> subsets;
            PS_GeneralMath::getRandomSubset(subsets, numSubsets, numAvailableSubsets-1);

            //qDebug() << "r " << subsets.size();
            if(subsets.size() > 0){
                //qDebug() << "val " << subsets.at(0);
            }


            //dissolve each subset
            unsigned int index = 0;
            foreach(const unsigned long long subset, subsets){
                QList<T> item;
                PS_GeneralMath::dissolveSubset(item, subset, subsetLength, values);
                //qDebug() << "itemsize " << item.size();
                result.insert(index, item);
                index++;
            }

        }

    }

private:
    /*!
     * Draws one random subset from values containing subsetLength items
     */
    template<class T> static void getRandomSubset(QList<T> &result, unsigned int subsetLength, const T &maxValue){

        T numItems = maxValue + 1;

        if(subsetLength > numItems){ //cannot draw more numbers than there are numbers available
            return;
        }else if(subsetLength == numItems){ //return all possible numbers if count is exactly numItems
            for(unsigned int i = 0; i <= maxValue; ++i){
                result.append(i);
            }
            return;
        }else{

            std::random_device rd;
            int seed = rd();
            std::default_random_engine generator( seed );

            if( 2 * subsetLength < numItems ){ //if there are equal or less items to draw than there are available

                for(T i = numItems - subsetLength; i < numItems; ++i){

                    uniform_int_distribution<T> distribution(0, i);
                    T item = distribution(generator);

                    if(result.contains(item)){
                        result.append( i );
                    }else{
                        result.append( item );
                    }

                }

            }else{ //otherwise it is easier to draw the items that won't be in the result

                subsetLength = numItems - subsetLength;

                for(unsigned int i = 0; i <= maxValue; ++i){
                    result.append(i);
                }

                for(T i = numItems - subsetLength; i < numItems; ++i){

                    uniform_int_distribution<T> distribution(0, i);
                    T item = distribution(generator);

                    if(!result.contains(item)){
                        result.removeOne( i );
                    }else{
                        result.removeOne( item );
                    }

                }

            }

        }
    }

    /*!
     * From all available subsets of values containing subsetLength items this method returns the one at position
     */
    template<class T> static void dissolveSubset(QList<T> &result, const unsigned long &position, const unsigned int &subsetLength, const QList<T> &values){

        //count of possible subsets (drawing subsetLength out of values)
        unsigned long long numAvailableSubsets = 0;
        numAvailableSubsets = PS_GeneralMath::computeBinomialCoefficient(values.size(), subsetLength);

        //number of yet viewed subsets
        unsigned long long numViewedSubsets = 0;

        //qDebug() << "numAvailableSubsets " << numAvailableSubsets;
        //qDebug() << "position " << position;

        unsigned int currentValueIndex = 0;
        do{

            unsigned int n = values.size() - currentValueIndex - 1;
            unsigned int k = subsetLength - result.size() - 1;

            //qDebug() << "n " << n;
            //qDebug() << "k " << k;

            //count of drawing-possibilities (number of still missing items from unviewed values from the current index
            unsigned long long solutionsWithIndex = 0;
            if(k > 0){
                solutionsWithIndex = PS_GeneralMath::computeBinomialCoefficient(n, k);
            }else{
                result.append(values.at(currentValueIndex + (position-numViewedSubsets)));
                return;
            }

            //qDebug() << "solutionWithindex " << solutionsWithIndex;

            if(solutionsWithIndex == 0){
                /*qDebug() << "solutionWithindex " << solutionsWithIndex;
                qDebug() << "subsetlength " << subsetLength;
                qDebug() << "actual " << result.size();
                qDebug() << "n " << n;
                qDebug() << "k " << k;*/
                result.clear();

                return;
            }

            solutionsWithIndex += numViewedSubsets;

            //if the requested position is within the possible solutions of currentValueIndex then add the value
            //at that index to the result list. Otherwise continue with the next index
            if(solutionsWithIndex > position){

                result.append(values.at(currentValueIndex));
                currentValueIndex++;

            }else{

                numViewedSubsets = solutionsWithIndex;
                currentValueIndex++;

            }


        }while(result.size() < subsetLength);

    }

};

#endif // PS_GENERALMATH_H
