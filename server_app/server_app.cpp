// server_app.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <dos.h>
#include <sys/types.h>
#include <chrono>
#include <thread>
//#include <sys/socket.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;


int main()
{
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        cout << "WSAStartup fail:" << iResult;
        return 1;
    }
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
        if (sockfd <0)
        {
            cout << "Socket error" << WSAGetLastError();
            return 1;
        }
    struct sockaddr_in myAddr;
    myAddr.sin_family = AF_INET;
    myAddr.sin_port = htons(8080);
    myAddr.sin_addr.s_addr = INADDR_ANY;

    memset(myAddr.sin_zero, '\0', sizeof(myAddr.sin_zero));

    int binding = bind(sockfd, (struct sockaddr*)&myAddr, sizeof(myAddr));
    if(binding!=0)
    {
        cout << "Bind problem" << WSAGetLastError();
        WSACleanup();
        closesocket(sockfd);
        return 1;
    }

    int listening = listen(sockfd, 10);
    if (listening != 0)
    {
        cout << "listening error" << WSAGetLastError();
        WSACleanup();
        return 1;

    }
    const char * msg = "eeee";

    while (1)
    {
        struct sockaddr_storage their_addr;
        int addr_size = sizeof(their_addr);
        int fdClient = accept(sockfd, (struct sockaddr*)&their_addr, &addr_size);
        cout << fdClient;
        if (fdClient < 0)
        {
            cout << "Accept problem:" << WSAGetLastError();
            closesocket(fdClient);
            WSACleanup();
            return 1;
        }
        int recieve = send(fdClient, msg, strlen(msg),0);
        if (recieve < 0)
        {
            cout << "problem with sending : " << WSAGetLastError();
            closesocket(fdClient);
            WSACleanup();
            return 1;
        }
     

        closesocket(fdClient);
        //this_thread::sleep_for(chrono::milliseconds(1000));
        
       
    }
    closesocket(sockfd);
    WSACleanup();
}

