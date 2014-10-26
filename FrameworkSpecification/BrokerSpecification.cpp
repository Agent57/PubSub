#include "CppUnitTest.h"

#include "Broker.h"
#include "Fakes/FakeConnector.h"
#include "UnitTestDefinitions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FrameworkSpecification
{
  typedef std::shared_ptr<Exchange::TestMessage> TestMessagePtr;

  TEST_CLASS(BrokerSpecification)
  {
  public:
    BrokerPtr broker;
    FakeConnectorPtr connection;
    TestMessagePtr testMessage;

    void Setup()
    {
      connection = std::make_shared<FakeConnector>();
      broker = std::make_shared<Broker>();
      testMessage = std::make_shared<Exchange::TestMessage>();
    }

    TEST_METHOD(Subscribing_a_connector_to_a_message_type_allows_it_to_receive_a_message_of_that_same_type)
    {
      Setup();
      broker->Subscribe(testMessage->GetTypeName(), connection);

      Assert::IsTrue(broker->Send(testMessage));

      Assert::AreEqual(CalledOnce, connection->SendCalled);
      Assert::IsTrue(testMessage == connection->SavedMessage);
    }

    TEST_METHOD(Sending_to_no_subscribers_returns_false_and_no_message_is_sent)
    {
      Setup();
      broker->Subscribe("Qwerty", connection);

      Assert::IsFalse(broker->Send(testMessage));

      Assert::AreEqual(NotCalled, connection->SendCalled);
      Assert::IsTrue(nullptr == connection->SavedMessage);
    }

    TEST_METHOD(We_dont_get_messages_we_unsubscribed_from)
    {
      Setup();
      broker->Subscribe(testMessage->GetTypeName(), connection);
      broker->Unsubscribe(testMessage->GetTypeName(), connection);

      Assert::IsTrue(broker->Send(testMessage));

      Assert::AreEqual(NotCalled, connection->SendCalled);
      Assert::IsTrue(nullptr == connection->SavedMessage);
    }

    TEST_METHOD(Subscribing_twice_will_still_only_give_one_queued_message)
    {
      Setup();
      broker->Subscribe(testMessage->GetTypeName(), connection);
      broker->Subscribe(testMessage->GetTypeName(), connection);

      Assert::IsTrue(broker->Send(testMessage));

      Assert::AreEqual(CalledOnce, connection->SendCalled);
      Assert::IsTrue(testMessage == connection->SavedMessage);
    }

    TEST_METHOD(Unsubscribing_an_unknown_message_type_does_not_cause_an_exception)
    {
      Setup();
      broker->Subscribe(testMessage->GetTypeName(), connection);
      broker->Unsubscribe("Qwerty", connection);

      Assert::IsTrue(broker->Send(testMessage));

      Assert::AreEqual(CalledOnce, connection->SendCalled);
      Assert::IsTrue(testMessage == connection->SavedMessage);
    }

    TEST_METHOD(subscribing_to_2_message_types_does_not_cause_an_exception)
    {
      Setup();
      broker->Subscribe("Qwerty", connection);
      broker->Subscribe(testMessage->GetTypeName(), connection);

      Assert::IsTrue(broker->Send(testMessage));

      Assert::AreEqual(CalledOnce, connection->SendCalled);
      Assert::IsTrue(testMessage == connection->SavedMessage);
    }

    TEST_METHOD(subscribing_to_1_message_type_on_2_connecions_gives_1_message_per_connection_send)
    {
      Setup();
      FakeConnectorPtr secondConnection = std::make_shared<FakeConnector>();
      broker->Subscribe(testMessage->GetTypeName(), connection);
      broker->Subscribe(testMessage->GetTypeName(), secondConnection);

      Assert::IsTrue(broker->Send(testMessage));

      Assert::AreEqual(CalledOnce, connection->SendCalled);
      Assert::IsTrue(testMessage == connection->SavedMessage);

      Assert::AreEqual(CalledOnce, secondConnection->SendCalled);
      Assert::IsTrue(testMessage == secondConnection->SavedMessage);
    }

    TEST_METHOD(subscribing_2_connecions_for_different_types_gives_1_message_for_the_correct_subscription)
    {
      Setup();
      FakeConnectorPtr secondConnection = std::make_shared<FakeConnector>();
      broker->Subscribe(testMessage->GetTypeName(), connection);
      broker->Subscribe("qwerty", secondConnection);

      Assert::IsTrue(broker->Send(testMessage));

      Assert::AreEqual(CalledOnce, connection->SendCalled);
      Assert::IsTrue(testMessage == connection->SavedMessage);

      Assert::AreEqual(NotCalled, secondConnection->SendCalled);
      Assert::IsTrue(nullptr == secondConnection->SavedMessage);
    }

    TEST_METHOD(The_return_value_on_send_correctly_reports_a_failure)
    {
      Setup();
      FakeConnectorPtr secondConnection = std::make_shared<FakeConnector>();
      connection->SendExpectedResult = false;

      broker->Subscribe(testMessage->GetTypeName(), connection);
      broker->Subscribe(testMessage->GetTypeName(), secondConnection);

      Assert::IsFalse(broker->Send(testMessage));
    }
 };
}