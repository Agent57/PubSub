#include <windows.h>

#include "Logger.h"
#include "IDELogger.h"

#include <chrono>
#include <iomanip>

std::chrono::high_resolution_clock::time_point Logger::m_start;

Logger& Logger::Singleton()
{
  static Logger instance;
  return instance;
}

Logger::Logger()
{
  m_queue = std::make_shared<LogEventQueue>();
  m_buffer = std::make_shared<LogEventQueue>();
  m_start = std::chrono::high_resolution_clock::now();
  m_enabled =  true;
  m_max_level = Trace;
  m_running = false;

  LoggerInfo("Application logging started");
}

Logger::~Logger()
{
  LoggerInfo("Application logging complete");
  std::lock_guard<std::mutex> lock(m_lock);
  Shutdown();
}

void Logger::Start()
{
  m_running = true;
  if (IsDebuggerPresent())
    AttachHandler(std::make_shared<IDELogger>());

  m_logger = std::thread(&Logger::LogWorker, this);
}

void Logger::Shutdown()
{
  if (!m_running)
    return;

  m_running = false;
  m_conditional.notify_all();

  if (m_logger.joinable())
    m_logger.join();

  ProcessLogEvents(m_queue);
}

void Logger::SetLogLevel(LogLevel level)
{
  if (m_max_level == level)
    return;

  m_max_level = level;
  LoggerInfo("Log output level set to " << LogEventData::Level(level));
}

bool Logger::CheckLogLevel(LogLevel level) const
{
  return level <= m_max_level && m_enabled;
}

void Logger::Enabled(bool enable)
{
  if (m_enabled == enable)
    return;

  m_enabled = enable;
  LoggerInfo("Logging output is now " << m_enabled ? "enabled" : "disabled");
}

bool Logger::Enabled() const
{
  return m_enabled;
}

void Logger::LogStream(const LogEventDataPtr& pLog)
{
  std::lock_guard<std::mutex> lock(m_lock);
  m_queue->push_back(pLog);
  m_conditional.notify_all();
}

void Logger::LogWorker()
{
  while(m_running)
  {
    BufferEventQueue();
    ProcessLogEvents(m_buffer);
  }
}

std::string Logger::RunTime()
{
  // Convert the internal log time value into regular clock time
  auto now = std::chrono::high_resolution_clock::now();
  auto runtime = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_start).count();
  int milliseconds = runtime % 1000;
  int seconds = runtime / 1000 % 60;
  int minutes = runtime / 60000 % 60;
  int hours = runtime / 3600000 % 24;
  auto days = runtime / 86400000;

  // Output the time in an easily readable format
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(3) << days << ":"
    << std::setw(2) << hours << ":"
    << std::setw(2) << minutes << ":"
    << std::setw(2) << seconds << ":"
    << std::setw(3) << milliseconds;
  return ss.str();
}

void Logger::BufferEventQueue()
{
  if (m_queue->empty())
  {
    auto waitlock = std::unique_lock<std::mutex>(m_lock);
    m_conditional.wait(waitlock);
  }

  std::lock_guard<std::mutex> lock(m_lock);
  m_buffer->clear();
  auto temp = m_buffer;
  m_buffer = m_queue;
  m_queue = temp;
}

void Logger::ProcessLogEvents(const LogEventQueuePtr& events)
{
  while(!events->empty())
  {
    const LogEventDataPtr pLog = events->front();
    events->pop_front();
    CallLogHandlers(pLog);
  }
}

void Logger::AttachHandler(const LogEventHandlerPtr& pHandler)
{
  std::lock_guard<std::mutex> lock(m_handlerLock);
  m_handlers.push_back(pHandler);
}

void Logger::DetachHandler(const LogEventHandlerPtr& pHandler)
{
  std::lock_guard<std::mutex> lock(m_handlerLock);
  m_handlers.remove(pHandler);
}

void Logger::CallLogHandlers(const LogEventDataPtr& pLog)
{
  std::lock_guard<std::mutex> lock(m_handlerLock);
  for (LogEventHandlerSetItem item = m_handlers.begin(); item != m_handlers.end(); ++item)
  {
    (*item)->HandleLogEvent(pLog);
  }
}
