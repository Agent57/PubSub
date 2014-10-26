#pragma once

#include "IDomainManager.h"
#include "IHandler.h"

#include "MessageLoopRunner.h"

namespace FrameworkSpecification
{
  class FakeMessageLoopRunner : public MessageLoopRunner
  {
  public:
    int RunMessageLoopCalled;
    int StopMessageLoopCalled;
    MessageLoopPtr SavedMessageLoop;

    FakeMessageLoopRunner()
    {
      RunMessageLoopCalled = 0;
      StopMessageLoopCalled = 0;
      SavedMessageLoop = nullptr;
    }

    void RunMessageLoop(const MessageLoopPtr& messageLoop)
    {
      RunMessageLoopCalled++;
      SavedMessageLoop = messageLoop;
    }

    void StopMessageLoop()
    {
      StopMessageLoopCalled++;
    }
  };

  typedef std::shared_ptr<FakeMessageLoopRunner> FakeMessageLoopRunnerPtr;
}