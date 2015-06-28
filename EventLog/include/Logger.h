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
    LogEventData pLog(level, Logger::RunTime(), ss.str(), __FILE__, __FUNCTION__, __LINE__); \
    Logger::Log(pLog); \
  } \
}

class Logger
{
  #define LoggerInfo(text) { \
    std::ostringstream ss; \
    ss << text; \
    LogEventData pLog (Info, Logger::RunTime(), ss.str(), __FILE__, __FUNCTION__, __LINE__); \
    Logger::Log(pLog); \
  }

  typedef std::list<LogEventHandlerPtr> LogEventHandlerSet;
  typedef LogEventHandlerSet::iterator LogEventHandlerSetItem;
  typedef std::list<LogEventDataPtr> LogEventQueue;
  typedef std::unique_ptr<LogEventQueue> LogEventQueuePtr;

  std::mutex m_lock;
  std::mutex m_handlerLock;
  std::condition_variable m_conditional;
  LogEventQueuePtr m_queue;
  LogEventQueuePtr m_buffer;
  static std::chrono::high_resolution_clock::time_point m_start;
  std::atomic_bool m_enabled;
  std::atomic_bool m_running;
  std::atomic<LogLevel> m_max_level;
  std::thread m_logger;
  std::list<LogEventHandlerPtr> m_handlers;

  Logger();
  Logger(Logger const& copy);
  Logger& operator= (Logger const& copy);

  ~Logger();

  void LogWorker();
  void CallLogHandlers(const LogEventDataPtr& pLog);
  void ProcessLogEvents(const LogEventQueuePtr& events);
  void BufferEventQueue();
  void QueueLogEvent(const LogEventData& pLog);
  bool CheckLogLevelEnabled(const LogLevel level);

public:
  static Logger& Singleton();

  static void Log(const LogEventData& pLog);
  static bool CheckLogAccess(LogLevel level);
  static std::string RunTime();

  void Start();
  void SetLogLevel(const LogLevel level);

  void Enabled(bool enable);
  bool Enabled() const;
  void Shutdown();
  void AttachHandler(const LogEventHandlerPtr& pHandler);
  void DetachHandler(const LogEventHandlerPtr& pHandler);
};
