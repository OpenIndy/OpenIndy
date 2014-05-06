#ifndef NOMINALATTRIBUTEEXCHANGE_H
#define NOMINALATTRIBUTEEXCHANGE_H

class NominalAttributeExchange
{
public:
    NominalAttributeExchange();
    NominalAttributeExchange(double nomX, double nomY, double nomZ);
    NominalAttributeExchange(double nomX, double nomY, double nomZ, double nomR);
    NominalAttributeExchange(double nomX, double nomY, double nomZ, double nomI, double nomJ, double nomK);

    double nomX;
    double nomY;
    double nomZ;
    double nomI;
    double nomJ;
    double nomK;
    double nomR;
    double nomSDE;
    double nomSAE;
    double nomSTE;
    double nomSMSE;

};

#endif // NOMINALATTRIBUTEEXCHANGE_H
