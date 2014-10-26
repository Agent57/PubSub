#include "TimedEvent.h"
#include "ITimeout.h"
#include "Logger.h"

#include <chrono>

TimedEvent::TimedEvent(const int duration, const int timerId, const MessagePtr& event, bool repeat)
{
  auto tm = std::chrono::high_resolution_clock::now().time_since_epoch();
  m_duration = duration;
  m_expire = std::chrono::duration_cast<std::chrono::milliseconds>(tm).count() + m_duration;
  m_timerId = timerId;
  m_event = event;
  m_repeat = repeat;
}

TimeoutPtr TimedEvent::Timeout() const
{
  auto tm = std::chrono::high_resolution_clock::now().time_since_epoch();

  long long remaining = m_expire - std::chrono::duration_cast<std::chrono::milliseconds>(tm).count();

  return TimeoutFactory::CreateTimeout(remaining);
}

const int TimedEvent::Id() const
{
  return m_timerId;
}

const MessagePtr TimedEvent::Event() const
{
  return m_event;
}

bool TimedEvent::Repeatable() const
{
  return m_repeat;
}

const long long TimedEvent::Duration() const
{
  return m_duration;
}

void TimedEvent::ExtendTimeout(const long long duration)
{
  m_expire += duration;
}
