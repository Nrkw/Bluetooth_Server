#include <WinSock2.h>
#include <ws2bth.h>
#include <bthdef.h>
#include "socket.hpp"
#include <string>
#include <iostream>

Socket::Socket()
{

    WSADATA wsaData;
    int iResult = WSAStartup( MAKEWORD(2, 2), &wsaData );
    if( iResult != NO_ERROR )
    {
        throw std::string("Error at WSAStartup(): " + std::to_string(WSAGetLastError()));
    }
    listen_socket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);

    if (listen_socket == INVALID_SOCKET)
    {
        throw std::string("Error at socket(): " + std::to_string(WSAGetLastError()));
    }

    SOCKADDR_BTH sa = {0};
    sa.addressFamily = AF_BTH;
    sa.port = BT_PORT_ANY;
    if(bind(listen_socket, (SOCKADDR*)&sa, sizeof(sa)) == SOCKET_ERROR)
    {
        throw std::string("Error at bind(): " + std::to_string(WSAGetLastError()));
    }
    int size = sizeof(sa);
    getsockname(listen_socket, (SOCKADDR*)&sa, &size);

    CSADDR_INFO info = { 0 };
    info.LocalAddr.lpSockaddr = (LPSOCKADDR)&sa;
    info.LocalAddr.iSockaddrLength = sizeof(sa);
    info.iSocketType = SOCK_STREAM;
    info.iProtocol = BTHPROTO_RFCOMM;



    WSAQUERYSET set = { 0 };
    set.dwSize = sizeof(WSAQUERYSET);                              // Must be set to sizeof(WSAQUERYSET)
    set.dwOutputFlags = 0;                                         // Not used
    set.lpszServiceInstanceName = (LPSTR)L"Server";                        // Recommended.
    //set.lpServiceClassId = (LPGUID)&SerialPortServiceClass_UUID;   // Requred.
//    set.lpServiceClassId = (LPGUID)0x1101;   // Requred.
//    LPCOLESTR uuidSTR = L"{0000110a-0000-1000-8000-00805f9b34fb}";
LPCOLESTR uuidSTR = L"{00001101-0000-1000-8000-00805f9b34fb}";
    GUID uuid;
    HRESULT result = IIDFromString(uuidSTR, &uuid);
    if(SUCCEEDED(result))
    {
        set.lpServiceClassId = (LPGUID)&uuid;
    }
    else
    {
        throw std::string("Error at IIDFromString(): " + std::to_string(WSAGetLastError()));
    }

    set.lpVersion = NULL;                                          // Not used.
    set.lpszComment = NULL;                                        // Optional.
    set.dwNameSpace = NS_BTH;                                      // Must be NS_BTH.
    set.lpNSProviderId = NULL;                                     // Not required.
    set.lpszContext = NULL;                                        // Not used.
    set.dwNumberOfProtocols = 0;                                   // Not used.
    set.lpafpProtocols = NULL;                                     // Not used.
    set.lpszQueryString = NULL;                                    // not used.
    set.dwNumberOfCsAddrs = 1;                                     // Must be 1.
    set.lpcsaBuffer = &info;                                       // Pointer to a CSADDR_INFO.
    set.lpBlob = NULL;                                             // Optional.


    if (WSASetService(&set, RNRSERVICE_REGISTER, 0) != 0) 
    {
        throw std::string("Error at WSASetService(): " + std::to_string(WSAGetLastError()));
    }
}

Socket::~Socket()
{
    closesocket(listen_socket);
    closesocket(communciate_socket);
    WSACleanup();
}
void Socket::startListen()
{
    listen(listen_socket, 0);

    SOCKADDR_BTH sab2;
    int ilen = sizeof(sab2);
    communciate_socket = accept(listen_socket, (SOCKADDR *)&sab2, &ilen);

}

void Socket::sendMessage(std::string str)
{
    int size = str.size();
    char *data = new char[size];
    data = (char*)str.c_str();
    //送信データを格納するchar型の配列
    char *buf = new char[size];

    //送信データを格納
    memcpy(buf, data, size);

    //socketにデータを送信
    send(communciate_socket, buf, size, 0);
    delete buf;
    delete data;
}
std::string Socket::receiveMessage()
{
    char *buf = new char[longest_receive_size];

    //socketからデータを受信し、char型のポインタに受信データを格納
    int res = recv(communciate_socket, buf, sizeof(char)*longest_receive_size, 0);
    
    if (res > 0) 
    {
        //正常に受信できた場合（受信データが1バイト以上の場合）
        return std::string(buf);
    }
    return "";

    delete buf;

}
void Socket::EndListen()
{
    
    closesocket(listen_socket);
    closesocket(communciate_socket);

    WSACleanup();
}

//最大受信サイズを設定（charを最大size個まで受信可能）
void Socket::setLongestReceiveSize(int size)
{
    longest_receive_size = size;
}
