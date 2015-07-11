#pragma once

#include "ILogEventHandler.h"

class IDELogWriter : public ILogWriter
{
public:
  void WriteLogEvent(std::string) override;
};

class IDELogFormatter : public ILogFormatter
{
public:
  std::string FormatLogOutput(const LogEventData& event) override;
};
