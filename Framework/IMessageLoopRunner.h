#pragma once

#include "MessageLoop.h"


class IMessageLoopRunner
{
public:
  virtual void RunMessageLoop(const MessageLoopPtr& messageLoop) = 0;
  virtual void StopMessageLoop() = 0;
};
typedef std::shared_ptr<IMessageLoopRunner> MessageLoopRunnerPtr;