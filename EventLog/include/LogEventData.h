#pragma once

#include <memory>
#include <string>
#include <thread>

typedef enum
{
  Special,
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
  std::string m_text;

  LogEventData() {}

public:
  LogLevel Level;
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

  std::string Severity() const;

  static std::string Severity(LogLevel level);
};

typedef std::unique_ptr<LogEventData> LogEventDataPtr;
