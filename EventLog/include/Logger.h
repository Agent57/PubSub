#pragma once

#include "LogEventData.h"
#include "ILogEventHandler.h"

#include <atomic>
#include <condition_variable>
#include <list>
#include <memory>
#include <mutex>
#include <thread>
#include <sstream>

#define LogEvent(level, text) { \
  if (Logger::CheckLogAccess(level)) \
  { \
    std::ostringstream ss; \
    ss << text; \
    LogEventData pLog(level, Logger::ElapsedTime(), ss.str(), __FILE__, __FUNCTION__, __LINE__); \
    Logger::QueueLogEvent(pLog); \
  } \
}

class Logger
{
  typedef std::list<LogEventDataPtr> LogEventQueue;
  typedef std::unique_ptr<LogEventQueue> LogEventQueuePtr;

  std::mutex m_lock;
  std::mutex m_handlerLock;
  std::condition_variable m_conditional;
  LogEventQueuePtr m_queue;
  std::chrono::high_resolution_clock::time_point m_start;
  std::atomic_bool m_enabled;
  std::atomic_bool m_running;
  std::atomic<LogLevel> m_max_level;
  std::thread m_logger;
  std::list<LogEventHandlerPtr> m_handlers;

  Logger();
  Logger(Logger const& copy);
  Logger& operator= (Logger const& copy);
  static Logger& Instance();
  ~Logger();

  void Start();
  void LogWorker();
  std::cv_status WaitForQueuedEvent();
  LogEventQueuePtr BufferEventQueue();
  void ProcessLogEvents(const LogEventQueuePtr& events);
  void CallLogHandlers(const LogEventData& event);

  std::string ElapsedTimeImpl();
  void QueueLogEventImpl(const LogEventData& pLog);
  void SetLogLevelImpl(LogLevel level);
  void SetEnabledImpl(bool enable);
  bool IsEnabledImpl() const;
  void AttachHandlerImpl(LogEventHandlerPtr pHandler, LogWriterPtr writer);
  bool CheckLogAccessImpl(const LogLevel level) const;

public:
  static std::string ElapsedTime();
  static void QueueLogEvent(const LogEventData& pLog);
  static void SetLogLevel(const LogLevel level);
  static void SetEnabled(bool enable);
  static bool IsEnabled();
  static void AttachHandler(LogEventHandlerPtr handler, LogWriterPtr writer);
  static bool CheckLogAccess(LogLevel level);
};
