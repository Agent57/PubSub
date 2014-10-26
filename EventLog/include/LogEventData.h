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
private:
  const static std::string _levels[];
  long long m_time;
  LogLevel m_level;
  std::thread::id m_thread;
  std::string m_text;
  std::string m_file;
  std::string m_function;
  int m_line;

private:
  LogEventData();

public:
  static const std::string& Level(LogLevel level);
  LogEventData(LogLevel level,
                const std::string& text,
                const std::string& file,
                const std::string& function,
                const int line);
  const long long Time() const;
  const std::string DisplayTime() const;
  const std::string& Level() const;
  const std::thread::id& ThreadId() const;
  const std::string& Text() const;
  const std::string& File() const;
  const std::string& Function() const;
  const int Line() const;
};

typedef std::shared_ptr<LogEventData> LogEventDataPtr;
