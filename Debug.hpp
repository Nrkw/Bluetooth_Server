#pragma once
#include <iostream>
#include <chrono>
namespace Debug
{
    enum class Level
    {
        INFO = 1,
        WARNING = 2,
        ERR = 3,
    };
    class LogManager
    {
        public:
        LogManager()
        {

        }
        LogManager(Level level)
        {
            logLevel = level;
        }
        void setLogLevel(Level level)
        {
            logLevel = level;
        }
        template<typename T>
        void log(Level level, T data)
        {
            //ログレベルが設定されたレベルよりも低い場合はログを出力しない
            if(level < logLevel)
            {
                return;
            }
            auto now = std::chrono::system_clock::now();
            auto now_time = std::chrono::system_clock::to_time_t(now);
            auto local_time = std::localtime(&now_time);
            
            std::string time;
            char buffer[100];
            std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local_time);
            time = std::string(buffer);

            switch(level)
            {
                case Level::INFO:
                    std::cout << time << " [INFO] " << data << std::endl;
                    break;
                case Level::WARNING:
                    std::cout << time << " [WARNING] " << data << std::endl;
                    break;
                case Level::ERR:
                    std::cout << time << " [ERROR] " << data << std::endl;
                    break;
            }
        }

        private:
        Level logLevel = Level::WARNING;

    };
}