#include <windows.h>

#include "Logger.h"
#include "IDELogger.h"
#include "ConsoleLogger.h"

#include <chrono>
#include <iomanip>
#include <memory>


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
  m_running = true;

  if (IsDebuggerPresent())
    AttachHandler(std::make_shared<IDELogger>());

  AttachHandler(std::make_shared<ConsoleLogger>());

#ifndef UNIT_TEST
  m_logger = std::thread(&Logger::LogWorker, this);
#endif // UNIT_TEST

  LoggerInfo("Application logging started");
}

Logger::~Logger()
{
  Shutdown();
  CallLogHandlers(std::make_shared<LogEventData>(Info,
                                                 "Application logging complete",
                                                 __FILE__, __FUNCTION__, __LINE__));
}

void Logger::Shutdown()
{
  if (!m_running)
    return;

  m_running = false;
  m_conditional.notify_all();

  if (m_logger.joinable())
    m_logger.join();

  // Finish processing any remaining log events
  CallLogHandlers(std::make_shared<LogEventData>(Warning,
                                                 "<<< Flushing log event queue before completing shutdown >>>",
                                                 __FILE__, __FUNCTION__, __LINE__));
  ProcessLogEvents(m_queue);
}

void Logger::SetLogLevel(LogLevel level)
{
  if (m_max_level == level)
    return;

  m_max_level = level;
  LoggerInfo("Log output level set to " << LogEventData::Level(level));
}

const std::chrono::high_resolution_clock::time_point Logger::StartTime() const
{
  return m_start;
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

void Logger::BufferEventQueue()
{
  if (m_queue->empty())
    m_conditional.wait(std::unique_lock<std::mutex>(m_lock));

  std::lock_guard<std::mutex> lock(m_lock);
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

void ConsoleLogger::HandleLogEvent(const LogEventDataPtr& pLog)
{
  if (pLog == nullptr)
    return;

  // Format log message output for the console
  std::cout << std::right << std::setfill('0') << std::setw(8) << pLog->DisplayTime()
    << "  " << std::setw(5) << pLog->ThreadId() << std::setfill(' ')
    << "  " << std::left << std::setw(9) << pLog->Level()
    << pLog->Text() << std::endl;
}

void IDELogger::HandleLogEvent(const LogEventDataPtr& pLog)
{
  if (pLog == nullptr)
    return;

  // Format log message output for the Visual Studio Debugger
  std::stringstream ss;

  ss << pLog->File()
     << "(" << pLog->Line() << "): "
     << std::setfill('0') << std::setw(8) << pLog->DisplayTime()
     << " [" << pLog->ThreadId() << "] "
     << pLog->Level() << ": "
     << pLog->Text() << std::endl;

  OutputDebugString(ss.str().c_str());
}
