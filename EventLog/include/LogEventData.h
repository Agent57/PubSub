#pragma warning (disable:4251)
#pragma once

#include <memory>
#include <string>
#include <thread>

enum LogLevel
{
  Fatal,
  Error,
  Warning,
  Info,
  Debug,
  Trace
};


class LogEventData
{
  const static std::string _levels[];

  LogEventData() {}

public:
  std::string Severity;
  std::thread::id ThreadId;
  std::string DisplayTime;
  std::string Text;
  std::string File;
  std::string Function;
  int Line;

  LogEventData(LogLevel level,
                const std::string& time,
                const std::string& text,
                const std::string& file,
                const std::string& function,
                const int line);

  static std::string Level(LogLevel level);
};

typedef std::shared_ptr<LogEventData> LogEventDataPtr;
