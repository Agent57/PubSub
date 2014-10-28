#include "LogEventData.h"
#include "Logger.h"

#include <iomanip>

const std::string LogEventData::_levels[] =
{
  "Fatal",
  "Error",
  "Warning",
  "Info",
  "Debug",
  "Trace"
};

const std::string& LogEventData::Level(LogLevel level)
{
  return _levels[level];
}

LogEventData::LogEventData(LogLevel level,
                            const std::string& text,
                            const std::string& file,
                            const std::string& function,
                            const int line)
{
  // Get the system clock time
  auto now = std::chrono::high_resolution_clock::now();

  // Calculate the time of this logging event since the application started
  auto start = Logger::Singleton().StartTime();
  m_time = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();

  // Fill in the remaining LogEvent details
  m_level = level;
  m_thread = std::this_thread::get_id();
  m_text = text;
  m_file = file;
  m_function = function;
  m_line = line;
}

const long long LogEventData::Time() const
{
  return m_time;
}

const std::string LogEventData::DisplayTime() const
{
  // Convert the internal log time value into regular clock time
  int milliseconds = m_time % 1000;
  long long x = m_time / 1000;
  int seconds = x % 60;
  x /= 60;
  int minutes = x % 60;
  x /= 60;
  int hours = x % 24;
  x /= 24;
  long long days = x;

  // Output the time in an easily readable format
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(3) << days << ":"
                          << std::setw(2) << hours << ":"
                          << std::setw(2) << minutes << ":"
                          << std::setw(2) << seconds << ":"
                          << std::setw(3) << milliseconds;
  return ss.str();
}

const std::string& LogEventData::Level() const
{
  return _levels[m_level];
}

const std::thread::id& LogEventData::ThreadId() const
{
  return m_thread;
}

const std::string& LogEventData::Text() const
{
  return m_text;
}

const std::string& LogEventData::File() const
{
  return m_file;
}

const std::string& LogEventData::Function() const
{
  return m_function;
}

const int LogEventData::Line() const
{
  return m_line;
}
