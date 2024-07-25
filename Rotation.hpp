#pragma once

class Rotation
{
    public:
    Rotation()
    {

    }
    Rotation(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    double x;
    double y;
    double z;
};