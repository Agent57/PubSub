#include <windows.h>

#include "Logger.h"
#include "IDELogger.h"

#include <chrono>
#include <iomanip>

Logger::Logger()
{
  m_queue = std::make_unique<LogEventQueue>();
  m_start = std::chrono::high_resolution_clock::now();
  m_enabled =  true;
  m_max_level = Info;
  m_running = false;
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

  LogEvent(Special, "Application logging complete");
  m_running = false;

  if (m_logger.joinable())
    m_logger.join();

  ProcessLogEvents(m_queue);
}

void Logger::Start()
{
  if (m_running)
    return;

  m_running = true;
  LogEvent(Special, "Application logging started");

  SetDebuggerLogging();

  m_logger = std::thread(&Logger::LogWorker, this);
}

void Logger::LogWorker()
{
  while (m_running)
  {
    if (std::cv_status::timeout == WaitForQueuedEvent())
      continue;

    auto buffer = BufferEventQueue();
    ProcessLogEvents(buffer);
  }
}

std::cv_status Logger::WaitForQueuedEvent()
{
  auto result = std::cv_status::no_timeout;
  if (m_queue->empty())
  {
    std::unique_lock<std::mutex> lock(m_lock);
    result = m_conditional.wait_for(lock, std::chrono::milliseconds(500));
  }

  return result;
}

Logger::LogEventQueuePtr Logger::BufferEventQueue()
{
  std::lock_guard<std::mutex> lock(m_lock);
  auto buffer = move(m_queue);
  m_queue = std::make_unique<LogEventQueue>();
  return buffer;
}

void Logger::ProcessLogEvents(const LogEventQueuePtr& events)
{
  while (!events->empty())
  {
    const auto& pLog = events->front();
    CallLogHandlers(*pLog);
    events->pop_front();
  }
}

void Logger::CallLogHandlers(const LogEventData& event)
{
  std::lock_guard<std::mutex> lock(m_handlerLock);
  for (auto& handler : m_handlers)
  {
    if (!handler->ValidLogOutput(event))
      continue;

    handler->SetLogOutput(event);
    handler->OutputLogEvent();
  }
}

void Logger::SetDebuggerLogging()
{
  if (IsDebuggerPresent())
  {
    auto handler = std::make_unique<LogEventHandler>();
    handler->SetLogWriter(std::make_unique<IDELogWriter>());
    handler->SetLogFormatter(std::make_unique<IDELogFormatter>());
    m_handlers.push_back(move(handler));
  }
}

// Private implementation of static accessors
std::string Logger::ElapsedTimeImpl()
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
  bool result;

  for (auto& handler : m_handlers)
  {
    result |= handler->SetLogOutputLevel(level);
  }

  if (result)
    LogEvent(Special, "Log output level set to " << LogEventData::Severity(level));
}

void Logger::SetEnabledImpl(bool enable)
{
    if (m_enabled == enable)
    return;

  m_enabled = enable;
  LogEvent(Special, "Logging output is now " << (m_enabled ? "enabled" : "disabled"));
}

bool Logger::IsEnabledImpl() const
{
  return m_enabled;
}

void Logger::AttachHandlerImpl(LogFormatterPtr formatter, LogWriterPtr writer)
{
  auto handler = std::make_unique<LogEventHandler>();
  handler->SetLogFormatter(move(formatter));
  handler->SetLogWriter(move(writer));
  std::lock_guard<std::mutex> lock(m_handlerLock);
  m_handlers.push_back(move(handler));
  Start();
}

bool Logger::CheckLogAccessImpl(LogLevel level) const
{
  return m_enabled && m_running;
}

// Static accessor methods
std::string Logger::ElapsedTime()
{
  return Instance().ElapsedTimeImpl();
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

void Logger::AttachHandler(LogFormatterPtr formatter, LogWriterPtr writer)
{
  Instance().AttachHandlerImpl(move(formatter), move(writer));
}

bool Logger::CheckLogAccess(LogLevel level)
{
  return Instance().CheckLogAccessImpl(level);
}
