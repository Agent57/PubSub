#pragma warning (disable:4251)
#pragma once

#include <WinSock2.h>
#include <Windows.h>

#include <memory>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

#include "IConnector.h"
#include "IMessage.h"
#include "TcpConnectionParameters.h"
#include "TimedEventQueue.h"

class TcpConnector :
  public IConnector
{
private:
  TcpConnectionParametersPtr m_params;
  std::queue<MessagePtr> m_queue;
  std::mutex m_lock;
  std::condition_variable m_conditional;
  std::atomic_bool m_connected;
  std::atomic_bool m_socketError;
  TimedEventQueue m_timer;
  SOCKET m_socket;
  std::thread m_thread;

public:
  TcpConnector(void);
  ~TcpConnector(void);

  bool Open(const ConnectionParametersPtr& connectionParameters);
  void OpenServer();
  void OpenClient();
  void Shutdown();
  bool Send(const MessagePtr& msg);
  const MessagePtr Read();
  const char* TcpConnector::Receive();
  const TimedEventPtr SetTimer(const int milliseconds, const MessagePtr& msg, bool repeatable = false)
  {
    return nullptr;
  }
  const MessagePtr StopTimer(const int timerId) { return nullptr; }
};


//#undef UNICODE
//
//#define WIN32_LEAN_AND_MEAN
//
//#include <windows.h>
//#include <winsock2.h>
//#include <ws2tcpip.h>
//#include <stdlib.h>
//#include <stdio.h>
//
//// Need to link with Ws2_32.lib
//#pragma comment (lib, "Ws2_32.lib")
//// #pragma comment (lib, "Mswsock.lib")
//
//#define DEFAULT_BUFLEN 512
//#define DEFAULT_PORT "27015"
//
//int __cdecl main(void) 
//{
//    WSADATA wsaData;
//    int iResult;
//
//    SOCKET ListenSocket = INVALID_SOCKET;
//    SOCKET ClientSocket = INVALID_SOCKET;
//
//    struct addrinfo *result = NULL;
//    struct addrinfo hints;
//
//    int iSendResult;
//    char recvbuf[DEFAULT_BUFLEN];
//    int recvbuflen = DEFAULT_BUFLEN;
//    
//    // Initialize Winsock
//    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
//    if (iResult != 0) {
//        printf("WSAStartup failed with error: %d\n", iResult);
//        return 1;
//    }
//
//    ZeroMemory(&hints, sizeof(hints));
//    hints.ai_family = AF_INET;
//    hints.ai_socktype = SOCK_STREAM;
//    hints.ai_protocol = IPPROTO_TCP;
//    hints.ai_flags = AI_PASSIVE;
//
//    // Resolve the server address and port
//    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
//    if ( iResult != 0 ) {
//        printf("getaddrinfo failed with error: %d\n", iResult);
//        WSACleanup();
//        return 1;
//    }
//
//    // Create a SOCKET for connecting to server
//    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
//    if (ListenSocket == INVALID_SOCKET) {
//        printf("socket failed with error: %ld\n", WSAGetLastError());
//        freeaddrinfo(result);
//        WSACleanup();
//        return 1;
//    }
//
//    // Setup the TCP listening socket
//    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
//    if (iResult == SOCKET_ERROR) {
//        printf("bind failed with error: %d\n", WSAGetLastError());
//        freeaddrinfo(result);
//        closesocket(ListenSocket);
//        WSACleanup();
//        return 1;
//    }
//
//    freeaddrinfo(result);
//
//    iResult = listen(ListenSocket, SOMAXCONN);
//    if (iResult == SOCKET_ERROR) {
//        printf("listen failed with error: %d\n", WSAGetLastError());
//        closesocket(ListenSocket);
//        WSACleanup();
//        return 1;
//    }
//
//    // Accept a client socket
//    ClientSocket = accept(ListenSocket, NULL, NULL);
//    if (ClientSocket == INVALID_SOCKET) {
//        printf("accept failed with error: %d\n", WSAGetLastError());
//        closesocket(ListenSocket);
//        WSACleanup();
//        return 1;
//    }
//
//    // No longer need server socket
//    closesocket(ListenSocket);
//
//    // Receive until the peer shuts down the connection
//    do {
//
//        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
//        if (iResult > 0) {
//            printf("Bytes received: %d\n", iResult);
//
//        // Echo the buffer back to the sender
//            iSendResult = send( ClientSocket, recvbuf, iResult, 0 );
//            if (iSendResult == SOCKET_ERROR) {
//                printf("send failed with error: %d\n", WSAGetLastError());
//                closesocket(ClientSocket);
//                WSACleanup();
//                return 1;
//            }
//            printf("Bytes sent: %d\n", iSendResult);
//        }
//        else if (iResult == 0)
//            printf("Connection closing...\n");
//        else  {
//            printf("recv failed with error: %d\n", WSAGetLastError());
//            closesocket(ClientSocket);
//            WSACleanup();
//            return 1;
//        }
//
//    } while (iResult > 0);
//
//    // shutdown the connection since we're done
//    iResult = shutdown(ClientSocket, SD_SEND);
//    if (iResult == SOCKET_ERROR) {
//        printf("shutdown failed with error: %d\n", WSAGetLastError());
//        closesocket(ClientSocket);
//        WSACleanup();
//        return 1;
//    }
//
//    // cleanup
//    closesocket(ClientSocket);
//    WSACleanup();
//
//    return 0;
//}
