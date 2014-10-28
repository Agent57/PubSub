#pragma once

#include "LogEventData.h"
#include "ILogEventHandler.h"

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <queue>
#include <sstream>
#include <thread>

#define LogEvent(level, text) { \
  if (Logger::Singleton().CheckLogLevel(level)) \
  { \
    std::ostringstream ss; \
    ss << text; \
    LogEventDataPtr pLog = std::make_shared<LogEventData>(level, ss.str(), __FILE__, __FUNCTION__, __LINE__); \
    Logger::Singleton().LogStream(pLog); \
  } \
}

class Logger
{
  #define LoggerInfo(text) { \
    std::ostringstream ss; \
    ss << text; \
    LogEventDataPtr pLog = std::make_shared<LogEventData>(Info, ss.str(), __FILE__, __FUNCTION__, __LINE__); \
    Logger::Singleton().LogStream(pLog); \
  }

  typedef std::list<LogEventHandlerPtr> LogEventHandlerSet;
  typedef LogEventHandlerSet::iterator LogEventHandlerSetItem;
  typedef std::list<LogEventDataPtr> LogEventQueue;
  typedef std::shared_ptr<LogEventQueue> LogEventQueuePtr;

private:
  std::mutex m_lock;
  std::mutex m_handlerLock;
  std::condition_variable m_conditional;
  LogEventQueuePtr m_queue;
  LogEventQueuePtr m_buffer;
  std::chrono::high_resolution_clock::time_point m_start;
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

public:
  static Logger& Singleton();
  void SetLogLevel(const LogLevel level);
  const std::chrono::high_resolution_clock::time_point StartTime() const;
  bool CheckLogLevel(const LogLevel level) const;
  void Enabled(bool enable);
  bool Enabled() const;
  void LogStream(const LogEventDataPtr& pLog);
  void Shutdown();
  void AttachHandler(const LogEventHandlerPtr& pHandler);
  void DetachHandler(const LogEventHandlerPtr& pHandler);
};
