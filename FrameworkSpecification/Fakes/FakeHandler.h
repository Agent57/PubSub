#pragma once

#include "IDomainManager.h"
#include "IHandler.h"

#include "MessageDefinitions\TestMessage.pb.h"

namespace FrameworkSpecification
{
  class FakeHandler : public MessageHandler<Exchange::TestMessage>
  {
  public:
    int ProcessCalled;
    bool ProcessExpectedResult;

    std::weak_ptr<IDomainManager> SavedDomainManager;
    std::shared_ptr<Exchange::TestMessage const> SavedMessage;

    FakeHandler(const DomainManagerPtr& domain)
    {
      ProcessCalled = 0;
      ProcessExpectedResult = false;

      SavedDomainManager = domain;
      SavedMessage = nullptr;
    }

    bool Process(const  std::shared_ptr<Exchange::TestMessage const>& pMsg)
    {
      ProcessCalled++;
      SavedMessage = pMsg;
      return ProcessExpectedResult;
    }
  };

  typedef std::shared_ptr<FakeHandler> FakeHandlerPtr;
}