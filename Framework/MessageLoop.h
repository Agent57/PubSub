#pragma once

#include "IMessageLoop.h"
#include "QueueConnector.h"

#include <mutex>


class MessageLoop : public IMessageLoop
{
private:
  std::mutex m_lock;
  std::atomic_bool m_running;
  ConnectorPtr m_inQueue;
  HandlerMapPtr m_handlers;

  bool CallHandlerForMessage(const MessagePtr& pMsg);

public:
  MessageLoop(const ConnectorPtr& connector);
  virtual bool ProcessMessageLoop();
  virtual ~MessageLoop();
  virtual void RegisterHandler(const ::google::protobuf::Descriptor* type, const HandlerPtr& handler);
  virtual void DeregisterHandler(const ::google::protobuf::Descriptor* type);
  virtual void Execute();
  virtual void Shutdown();
  virtual const HandlerMapPtr GetHandlerMap() { return m_handlers; }
};

