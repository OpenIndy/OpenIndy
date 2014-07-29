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
    static unsigned long computeBinomialCoefficient(unsigned int n, unsigned int k);

    /*!
     * Draws numSubsets random subsets from values, each containing subsetLength items
     */
    template<class T> static QMap<int, QList<T> > getRandomSubsets(unsigned int numSubsets, unsigned int subsetLength, QList<T> values){
        QMap<int, QList<T> > result;

        //count of possible subsets (drawing subsetLength out of values)
        unsigned long numAvailableSubsets = PS_GeneralMath::computeBinomialCoefficient(values.size(), subsetLength);

        if(numSubsets <= numAvailableSubsets){ //if drawing numSubsets subsets is possible

            //draw numSubsets subsets
            QList<unsigned long> subsets = PS_GeneralMath::getRandomSubset(numSubsets, numAvailableSubsets-1);

            //dissolve each subset
            unsigned int index = 0;
            foreach(unsigned long subset, subsets){
                result.insert(index, PS_GeneralMath::dissolveSubset(subset, subsetLength, values));
                index++;
            }

        }

        return result;
    }

private:
    /*!
     * Draws one random subset from values containing subsetLength items
     */
    template<class T> static QList<T> getRandomSubset(unsigned int subsetLength, T maxValue){
        QList<T> result;

        T numItems = maxValue + 1;

        if(subsetLength > numItems){ //cannot draw more numbers than there are numbers available
            return result;
        }else if(subsetLength == numItems){ //return all possible numbers if count is exactly numItems
            for(unsigned int i = 0; i <= maxValue; i++){
                result.append(i);
            }
            return result;
        }else{

            std::random_device rd;
            int seed = rd();
            std::default_random_engine generator( seed );

            //TODO switch to draw numItems - count values if count is large

            for(T i = numItems - subsetLength; i < numItems; i++){

                uniform_int_distribution<T> distribution(0, i);
                T item = distribution(generator);

                if(result.contains(item)){
                    result.append( i );
                }else{
                    result.append( item );
                }

            }

            return result;

        }
    }

    /*!
     * From all available subsets of values containing subsetLength items this method returns the one at position
     */
    template<class T> static QList<T> dissolveSubset(unsigned long position, unsigned int subsetLength, QList<T> values){
        QList<T> result;

        //count of possible subsets (drawing subsetLength out of values)
        unsigned long numAvailableSubsets = 0;
        numAvailableSubsets = PS_GeneralMath::computeBinomialCoefficient(values.size(), subsetLength);

        //number of yet viewed subsets
        unsigned long numViewedSubsets = 0;

        unsigned int currentValueIndex = 0;
        do{

            unsigned int n = values.size() - currentValueIndex - 1;
            unsigned int k = subsetLength - result.size() - 1;

            //count of drawing-possibilities (number of still missing items from unviewed values from the current index
            unsigned long solutionsWithIndex = 0;
            if(k > 0){
                solutionsWithIndex = PS_GeneralMath::computeBinomialCoefficient(n, k);
            }else{
                result.append(values.at(currentValueIndex + (position-numViewedSubsets)));
                return result;
            }

            if(solutionsWithIndex == 0){
                result.clear();
                return result;
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

        return result;
    }

};

#endif // PS_GENERALMATH_H
