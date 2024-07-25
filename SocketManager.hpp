#pragma once
#include <future>
#include <functional>
#include <mutex>


#include "socket.hpp"
#include "Data.hpp"
#include "Debug.hpp"
#include "Event.hpp"
class SocketManager
{
    public:
    SocketManager();
    ~SocketManager();

    /*
        Bluetooth機器から情報が送られてきた時の情報を受け取る関数を設定して、サーバーを起動する。
        @param callback コールバック関数
    */
    void start(std::function<void(Data)> callback);

    /*
        Bluetooth機器から情報が送られてきた時の情報を受け取る関数を変更する。
        @param callback コールバック関数
    */
    void setEventFunction(std::function<void(Data)> callback);
    std::function<void(Data data)> getEventFunction()
    {
        return callbackFunc;
    }
    void end();
    void sendEvent(Event event, std::string data_str);
    void setLogLevel(Debug::Level level)
    {
        log_mgr.setLogLevel(level);
    }



    private:

    std::promise<bool> promise;
    std::future<bool> future;
    Debug::LogManager log_mgr = Debug::LogManager(Debug::Level::WARNING);

    bool isEnd = false;
    Socket socket;
    
    
    void startServer();
    std::function<void(Data)> callbackFunc;
};