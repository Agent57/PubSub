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
  static const std::string _level[];

  LogEventData() {}

public:
  std::string Severity;
  std::string DisplayTime;
  std::string Text;
  std::string File;
  std::string Function;
  int Line;
  std::thread::id ThreadId;

  LogEventData(LogLevel level,
               std::string time,
               std::string text,
               std::string file,
               std::string function,
               int line);

  static std::string Level(LogLevel level);
};

typedef std::unique_ptr<LogEventData> LogEventDataPtr;
