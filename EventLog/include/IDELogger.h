#pragma once

#include "ILogEventHandler.h"

class IDELogger : public ILogEventHandler
{
public:
  ~IDELogger() override;
  void HandleLogEvent(const LogEventDataPtr& pLog) override;
};
