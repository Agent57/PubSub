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
  virtual ~MessageLoop();
  virtual bool ProcessMessageLoop() override;
  virtual void Execute() override;
  virtual void Shutdown() override;
};
