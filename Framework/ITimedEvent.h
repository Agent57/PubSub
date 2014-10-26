#pragma once

#include "IMessage.h"
#include "ITimeout.h"

#include <memory>

class ITimedEvent
{
public:
  virtual TimeoutPtr Timeout() const = 0;
  virtual const int Id() const = 0;
  virtual const MessagePtr Event() const = 0;
  virtual bool Repeatable() const = 0;
  virtual const long long Duration() const = 0;
  virtual void ExtendTimeout(const long long duration) = 0;
};

typedef std::shared_ptr<ITimedEvent> TimedEventPtr;
