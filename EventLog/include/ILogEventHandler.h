#pragma once

#include "LogEventData.h"

#include <memory>

class ILogEventHandler
{
public:
  virtual ~ILogEventHandler() {}
  virtual void HandleLogEvent(const LogEventDataPtr& pLog) = 0;
};

typedef std::unique_ptr<ILogEventHandler> LogEventHandlerPtr;