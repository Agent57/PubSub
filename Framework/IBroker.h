#pragma once

#include <memory>
#include <set>

#include "IConnector.h"

class IBroker
{
public:
  virtual ~IBroker() {};
  virtual void Subscribe(const std::string& type, const ConnectorPtr& connector) = 0;
  virtual void Unsubscribe(const std::string& type, const ConnectorPtr& connector) = 0;
  virtual void Unsubscribe(const std::set<std::string>& type, const ConnectorPtr& connector) = 0;
  virtual bool Send(const MessagePtr& msg) = 0;
};

typedef std::shared_ptr<IBroker> BrokerPtr;