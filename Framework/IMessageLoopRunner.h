#pragma once

#include "MessageLoop.h"


class IMessageLoopRunner
{
public:
  virtual ~IMessageLoopRunner() {}
  virtual void RunMessageLoop(const MessageLoopPtr& messageLoop) = 0;
};
typedef std::shared_ptr<IMessageLoopRunner> MessageLoopRunnerPtr;