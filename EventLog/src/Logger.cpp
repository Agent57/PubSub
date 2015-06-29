#include <windows.h>

#include "Logger.h"
#include "IDELogger.h"

#include <chrono>
#include <iomanip>

Logger::Logger()
{
  m_queue = std::make_unique<LogEventQueue>();
  m_buffer = std::make_unique<LogEventQueue>();
  m_start = std::chrono::high_resolution_clock::now();
  m_enabled =  true;
  m_max_level = Info;
  m_running = false;

  LogEventData pLog(Info, ElapsedRunTime(), "Application logging started", __FILE__, __FUNCTION__, __LINE__);
  QueueLogEvent(pLog);
}

Logger& Logger::Instance()
{
  static Logger instance;
  return instance;
}

Logger::~Logger()
{
  if (!m_running)
    return;

  m_running = false;
  LogEventData pLog(Info, ElapsedRunTime(), "Application logging complete", __FILE__, __FUNCTION__, __LINE__);
  QueueLogEvent(pLog);

  if (m_logger.joinable())
    m_logger.join();

  ProcessLogEvents(m_queue);
}

void Logger::Start()
{
  if (m_running)
    return;

  m_running = true;

  if (IsDebuggerPresent())
    m_handlers.push_back(std::make_unique<IDELogger>());

  m_logger = std::thread(&Logger::LogWorker, this);
}

void Logger::LogWorker()
{
  while (m_running)
  {
    BufferEventQueue();
    ProcessLogEvents(m_buffer);
  }
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
  auto temp = move(m_buffer);
  m_buffer = move(m_queue);
  m_queue = move(temp);
}

void Logger::ProcessLogEvents(const LogEventQueuePtr& events)
{
  while (!events->empty())
  {
    const auto& pLog = events->front();
    CallLogHandlers(pLog);
    events->pop_front();
  }
}

void Logger::CallLogHandlers(const LogEventDataPtr& pLog)
{
  std::lock_guard<std::mutex> lock(m_handlerLock);
  for (const auto& handler : m_handlers)
  {
    handler->HandleLogEvent(pLog);
  }
}

// Private implementation of static accessors
std::string Logger::ElapsedRunTimeImpl()
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

void Logger::QueueLogEventImpl(const LogEventData& pLog)
{
  std::lock_guard<std::mutex> lock(m_lock);
  m_queue->push_back(std::make_unique<LogEventData>(pLog));
  m_conditional.notify_all();
}

void Logger::SetLogLevelImpl(LogLevel level)
{
  if (m_max_level == level)
    return;

  m_max_level = level;

  std::ostringstream ss;
  ss << "Log output level set to " << LogEventData::Level(level);
  LogEventData pLog(Info, ElapsedRunTime(), ss.str(), __FILE__, __FUNCTION__, __LINE__);
  QueueLogEvent(pLog);
}

void Logger::SetEnabledImpl(bool enable)
{
    if (m_enabled == enable)
    return;

  m_enabled = enable;

  std::ostringstream ss;
  ss << "Logging output is now " << (m_enabled ? "enabled" : "disabled");
  LogEventData pLog(Info, ElapsedRunTime(), ss.str(), __FILE__, __FUNCTION__, __LINE__);
  QueueLogEvent(pLog);
}

bool Logger::IsEnabledImpl() const
{
  return m_enabled;
}

void Logger::AttachHandlerImpl(LogEventHandlerPtr pHandler)
{
  std::lock_guard<std::mutex> lock(m_handlerLock);
  m_handlers.push_back(move(pHandler));
  Start();
}

bool Logger::CheckLogAccessImpl(LogLevel level) const
{
  return level <= m_max_level && m_enabled && m_running;
}

// Static accessor methods
std::string Logger::ElapsedRunTime()
{
  return Instance().ElapsedRunTimeImpl();
}

void Logger::QueueLogEvent(const LogEventData& pLog)
{
  Instance().QueueLogEventImpl(pLog);
}

void Logger::SetLogLevel(LogLevel level)
{
  Instance().SetLogLevelImpl(level);
}

void Logger::SetEnabled(bool enable)
{
  Instance().SetEnabledImpl(enable);
}

bool Logger::IsEnabled()
{
  return Instance().IsEnabledImpl();
}

void Logger::AttachHandler(LogEventHandlerPtr pHandler)
{
  Instance().AttachHandlerImpl(move(pHandler));
}

bool Logger::CheckLogAccess(LogLevel level)
{
  return Instance().CheckLogAccessImpl(level);
}
