#pragma once

#include "IBroker.h"
#include "IDomainManager.h"
#include "IMessageLoopRunner.h"
#include "QueueConnector.h"


class DomainManager : public IDomainManager
{
private:
  BrokerPtr m_broker;
  ConnectorPtr m_inQueue;
  MessageLoopRunnerPtr m_messageLoopRunner;
  MessageLoopPtr m_messageLoop;
  MessageHandlerRegisterPtr m_handlers;

  DomainManager();
  DomainManager(DomainManager const& copy);

public:
  DomainManager(const BrokerPtr& broker, const ConnectorPtr& connector, const MessageLoopRunnerPtr& messageLoop);
  virtual ~DomainManager();
  
  bool Initialise();
  void RegisterHandler(const ::google::protobuf::Descriptor* type, const HandlerPtr& handler);
  void DeregisterHandler(const ::google::protobuf::Descriptor* type);
  bool Send(const MessagePtr& msg);
  const TimedEventPtr SetTimer(const int milliseconds, const MessagePtr& msg, bool repeatable = false);
  const MessagePtr StopTimer(const int TimerId);
};
