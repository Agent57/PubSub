#pragma once

#include "TimedEvent.h"

namespace FrameworkSpecification
{
  class FakeTimedEvent : public ITimedEvent
  {
  public:
    int ExtendTimeoutCalled;

    TimeoutPtr TimeoutExpectedResult;
    int IdExpectedResult;
    MessagePtr EventExpectedResult;
    bool RepeatableExpectedResult;
    long long DurationExpectedResult;

    FakeTimedEvent()
    {
      ExtendTimeoutCalled = 0;

      TimeoutExpectedResult = nullptr;
      IdExpectedResult = 0;
      EventExpectedResult = nullptr;
      RepeatableExpectedResult = false;
      DurationExpectedResult = 0L;
    }

    virtual TimeoutPtr Timeout() const
    {
      return TimeoutExpectedResult;
    }

    virtual const int Id() const
    {
      return IdExpectedResult;
    }

    virtual const MessagePtr Event() const
    {
      return EventExpectedResult;
    }

    virtual bool Repeatable() const
    {
      return RepeatableExpectedResult;
    }

    virtual const long long Duration() const
    {
      return DurationExpectedResult;
    }

    virtual void ExtendTimeout(const long long duration)
    {
      ExtendTimeoutCalled++;
    }
  };

  typedef std::shared_ptr<FakeTimedEvent> FakeTimedEventPtr;
}