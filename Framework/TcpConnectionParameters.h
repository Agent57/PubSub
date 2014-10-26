#pragma once

#include "ISerialiser.h"
#include "IProtocolBinder.h"
#include "IDomainManager.h"

#include <string>

class TcpConnectionParameters : public IConnectionParameters
{
private:
  bool m_isServer;
  std::string m_ipAddress;
  unsigned short m_port;
  SerialiserPtr m_serialiser;
  ProtocolBinderPtr m_protocol;
  DomainManagerPtr m_parent;

public:

  bool isServer() { return m_isServer; }
  void set_isServer(bool value) { m_isServer = value; }

  const std::string address() { return m_ipAddress; }
  void set_parent(std::string value) { m_ipAddress = value; }

  const unsigned short port() { return m_port; }
  void set_port(unsigned short value) { m_port = value; }

  const SerialiserPtr serialiser() { return m_serialiser; }
  void set_serialiser(const SerialiserPtr& serialiser) { m_serialiser = serialiser; }

  const ProtocolBinderPtr protocol() { return m_protocol; }
  void set_protocol(const ProtocolBinderPtr& value) { m_protocol = value; }

  const DomainManagerPtr parent() { return m_parent; }
  void set_parent(DomainManagerPtr value) { m_parent = value; }

};

typedef std::shared_ptr<TcpConnectionParameters> TcpConnectionParametersPtr;
