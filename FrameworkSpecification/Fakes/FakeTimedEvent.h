#pragma once
#include <ITimedEvent.h>

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

    virtual TimeoutPtr Timeout() const override
    {
      return TimeoutExpectedResult;
    }

    virtual int Id() const override
    {
      return IdExpectedResult;
    }

    virtual MessagePtr Event() const override
    {
      return EventExpectedResult;
    }

    virtual bool Repeatable() const override
    {
      return RepeatableExpectedResult;
    }

    virtual long long Duration() const override
    {
      return DurationExpectedResult;
    }

    virtual void ExtendTimeout(long long duration) override
    {
      ExtendTimeoutCalled++;
    }
  };

  typedef std::shared_ptr<FakeTimedEvent> FakeTimedEventPtr;
}