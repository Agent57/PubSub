#pragma warning (disable:4251)
#pragma once

#include "IMessage.h"
#include "ITimeout.h"
#include "TimedEvent.h"

#include <list>
#include <mutex>

class TimedEventQueue
{
private:
  int m_timerId;
  std::list<TimedEventPtr> m_timedEvents;
  std::mutex m_lock;

  void ResetIfRepeatable(const TimedEventPtr timedEvent);
  void InsertTimedEvent(const TimedEventPtr timedEvent);
  bool CheckTimeSlot(std::list<TimedEventPtr>::iterator& it, const TimedEventPtr timedEvent);

public:
  TimedEventQueue();
  virtual const TimedEventPtr SetTimer(const int ms, const MessagePtr& msg, bool repeatable = false);
  virtual const MessagePtr StopTimer(const int timerId);
  const TimeoutPtr Timeout();
  const MessagePtr TimeoutEvent();
  const MessagePtr FindTimerById(const int timerId);
};
