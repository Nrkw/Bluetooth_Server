#include "SocketManager.hpp"
#include "Data.hpp"
#include "Event.hpp"
#include <string>
#include <iostream>
#include <chrono>
#include <mutex>

bool isConnected = false;
class Position
{
    public:
    Position(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Position()
    {
        x = 0;
        y = 0;
        z = 0;
    }
    double x;
    double y;
    double z;

};

Position pos(0, 0, 0);
std::mutex mtx;
void callback(Data data)
{
    if(data.type == "connected")
    {
        isConnected = true;
        std::cout << "Connected!" << std::endl;
    }
    if(data.type == "grad")
    {
        pos.x += data.rotation.x;
        pos.y += data.rotation.y;
        pos.z += data.rotation.z;
        //std::cout << "ROTATE!!" << std::endl;
    }else if(data.type == "accel")
    {
        //std::cout << "ACCEL!" << std::endl;
    }
}
int main()
{
    SocketManager manager;
    manager.setLogLevel(Debug::Level::INFO);
    manager.start(callback);
    while(1)
    {
        for(int i = 0; i < 10; i++)
        {
            if(isConnected)
            {
                manager.sendEvent(Event::DEVICE_VIBRATE, "1000");
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            }
        }
    }
    manager.end();
    return 0;
}
