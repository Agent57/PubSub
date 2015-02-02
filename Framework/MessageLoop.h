#pragma once

#include "IMessageLoop.h"
#include "MessageHandlerRegister.h"
#include "QueueConnector.h"


class MessageLoop : public IMessageLoop
{
private:
  std::atomic_bool m_running;
  ConnectorPtr m_inQueue;
  MessageHandlerRegisterPtr m_handlers;

  bool CallHandlerForMessage(const MessagePtr& pMsg);

public:
  MessageLoop(const ConnectorPtr& connector, MessageHandlerRegisterPtr handlers);
  virtual bool ProcessMessageLoop();
  virtual ~MessageLoop();
  virtual void Execute();
  virtual void Shutdown();
};
