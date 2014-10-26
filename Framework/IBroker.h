#pragma once

#include <string>
#include <memory>

#include "IConnector.h"
#include "ITimedEvent.h"

class IBroker
{
public:
  virtual void Subscribe(const std::string& type, const ConnectorPtr& connector) = 0;
  virtual void Unsubscribe(const std::string& type, const ConnectorPtr& connector) = 0;
  virtual bool Send(const MessagePtr& msg) = 0;
};

typedef std::shared_ptr<IBroker> BrokerPtr;