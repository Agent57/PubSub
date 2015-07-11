#pragma once

#include "ILogEventHandler.h"

class IDEWriter : public ILogWriter
{
public:
  void WriteLogEvent(std::string) override;
};

class IDELogger : public ILogEventHandler
{
public:
  IDELogger()
  {
    SetLogOutputLevel(Trace);
  }

  void FormatLogOutput(const LogEventData& event) override;
};
