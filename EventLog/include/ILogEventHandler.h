#pragma once

#include "LogEventData.h"

#include <memory>

class ILogWriter
{
public:
  virtual ~ILogWriter() {}
  virtual void WriteLogEvent(std::string) = 0;
};
typedef std::unique_ptr<ILogWriter> LogWriterPtr;

class ILogEventHandler
{
protected:
  LogLevel m_level;
  std::string m_text;
  LogWriterPtr m_writer;

public:
  virtual ~ILogEventHandler() {}
  virtual void FormatLogOutput(const LogEventData& event) = 0;
  
  void OutputLogEvent()
  {
    m_writer->WriteLogEvent(m_text);
  }

  void SetLogWriter(LogWriterPtr writer)
  {
    m_writer = move(writer);
  }

  void SetLogOutputLevel(LogLevel level)
  {
    m_level = level;
  }
  
  void SetLogOutput(const std::string& text)
  {
    m_text = text;
  }

  bool ValidLogOutput(const LogEventData& event)
  {
    return (event.Level <= m_level);
  }
};

typedef std::unique_ptr<ILogEventHandler> LogEventHandlerPtr;