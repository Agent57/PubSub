#include "TimedEvent.h"
#include "ITimeout.h"

#include <chrono>

TimedEvent::TimedEvent(int duration, int timerId, const MessagePtr& event, bool repeat)
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
  auto remaining = m_expire - std::chrono::duration_cast<std::chrono::milliseconds>(tm).count();

  return TimeoutFactory::CreateTimeout(remaining);
}

int TimedEvent::Id() const
{
  return m_timerId;
}

MessagePtr TimedEvent::Event() const
{
  return m_event;
}

bool TimedEvent::Repeatable() const
{
  return m_repeat;
}

long long TimedEvent::Duration() const
{
  return m_duration;
}

void TimedEvent::ExtendTimeout(long long duration)
{
  m_expire += duration;
}
