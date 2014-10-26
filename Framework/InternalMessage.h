#pragma once

#include "iHandler.h"
#include "iMessage.h"

class InternalMessage : virtual public IMessage
{
public:
  InternalMessage() : IMessage() {}

protected:
  template<class T>
  bool CallHandlerTemplate(const HandlerPtr& handler, const std::shared_ptr<T>& self) const
  {
    std::shared_ptr<MessageHandler<T>> pTypedHandler = std::dynamic_pointer_cast<MessageHandler<T>>(handler);
    if (pTypedHandler == nullptr)
      return false;

    return pTypedHandler->Process(self);
  }
};
