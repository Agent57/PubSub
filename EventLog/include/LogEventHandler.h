#pragma once

#include "LogEventData.h"
#include "ILogFormatter.h"
#include "ILogWriter.h"

#include <memory>

class LogEventHandler
{
  LogLevel m_level;
  std::string m_text;
  LogWriterPtr m_writer;
  LogFormatterPtr m_formatter;

public:
  LogEventHandler();

  void OutputLogEvent();
  void SetLogFormatter(LogFormatterPtr formatter);
  void SetLogWriter(LogWriterPtr writer);
  void SetLogOutputLevel(LogLevel level);
  void SetLogOutput(const LogEventData& event);
  bool ValidLogOutput(const LogEventData& event) const;
};
typedef std::unique_ptr<LogEventHandler> LogEventHandlerPtr;