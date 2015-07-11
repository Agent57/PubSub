#pragma once

#include "LogEventHandler.h"

class ConsoleLogWriter : public ILogWriter
{
public:
  void WriteLogEvent(std::string) override;
};

class ConsoleLogFormatter : public ILogFormatter
{
public:
  std::string FormatLogOutput(const LogEventData& event) override;
};
