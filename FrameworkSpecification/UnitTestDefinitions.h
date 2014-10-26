#pragma once

#include <memory>
#include <string>

#include "MessageDefinitions\TestMessage.pb.h"

namespace FrameworkSpecification
{
  static const std::string TestMessageType("Exchange.TestMessage");

  static const int NotCalled = 0;
  static const int CalledOnce = 1;

  typedef std::shared_ptr<Exchange::TestMessage> TestMessagePtr;

}