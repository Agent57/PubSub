#pragma once

#include "IHandler.h"
#include "IMessage.h"


class IMessageLoop
{
public:
  virtual ~IMessageLoop(void) {};
  virtual bool ProcessMessageLoop() = 0;
  virtual void RegisterHandler(const ::google::protobuf::Descriptor* type, const HandlerPtr& handler) = 0;
  virtual void DeregisterHandler(const ::google::protobuf::Descriptor* type) = 0;
  virtual const HandlerMapPtr GetHandlerMap() = 0;
  virtual void Execute() = 0;
  virtual void Shutdown() = 0;
};
typedef std::shared_ptr<IMessageLoop> MessageLoopPtr;
