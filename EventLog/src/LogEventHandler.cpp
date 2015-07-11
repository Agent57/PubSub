#include "LogEventHandler.h"

LogEventHandler::LogEventHandler()
{
  m_level = Trace;
}

void LogEventHandler::OutputLogEvent()
{
  m_writer->WriteLogEvent(m_text);
}

void LogEventHandler::SetLogFormatter(LogFormatterPtr formatter)
{
  m_formatter = move(formatter);
}

void LogEventHandler::SetLogWriter(LogWriterPtr writer)
{
  m_writer = move(writer);
}

void LogEventHandler::SetLogOutputLevel(LogLevel level)
{
  m_level = level;
}

void LogEventHandler::SetLogOutput(const LogEventData& event)
{
  m_text = m_formatter->FormatLogOutput(event);
}

bool LogEventHandler::ValidLogOutput(const LogEventData& event) const
{
  return (event.Level <= m_level);
}
