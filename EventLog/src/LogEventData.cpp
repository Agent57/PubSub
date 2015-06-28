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
                           std::string time,
                           std::string text,
                           std::string file,
                           std::string function,
                           int line)
                           : Severity(_levels[level]),
                             DisplayTime(time),
                             Text(text),
                             File(file),
                             Function(function),
                             Line(line)
{
  ThreadId = std::this_thread::get_id();
}
