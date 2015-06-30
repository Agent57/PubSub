#pragma once

#include "ILogEventHandler.h"

class IDELogger : public ILogEventHandler
{
public:
  void HandleLogEvent(const LogEventDataPtr& pLog) override;
};
