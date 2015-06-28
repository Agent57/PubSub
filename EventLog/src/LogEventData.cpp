#include "LogEventData.h"

const std::string LogEventData::_level[] =
{
  "Fatal",
  "Error",
  "Warning",
  "Info",
  "Debug",
  "Trace"
};

LogEventData::LogEventData(LogLevel level,
                           std::string time,
                           std::string text,
                           std::string file,
                           std::string function,
                           int line)
                           : Severity(move(_level[level])),
                             DisplayTime(move(time)),
                             Text(move(text)),
                             File(move(file)),
                             Function(move(function)),
                             Line(line)
{
  ThreadId = std::this_thread::get_id();
}

std::string LogEventData::Level(LogLevel level)
{
  return _level[level];
}
