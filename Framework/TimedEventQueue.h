#pragma once

#include "IMessage.h"
#include "ITimeout.h"
#include "TimedEvent.h"

#include <list>
#include <mutex>

class TimedEventQueue
{
  typedef std::list<TimedEventPtr>::iterator TimedEventItem;

  int m_timerId;
  std::list<TimedEventPtr> m_timedEvents;
  std::mutex m_lock;

  void ResetIfRepeatable(const TimedEventPtr timedEvent);
  void InsertTimedEvent(const TimedEventPtr timedEvent);
  bool CheckTimeSlot(TimedEventItem& item, const TimedEventPtr timedEvent);

public:
  TimedEventQueue();
  virtual ~TimedEventQueue() {}
  virtual TimedEventPtr SetTimer(const int ms, const MessagePtr& msg, bool repeatable = false);
  virtual MessagePtr StopTimer(const int timerId);
  TimeoutPtr Timeout();
  MessagePtr TimeoutEvent();
  MessagePtr FindTimerById(const int timerId);
};
