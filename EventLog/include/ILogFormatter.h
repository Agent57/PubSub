#pragma once

#include "LogEventData.h"

#include <memory>

class ILogFormatter
{
public:
  virtual ~ILogFormatter() {}
  virtual std::string FormatLogOutput(const LogEventData& event) { return ""; }
};
typedef std::unique_ptr<ILogFormatter> LogFormatterPtr;
