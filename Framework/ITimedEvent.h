#pragma once

#include "IMessage.h"
#include "ITimeout.h"

#include <memory>

class ITimedEvent
{
public:
  virtual ~ITimedEvent() {}
  virtual TimeoutPtr Timeout() const = 0;
  virtual int Id() const = 0;
  virtual MessagePtr Event() const = 0;
  virtual bool Repeatable() const = 0;
  virtual long long Duration() const = 0;
  virtual void ExtendTimeout(long long duration) = 0;
};

typedef std::shared_ptr<ITimedEvent> TimedEventPtr;
