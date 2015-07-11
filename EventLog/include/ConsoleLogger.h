#pragma once

#include "ILogEventHandler.h"

class ConsoleWriter : public ILogWriter
{
public:
  void WriteLogEvent(std::string) override;
};

class ConsoleLogger : public ILogEventHandler
{
public:
  ConsoleLogger()
  {
    SetLogOutputLevel(Trace);
  }

  void FormatLogOutput(const LogEventData& event) override;
};
