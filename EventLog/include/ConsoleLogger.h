#pragma once

#include "ILogEventHandler.h"

class ConsoleLogger : public ILogEventHandler
{
public:
  ~ConsoleLogger() override;
public:
  void HandleLogEvent(const LogEventDataPtr& pLog) override;
};
