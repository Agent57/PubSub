#pragma once

#include "ILogEventHandler.h"

class ConsoleLogger : public ILogEventHandler
{
public:
  void HandleLogEvent(const LogEventDataPtr& pLog);
};
