#pragma warning (disable:4251)
#pragma once

#include "IMessage.h"
#include "ITimedEvent.h"

#include <memory>

class TimedEvent : public ITimedEvent
{
  long long m_duration;
  long long m_expire;
  int m_timerId;
  MessagePtr m_event;
  bool m_repeat;

  TimedEvent() {};

public:
  TimedEvent(int duration, int timerId, const MessagePtr& event, bool repeat = false);

  TimeoutPtr Timeout() const override;
  int Id() const override;
  MessagePtr Event() const override;
  bool Repeatable() const override;
  long long Duration() const override;
  void ExtendTimeout(long long duration) override;
};
