#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <future>

#include "SocketManager.hpp"
#include "socket.hpp"
#include "util.hpp"
#include "Rotation.hpp"
#include "Attitude.hpp"

SocketManager::SocketManager()
{
    future = promise.get_future();
}
SocketManager::~SocketManager()
{

}
void SocketManager::start(std::function<void(Data)> callback)
{
    callbackFunc = callback;
    std::thread th([this](){startServer();});
    th.detach();
}
void SocketManager::setEventFunction(std::function<void(Data)> callback)
{
    callbackFunc = callback;
}

void SocketManager::startServer()
{
    
    try
    {
        log_mgr.log(Debug::Level::INFO, "サーバーを起動します");
        socket.startListen();
        Data data;
        data.type = "connected";
        callbackFunc(data);
        log_mgr.log(Debug::Level::INFO, "端末と接続しました");

        socket.setLongestReceiveSize(128);
        int i = 0;
        
        while(future.wait_for(std::chrono::seconds(0)) != std::future_status::ready)
        {
           std::string info = socket.receiveMessage();
           //TODO: ここで、info = "grad,x,0.0000,y,0.0000,z,0.000"
           //のような文字列を受け取るので、","で分割してDataクラスに格納してcallbackFuncに渡す  
            std::vector <std::string> received;

            std::string info_separated = util::separate_at_semicolon(info);
            received = util::split_naive(info_separated, ',');
            if(received[0] == "grad")
            {
                //Rotationのデータを受け取った時。
                try
                {
                    double x = std::stod(received[2].substr(0, 3));
                    double y = std::stod(received[4].substr(0, 3));
                    double z = std::stod(received[6].substr(0, 3));
                    Data return_Data;
                    return_Data.type = received[0];
                    return_Data.rotation = Rotation(x, y, z);
                    //以上に、受け取ったデータをDataクラスに格納する。
                    callbackFunc(return_Data);
                }catch(const std::out_of_range &e)
                {
                    //Do nothing.
                }
            }else if(received[0] == "accel")
            {
                try
                {
                    double x = std::stod(received[2].substr(0, 3));
                    double y = std::stod(received[4].substr(0, 3));
                    double z = std::stod(received[6].substr(0, 3));
                    Data return_Data;
                    return_Data.type = received[0];
                    return_Data.accel.x = x;
                    return_Data.accel.y = y;
                    return_Data.accel.z = z; 
                    //以上に、受け取ったデータをDataクラスに格納する。
                    callbackFunc(return_Data);
                }
                catch(const std::exception& e)
                {
                    log_mgr.log(Debug::Level::ERR, e.what());
                }
                
            }else if(received[0] == "attitude")
            {
                try
                {
                    double x = std::stod(received[2].substr(0, 5));
                    double y = std::stod(received[4].substr(0, 5));
                    double z = std::stod(received[6].substr(0, 5));
                    Data return_Data;
                    return_Data.type = received[0];
                    return_Data.attitude.x = x;
                    return_Data.attitude.y = y;
                    return_Data.attitude.z = z; 
                    //以上に、受け取ったデータをDataクラスに格納する。
                    callbackFunc(return_Data);
                }
                catch(const std::exception& e)
                {
                    log_mgr.log(Debug::Level::ERR, e.what());
                }
            }else if(received[0] == "button_press")
            {
                Data return_data;
                return_data.type = "button_press";
                if(util::isIncludeChar(received[0], 'a'))
                {
                    return_data.button = Buttons::BUTTON_A;
                }else if(util::isIncludeChar(received[0], 'b'))
                {
                    return_data.button = Buttons::BUTTON_B;
                }else if(util::isIncludeChar(received[0], 'x'))
                {
                    return_data.button = Buttons::BUTTON_X;
                }else if(util::isIncludeChar(received[0], 'y'))
                {
                    return_data.button = Buttons::BUTTON_Y;
                }else if(util::isIncludeStr(received[0], "center"))
                {
                    return_data.button = Buttons::BUTTON_CENTER;
                }
                callbackFunc(return_data);
                // std::cout << "ボタン" << received[1] << "がおされた！" << std::endl;
            }else if(received[0] == "button_release")
            {
                Data return_data;
                return_data.type = "button_release";
                if(util::isIncludeChar(received[0], 'a'))
                {
                    return_data.button = Buttons::BUTTON_A;
                }else if(util::isIncludeChar(received[0], 'b'))
                {
                    return_data.button = Buttons::BUTTON_B;
                }else if(util::isIncludeChar(received[0], 'x'))
                {
                    return_data.button = Buttons::BUTTON_X;
                }else if(util::isIncludeChar(received[0], 'y'))
                {
                    return_data.button = Buttons::BUTTON_Y;
                }else if(util::isIncludeStr(received[0], "center"))
                {
                    return_data.button = Buttons::BUTTON_CENTER;
                }else
                {
                    return_data.button = Buttons::BUTTON_UNKNOWN;
                }

                callbackFunc(return_data);
                // std::cout << "ボタン" << received[1] << "が離された！" << std::endl;
            }else if(received[0] == "joystick_move")
            {
                // std::cout << "ジョイスティックが動かされた。 x = " << received[2] << ", y = " << received[4] << std::endl;
                Data return_data;

                //0.145のように、数字をstd::stringにしたときの最初の5文字(0~4文字目)をdoubleに変換
                return_data.type = "joystick_move";
                return_data.joystick.x = std::stod(received[2].substr(0, 4));
                return_data.joystick.y = std::stod(received[4].substr(0, 4));
                callbackFunc(return_data);
            }



           //std::cout << "info: " << info << std::endl;
           i++;
        }
        log_mgr.log(Debug::Level::INFO, "サーバーを終了します");
    }catch(const std::exception &e)
    {
        log_mgr.log(Debug::Level::ERR, std::string("エラー発生") + e.what());
    }
}

void SocketManager::sendEvent(Event event, std::string data_str)
{
    std::string send_str = "";

    //イベントに応じて送信する文字列を変更
    switch(event)
    {
        case Event::DEVICE_VIBRATE:
            send_str = "vibrate";
            if(data_str.size() > 0 && util::canConvertToint(data_str))
            {
                send_str += "," + data_str;
            }
        break;
    }
    if(send_str == "")
    {
        log_mgr.log(Debug::Level::ERR, "in SocketManager::sendEvent(): 与えられたイベントの送信は現在非対応です。");
        return;
    }
    
    //送信処理を別スレッドで行う(ブロッキングを避けるため)
    std::thread th([this, send_str](){socket.sendMessage(send_str);});
    th.detach();
}

void SocketManager::end()
{
    promise.set_value(true);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    socket.EndListen();
}