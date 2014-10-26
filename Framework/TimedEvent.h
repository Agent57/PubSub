#pragma warning (disable:4251)
#pragma once

#include "IMessage.h"
#include "ITimedEvent.h"

#include <memory>

class TimedEvent : public ITimedEvent
{
private:
  long long m_duration;
  long long m_expire;
  int m_timerId;
  MessagePtr m_event;
  bool m_repeat;

  TimedEvent() {};

public:
  TimedEvent(const int duration, const int timerId, const MessagePtr& event, bool repeat = false);

  TimeoutPtr Timeout() const;
  const int Id() const;
  const MessagePtr Event() const;
  bool Repeatable() const;
  const long long Duration() const;
  void ExtendTimeout(const long long duration);
};
