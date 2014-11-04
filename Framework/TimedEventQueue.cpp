#include "TimedEventQueue.h"
#include "Logger.h"

#include <chrono>

TimedEventQueue::TimedEventQueue()
{
  m_timerId = 0;
}

const TimedEventPtr TimedEventQueue::SetTimer(const int ms, const MessagePtr& msg, bool repeatable)
{
  TimedEventPtr timer = std::make_shared<TimedEvent>(ms, m_timerId++, msg, repeatable);

  std::lock_guard<std::mutex> lock(m_lock);
  InsertTimedEvent(timer);
  return timer;
}

const MessagePtr TimedEventQueue::StopTimer(const int timerId)
{
  std::lock_guard<std::mutex> lock(m_lock);

  std::list<TimedEventPtr>::iterator it = m_timedEvents.begin();
  while (it != m_timedEvents.end() && (*it)->Id() != timerId)
    ++it;

  if (it == m_timedEvents.end())
    return nullptr;

  MessagePtr event = (*it)->Event();
  m_timedEvents.erase(it);
  return event;
}

const TimeoutPtr TimedEventQueue::Timeout()
{
  std::lock_guard<std::mutex> lock(m_lock);

  if (m_timedEvents.empty())
    return TimeoutFactory::CreateTimeout(0);

  TimedEventPtr timer = m_timedEvents.front();

  return timer->Timeout();
}

const MessagePtr TimedEventQueue::TimeoutEvent()
{
  std::lock_guard<std::mutex> lock(m_lock);

  if (m_timedEvents.empty())
    return nullptr;

  TimedEventPtr timedEvent = m_timedEvents.front();
  m_timedEvents.pop_front();
  ResetIfRepeatable(timedEvent);

  return timedEvent->Event();
}


// Private Methods
void TimedEventQueue::ResetIfRepeatable(const TimedEventPtr timedEvent)
{
  if (timedEvent->Repeatable())
  {
    timedEvent->ExtendTimeout(timedEvent->Duration());
    InsertTimedEvent(timedEvent);
  }
}

void TimedEventQueue::InsertTimedEvent(const TimedEventPtr timedEvent)
{
  TimedEventItem it = m_timedEvents.begin();
  while (false == CheckTimeSlot(it, timedEvent))
    ++it;

  m_timedEvents.insert(it, timedEvent);
}

bool TimedEventQueue::CheckTimeSlot(TimedEventItem& it, const TimedEventPtr timedEvent)
{
  if (it == m_timedEvents.end())
    return true;

  return (*it)->Timeout()->Remaining() > timedEvent->Timeout()->Remaining();
}
