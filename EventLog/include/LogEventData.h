#pragma warning (disable:4251)
#pragma once

#include <memory>
#include <string>
#include <thread>

typedef enum
{
  Fatal,
  Error,
  Warning,
  Info,
  Debug,
  Trace
} LogLevel;

class LogEventData
{
  static const std::string _levels[];

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
                std::string time,
                std::string text,
                std::string file,
                std::string function,
                int line);

  static std::string Level(LogLevel level);
};

typedef std::shared_ptr<LogEventData> LogEventDataPtr;
