#pragma once
#include "Rotation.hpp"
#include "Accel.hpp"
#include "Attitude.hpp"
#include "Buttons.hpp"
#include "Joystick.hpp"

class Data
{
    public:
    Data()
    {

    }


    Rotation rotation; //端末の回転の角速度の情報
    Accel accel; //端末の加速度情報(重力を含まない)
    Attitude attitude; //端末の傾き情報
    Buttons button;
    Joystick joystick;
    std::string type; // ここにデータの種類を入れる。 

    
};