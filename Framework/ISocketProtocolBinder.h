#pragma once

#include "IMessage.h"

#include <memory>

class ISocketProtocolBinder
{
private:

public:
  virtual ~ISocketProtocolBinder(void) {};
  virtual char* Send(const MessagePtr& msg, int& size) = 0;
  virtual const MessagePtr Deserialise(char *buffer, int& size) = 0;
};

typedef std::shared_ptr<ISocketProtocolBinder> SocketProtocolBinderPtr;
