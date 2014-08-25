#ifndef PS_POINT_PC_H
#define PS_POINT_PC_H

class PS_Point_PC
{
public:
    PS_Point_PC();

    float xyz[3];
    bool isUsed; //true wenn dieser Punkt für eine endgültig gefundene Form (Ebene, Kugel, Zylinder) benutzt wird
};

#endif // PS_POINT_PC_H
