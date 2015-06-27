#pragma once

#include "IConnector.h"

namespace FrameworkSpecification
{
  class FakeConnector : public IConnector
  {
  public:
    int OpenCalled;
    int ShutdownCalled;
    int SendCalled;
    int ReadCalled;
    int SetTimerCalled;
    int StopTimerCalled;

    bool OpenExpectedResult;
    bool SendExpectedResult;
    MessagePtr ReadExpectedResult;
    TimedEventPtr SetTimerExpectedResult;
    MessagePtr StopTimerExpectedResult;

    MessagePtr SavedMessage;

    FakeConnector()
    {
      OpenCalled = 0;
      ShutdownCalled = 0;
      SendCalled = 0;
      ReadCalled = 0;
      SetTimerCalled = 0;
      StopTimerCalled = 0;

      OpenExpectedResult = true;
      SendExpectedResult = true;
      ReadExpectedResult = nullptr;
      SetTimerExpectedResult = nullptr;
      StopTimerExpectedResult = nullptr;

      SavedMessage = nullptr;
    }

    virtual bool Open(const ConnectionParametersPtr& connectionParameters) override
    {
      OpenCalled++;
      return OpenExpectedResult;
    }

    virtual void Shutdown(void) override
    {
      ShutdownCalled++;
    }

    virtual bool Send(const MessagePtr& msg) override
    {
      SendCalled++;
      SavedMessage = msg;
      return SendExpectedResult;
    }

    virtual MessagePtr Read(void) override
    {
      ReadCalled++;
      return ReadExpectedResult;
    }
    
    virtual TimedEventPtr SetTimer(const int milliseconds, const MessagePtr& msg, bool repeatable = false) override
    {
      SetTimerCalled++;
      SavedMessage = msg;
      return SetTimerExpectedResult;
    }
    
    virtual MessagePtr StopTimer(const int timerId) override
    {
      StopTimerCalled++;
      return StopTimerExpectedResult;
    }
  };

  typedef std::shared_ptr<FakeConnector> FakeConnectorPtr;
}
