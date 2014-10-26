#pragma once

#include "CppUnitTest.h"
#include "Broker.h"

namespace FrameworkSpecification
{
  class FakeBroker : public IBroker
  {
  public:
    int SubscribeCalled;
    int UnsubscribeCalled;
    int SendCalled;

    bool SendExpectedResult;

    std::string SavedType;
    ConnectorPtr SavedConnector;
    MessagePtr SavedMessage;

    FakeBroker()
    {
      SubscribeCalled = 0;
      UnsubscribeCalled = 0;
      SendCalled = 0;

      SendExpectedResult = false;

      SavedType = "";
      SavedConnector = nullptr;
      SavedMessage = nullptr;
    }

    void Subscribe(const std::string& type, const ConnectorPtr& connector)
    {
      SavedType = type;
      SavedConnector = connector;
      SubscribeCalled++;
    }

    void Unsubscribe(const std::string& type, const ConnectorPtr& connector)
    {
      SavedType = type;
      SavedConnector = connector;
      UnsubscribeCalled++;
    }

    bool Send(const MessagePtr& msg)
    {
      SendCalled++;
      SavedMessage = msg;
      return SendExpectedResult;
    }
  };

  typedef std::shared_ptr<FakeBroker> FakeBrokerPtr;
}