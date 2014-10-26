#pragma once

#include "IConnectionParameters.h"
#include "IMessage.h"
#include "ITimedEvent.h"

class IConnector
{
public:
  virtual bool Open(const ConnectionParametersPtr& connectionParameters) = 0;
  virtual void Shutdown(void) = 0;
  virtual bool Send(const MessagePtr& msg) = 0;
  virtual const MessagePtr Read(void) = 0;
  virtual const TimedEventPtr SetTimer(const int milliseconds, const MessagePtr& msg, bool repeatable = false) = 0;
  virtual const MessagePtr StopTimer(const int timerId) = 0;
};

typedef std::shared_ptr<IConnector> ConnectorPtr;