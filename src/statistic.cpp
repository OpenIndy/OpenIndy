#include "statistic.h"

/*!
 * \brief Statistic::Statistic
 */
Statistic::Statistic(){

    this->isValid = false;

    this->s0_apriori = 0.0;
    this->s0_aposteriori = 0.0;
    this->stdev = 0.0;

}

/*!
 * \brief Statistic::Statistic
 * \param copy
 */
Statistic::Statistic(const Statistic &copy){

    this->s0_apriori = copy.s0_apriori;
    this->s0_aposteriori = copy.s0_aposteriori;
    this->stdev = copy.stdev;

    //this->displayResiduals = copy.displayResiduals;
    this->isValid = copy.isValid;

    this->p.replace(copy.p);
    this->qxx.replace(copy.qxx);
    this->v.replace(copy.v);

}

/*!
 * \brief Statistic::operator =
 * \param other
 * \return
 */
Statistic& Statistic::operator =(const Statistic &other){

    this->s0_apriori = other.s0_apriori;
    this->s0_aposteriori = other.s0_aposteriori;
    this->stdev = other.stdev;

    //this->displayResiduals = other.displayResiduals;
    this->isValid = other.isValid;

    this->p.replace(other.p);
    this->qxx.replace(other.qxx);
    this->v.replace(other.v);

    return *this;

}

/*!
 * \brief Statistic::getIsValid
 * \return
 */
const bool &Statistic::getIsValid() const{
    return this->isValid;
}

/*!
 * \brief Statistic::setIsValid
 * \param isValid
 */
void Statistic::setIsValid(const bool &isValid){
    this->isValid = isValid;
}

/*!
 * \brief Statistic::reset
 * Clear all statistical parameters and set isValid to false
 */
void Statistic::reset(){

    this->isValid = false;

    this->s0_apriori = 0.0;
    this->s0_aposteriori = 0.0;
    this->stdev = 0.0;

    OiMat resetMat;
    this->p.replace(resetMat);
    this->qxx.replace(resetMat);
    OiVec resetVec;
    this->v.replace(resetVec);

    this->displayResiduals.clear();

}

/*!
 * \brief Statistic::getS0APriori
 * \return
 */
const double &Statistic::getS0APriori() const{
    return this->s0_apriori;
}

/*!
 * \brief Statistic::setS0APriori
 * \param s0
 */
void Statistic::setS0APriori(const double &s0){
    this->s0_apriori = s0;
}

/*!
 * \brief Statistic::getS0APosteriori
 * \return
 */
const double &Statistic::getS0APosteriori() const{
    return this->s0_aposteriori;
}

/*!
 * \brief Statistic::setS0APosteriori
 * \param s0
 */
void Statistic::setS0APosteriori(const double &s0){
    this->s0_aposteriori = s0;
}

/*!
 * \brief Statistic::getStdev
 * \return
 */
const double &Statistic::getStdev() const{
    return this->stdev;
}

/*!
 * \brief Statistic::setStdev
 * \param stdev
 */
void Statistic::setStdev(const double &stdev){
    this->stdev = stdev;
}

/*!
 * \brief Statistic::getP
 * \return
 */
const OiMat &Statistic::getP() const{
    return this->p;
}

/*!
 * \brief Statistic::setP
 * \param p
 */
void Statistic::setP(const OiMat &p){
    if( (this->p.getRowCount() == p.getRowCount() &&this->p.getColCount() == p.getColCount())
            || (this->p.getRowCount() == 0 && this->p.getColCount() == 0) ){
        this->p = p;
    }
}

/*!
 * \brief Statistic::getQxx
 * \return
 */
const OiMat &Statistic::getQxx() const{
    return this->qxx;
}

/*!
 * \brief Statistic::setQxx
 * \param qxx
 */
void Statistic::setQxx(const OiMat &qxx){
    if( (this->qxx.getRowCount() == qxx.getRowCount() &&this->qxx.getColCount() == qxx.getColCount())
            || (this->qxx.getRowCount() == 0 && this->qxx.getColCount() == 0) ){
        this->qxx = qxx;
    }
}

/*!
 * \brief Statistic::getV
 * \return
 */
const OiVec &Statistic::getV() const{
    return this->v;
}

/*!
 * \brief Statistic::setV
 * \param v
 */
void Statistic::setV(const OiVec &v){
    if(this->v.getSize() == v.getSize() || this->v.getSize() == 0){
        this->v = v;
    }
}

/*!
 * \brief Statistic::getDisplayResiduals
 * \return
 */
const QList<Residual> &Statistic::getDisplayResiduals() const{
    return this->displayResiduals;
}

/*!
 * \brief Statistic::addDisplayResidual
 * \param residual
 */
void Statistic::addDisplayResidual(const Residual &residual){
    this->displayResiduals.append(residual);
}


