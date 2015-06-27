#include "LogEventData.h"

const std::string LogEventData::_levels[] =
{
  "Fatal",
  "Error",
  "Warning",
  "Info",
  "Debug",
  "Trace"
};

std::string LogEventData::Level(LogLevel level)
{
  return _levels[level];
}

LogEventData::LogEventData(LogLevel level,
                           const std::string& time,
                           const std::string& text,
                           const std::string& file,
                           const std::string& function,
                           const int line)
                           : Severity(_levels[level]),
                           DisplayTime(time),
                           Text(text),
                           File(file),
                           Function(function),
                           Line(line)
{
  // Fill in the remaining LogEvent details
  ThreadId = std::this_thread::get_id();
}
