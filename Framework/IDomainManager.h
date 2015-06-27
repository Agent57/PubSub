#pragma once

#include "IHandler.h"
#include "IMessage.h"
#include "ITimedEvent.h"

#include <memory>

class IDomainManager
{
public:
  virtual ~IDomainManager() {}
  virtual bool Initialise() = 0;
  virtual void RegisterHandler(const ::google::protobuf::Descriptor* type, const HandlerPtr& handler) = 0;
  virtual void DeregisterHandler(const ::google::protobuf::Descriptor* type) = 0;
  virtual bool Send(const MessagePtr& msg) = 0;
  virtual TimedEventPtr SetTimer(int milliseconds, const MessagePtr& msg, bool repeatable = false) = 0;
  virtual MessagePtr StopTimer(int TimerId) = 0;
};
typedef std::shared_ptr<IDomainManager> DomainManagerPtr;