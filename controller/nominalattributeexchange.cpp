#include "nominalattributeexchange.h"

NominalAttributeExchange::NominalAttributeExchange()
{
    this->nomX = 0.0;
    this->nomY = 0.0;
    this->nomZ = 0.0;
    this->nomI = 0.0;
    this->nomJ = 0.0;
    this->nomK = 0.0;
    this->nomR = 0.0;
    this->nomSDE = 0.0;
    this->nomSAE = 0.0;
    this->nomSTE = 0.0;
    this->nomSMSE = 0.0;
}

NominalAttributeExchange::NominalAttributeExchange(double nomX, double nomY, double nomZ)
{
    this->nomX = nomX;
    this->nomY = nomY;
    this->nomZ = nomZ;
    this->nomI = 0.0;
    this->nomJ = 0.0;
    this->nomK = 0.0;
    this->nomR = 0.0;
    this->nomSDE = 0.0;
    this->nomSAE = 0.0;
    this->nomSTE = 0.0;
    this->nomSMSE = 0.0;
}

NominalAttributeExchange::NominalAttributeExchange(double nomX, double nomY, double nomZ, double nomR)
{
    this->nomX = nomX;
    this->nomY = nomY;
    this->nomZ = nomZ;
    this->nomR = nomR;
    this->nomI = 0.0;
    this->nomJ = 0.0;
    this->nomK = 0.0;
    this->nomSDE = 0.0;
    this->nomSAE = 0.0;
    this->nomSTE = 0.0;
    this->nomSMSE = 0.0;
}

NominalAttributeExchange::NominalAttributeExchange(double nomX, double nomY, double nomZ, double nomI, double nomJ, double nomK)
{
    this->nomX = nomX;
    this->nomY = nomY;
    this->nomZ = nomZ;
    this->nomI = nomI;
    this->nomJ = nomJ;
    this->nomZ = nomZ;
    this->nomR = 0.0;
    this->nomSDE = 0.0;
    this->nomSAE = 0.0;
    this->nomSTE = 0.0;
    this->nomSMSE = 0.0;
}
