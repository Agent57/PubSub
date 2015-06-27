#include "TimedEventQueue.h"

TimedEventQueue::TimedEventQueue()
{
  m_timerId = 0;
}

TimedEventPtr TimedEventQueue::SetTimer(const int ms, const MessagePtr& msg, bool repeatable)
{
  TimedEventPtr timer = std::make_shared<TimedEvent>(ms, m_timerId++, msg, repeatable);

  std::lock_guard<std::mutex> lock(m_lock);
  InsertTimedEvent(timer);
  return timer;
}

MessagePtr TimedEventQueue::StopTimer(const int timerId)
{
  std::lock_guard<std::mutex> lock(m_lock);

  auto item = m_timedEvents.begin();
  while (item != m_timedEvents.end() && (*item)->Id() != timerId)
    ++item;

  if (item == m_timedEvents.end())
    return nullptr;

  MessagePtr event = (*item)->Event();
  m_timedEvents.erase(item);
  return event;
}

TimeoutPtr TimedEventQueue::Timeout()
{
  std::lock_guard<std::mutex> lock(m_lock);

  if (m_timedEvents.empty())
    return TimeoutFactory::CreateTimeout(0);

  auto timer = m_timedEvents.front();

  return timer->Timeout();
}

MessagePtr TimedEventQueue::TimeoutEvent()
{
  std::lock_guard<std::mutex> lock(m_lock);

  if (m_timedEvents.empty())
    return nullptr;

  auto timedEvent = m_timedEvents.front();
  m_timedEvents.pop_front();
  ResetIfRepeatable(timedEvent);

  return timedEvent->Event();
}

MessagePtr TimedEventQueue::FindTimerById(const int timerId)
{
  return {};
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
  auto it = m_timedEvents.begin();
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
