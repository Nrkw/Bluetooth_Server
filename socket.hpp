#pragma once
#include <WinSock2.h>
#include <ws2bth.h>
#include <string>
class Socket
{
    public:

    Socket();
    ~Socket();

    void startListen();

    void sendMessage(std::string str);
    std::string receiveMessage();
    void EndListen();

    void setLongestReceiveSize(int size);

    private:

    int longest_receive_size = 1024;
    //他のデバイスからの接続を待ち受けるソケット
    SOCKET listen_socket;

    //接続された後にデータの送受信を行うソケット
    SOCKET communciate_socket;
};