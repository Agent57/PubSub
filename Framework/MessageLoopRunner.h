#include "IMessageLoopRunner.h"

#include <thread>

class MessageLoopRunner : public IMessageLoopRunner
{
private:
  MessageLoopPtr m_messageLoop;
  std::thread m_thread;

public:
  virtual void RunMessageLoop(const MessageLoopPtr& messageLoop);
  virtual void StopMessageLoop();

};

