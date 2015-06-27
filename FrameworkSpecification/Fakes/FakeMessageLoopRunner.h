#pragma once

#include "IMessageLoopRunner.h"

namespace FrameworkSpecification
{
  class FakeMessageLoopRunner : public IMessageLoopRunner
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

    void RunMessageLoop(const MessageLoopPtr& messageLoop) override
    {
      RunMessageLoopCalled++;
      SavedMessageLoop = messageLoop;
    }
  };

  typedef std::shared_ptr<FakeMessageLoopRunner> FakeMessageLoopRunnerPtr;
}