#pragma once

#include <memory>


class IMessageLoop
{
public:
  virtual ~IMessageLoop(void) {};
  virtual bool ProcessMessageLoop() = 0;
  virtual void Execute() = 0;
  virtual void Shutdown() = 0;
};
typedef std::shared_ptr<IMessageLoop> MessageLoopPtr;
