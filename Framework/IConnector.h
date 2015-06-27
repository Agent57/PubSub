#pragma once

#include "IConnectionParameters.h"
#include "IMessage.h"
#include "ITimedEvent.h"

class IConnector
{
public:
  virtual ~IConnector() {}
  virtual bool Open(const ConnectionParametersPtr& connectionParameters) = 0;
  virtual void Shutdown(void) = 0;
  virtual bool Send(const MessagePtr& msg) = 0;
  virtual MessagePtr Read(void) = 0;
  virtual TimedEventPtr SetTimer(const int milliseconds, const MessagePtr& msg, bool repeatable = false) = 0;
  virtual MessagePtr StopTimer(const int timerId) = 0;
};

typedef std::shared_ptr<IConnector> ConnectorPtr;
