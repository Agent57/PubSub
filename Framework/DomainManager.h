#pragma once

#include "IBroker.h"
#include "IDomainManager.h"
#include "IMessageLoopRunner.h"


class DomainManager : public IDomainManager
{
private:
  BrokerPtr m_broker;
  ConnectorPtr m_inQueue;
  MessageLoopRunnerPtr m_messageLoopRunner;
  MessageLoopPtr m_messageLoop;
  MessageHandlerRegisterPtr m_handlers;

  DomainManager(){}
  DomainManager(DomainManager const& copy);

public:
  DomainManager(const BrokerPtr& broker, const ConnectorPtr& connector, const MessageLoopRunnerPtr& messageLoop);
  virtual ~DomainManager();
  
  bool Initialise() override;
  void RegisterHandler(const ::google::protobuf::Descriptor* type, const HandlerPtr& handler) override;
  void DeregisterHandler(const ::google::protobuf::Descriptor* type) override;
  bool Send(const MessagePtr& msg) override;
  TimedEventPtr SetTimer(int milliseconds, const MessagePtr& msg, bool repeatable = false) override;
  MessagePtr StopTimer(int TimerId) override;
};
