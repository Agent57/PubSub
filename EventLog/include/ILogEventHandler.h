#pragma once

#include "LogEventData.h"

#include <memory>

class ILogWriter
{
public:
  virtual ~ILogWriter() {}
  virtual void WriteLogEvent(std::string) {};
};
typedef std::unique_ptr<ILogWriter> LogWriterPtr;

class ILogFormatter
{
public:
  virtual ~ILogFormatter() {}
  virtual std::string FormatLogOutput(const LogEventData& event) { return ""; }
};
typedef std::unique_ptr<ILogFormatter> LogFormatterPtr;

class LogEventHandler
{
  LogLevel m_level;
  std::string m_text;
  LogWriterPtr m_writer;
  LogFormatterPtr m_formatter;

public:
  LogEventHandler()
  {
    m_level = Trace;
  }

  void OutputLogEvent()
  {
    m_writer->WriteLogEvent(m_text);
  }

  void SetLogFormatter(LogFormatterPtr formatter)
  {
    m_formatter = move(formatter);
  }

  void SetLogWriter(LogWriterPtr writer)
  {
    m_writer = move(writer);
  }

  void SetLogOutputLevel(LogLevel level)
  {
    m_level = level;
  }
  
  void SetLogOutput(const LogEventData& event)
  {
    m_text = m_formatter->FormatLogOutput(event);
  }

  bool ValidLogOutput(const LogEventData& event) const
  {
    return (event.Level <= m_level);
  }
};
typedef std::unique_ptr<LogEventHandler> LogEventHandlerPtr;