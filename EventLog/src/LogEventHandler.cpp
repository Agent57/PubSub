#include "LogEventHandler.h"

LogEventHandler::LogEventHandler()
{
  m_level = Info;
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

bool LogEventHandler::SetLogOutputLevel(LogLevel level)
{
  if (m_level == level || m_level == Special)
    return false;

  m_level = level;
  return true;
}

void LogEventHandler::SetLogOutput(const LogEventData& event)
{
  m_text = m_formatter->FormatLogOutput(event);
}

bool LogEventHandler::ValidLogOutput(const LogEventData& event) const
{
  return (event.Level <= m_level);
}
