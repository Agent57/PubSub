#include "Logger.h"
#include "TcpConnector.h"

#include "wrapper.pb.h"

TcpConnector::TcpConnector()
{
  m_connected = false;
  m_params = nullptr;

  // Initialise Microsoft winsock
  WORD wVersionRequested;
  WSADATA wsaData;
  int err;

  wVersionRequested = MAKEWORD(2, 2);
  err = WSAStartup(wVersionRequested, &wsaData);
  if (err != 0)
  {
    LogEvent(Error, "WSAStartup() failed with error: " << err);
    return;
  }

  if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
  {
    LogEvent(Error, "Could not find a usable version of Winsock.dll");
    return;
  }
}

TcpConnector::~TcpConnector()
{
  Shutdown();
  WSACleanup();
}

void TcpConnector::OpenServer()
{
  return;
}

void TcpConnector::OpenClient()
{
  return;
}

bool TcpConnector::Open(const ConnectionParametersPtr& connectionParameters)
{
  m_params = std::dynamic_pointer_cast<TcpConnectionParameters>(connectionParameters);
  if (m_params == nullptr)
  {
    LogEvent(Error, "The supplied connection parameters are not suitable for creating a TCP socket");
    return false;
  }

  // Determine whether to start our socket thread as client or server
  if (m_params->isServer())
  {
    m_thread = std::thread(&TcpConnector::OpenServer, this);
  }
  else
  {
    m_thread = std::thread(&TcpConnector::OpenClient, this);
  }

  return true;
}

void TcpConnector::Shutdown()
{
  // Perform clean closedown if we have an established socket connection
  if (m_connected && !m_socketError)
    shutdown(m_socket, SD_SEND);

  // Close the socket
  closesocket(m_socket);
  m_connected = false;
}

bool TcpConnector::Send(const MessagePtr& msg)
{
  // Check that we have a connection first
  if (m_connected || !m_params)
    return false;

  // Serialise the message into a byte array
  DataBufferPtr buffer = m_params->serialiser()->Serialise(msg);
  int bufferSize = buffer->size();

  // Ensure synchronisation of multithreaded write access to the socket
  std::lock_guard<std::mutex> lock(m_lock);
  {
    int count = 0;
    int val;

    do
    {
      val = send(m_socket, buffer->data(), bufferSize, 0);
      if (val == -1)
      {
        LogEvent(Error, "Failed to send data on TCP socket connection");
        delete[] &buffer;
        return false;
      }
    }while (count < bufferSize);
  }

  return true;
}

const MessagePtr TcpConnector::Read()
{
  // Initialise the message to be null
  MessagePtr pMsg = nullptr;

  // Check that we have a connection
  if (!m_connected)
    return pMsg;

  // Read first 4 bytes 
//  int packetLength;


  return pMsg;
}

const char* TcpConnector::Receive()
{
  return NULL;
}