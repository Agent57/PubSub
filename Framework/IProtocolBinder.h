#pragma once

#include <memory>

class IProtocolBinder
{
public:
  virtual ~IProtocolBinder(void) {};
};

typedef std::shared_ptr<IProtocolBinder> ProtocolBinderPtr;
