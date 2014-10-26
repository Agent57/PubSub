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

    virtual bool Open(const ConnectionParametersPtr& connectionParameters)
    {
      OpenCalled++;
      return OpenExpectedResult;
    }

    virtual void Shutdown(void)
    {
      ShutdownCalled++;
    }

    virtual bool Send(const MessagePtr& msg)
    {
      SendCalled++;
      SavedMessage = msg;
      return SendExpectedResult;
    }

    virtual const MessagePtr Read(void)
    {
      ReadCalled++;
      return ReadExpectedResult;
    }
    
    virtual const TimedEventPtr SetTimer(const int milliseconds, const MessagePtr& msg, bool repeatable = false)
    {
      SetTimerCalled++;
      SavedMessage = msg;
      return SetTimerExpectedResult;
    }
    
    virtual const MessagePtr StopTimer(const int timerId)
    {
      StopTimerCalled++;
      return StopTimerExpectedResult;
    }
  };

  typedef std::shared_ptr<FakeConnector> FakeConnectorPtr;
}