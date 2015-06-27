#include "IMessageLoopRunner.h"

#include <thread>

class MessageLoopRunner : public IMessageLoopRunner
{
  MessageLoopPtr m_messageLoop;
  std::thread m_thread;

public:
  ~MessageLoopRunner();
  virtual void RunMessageLoop(const MessageLoopPtr& messageLoop) override;
};

