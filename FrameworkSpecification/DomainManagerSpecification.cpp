#include "CppUnitTest.h"

#include "Fakes/FakeBroker.h"
#include "Fakes/FakeHandler.h"
#include "Fakes/FakeConnector.h"
#include "Fakes/FakeTimedEvent.h"
#include "DomainManager.h"
#include "Fakes/FakeMessageLoopRunner.h"
#include "UnitTestDefinitions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FrameworkSpecification
{
  TEST_CLASS(DomainManagerSpecification)
  {
  public:
    FakeBrokerPtr broker;
    FakeConnectorPtr connection;
    DomainManagerPtr domain;
    FakeHandlerPtr handler;
    FakeMessageLoopRunnerPtr MessageLoopRunner;

    void SetupSimpleDomain()
    {
      broker = std::make_shared<FakeBroker>();
      connection = std::make_shared<FakeConnector>();
      MessageLoopRunner = std::make_shared<FakeMessageLoopRunner>();

      domain = std::make_shared<DomainManager>(broker, connection, MessageLoopRunner);
    }

    void SetupSimpleDomainAndRegisterTestMessageHandler()
    {
      SetupSimpleDomain();

      handler = std::make_shared<FakeHandler>(domain);
      domain->RegisterHandler(Exchange::TestMessage::descriptor(), handler);
    }
    
    TEST_METHOD(Initialising_a_domaim_manager_will_start_the_message_loop_worker_thread)
    {
      SetupSimpleDomain();

      Assert::IsTrue(domain != nullptr);
      Assert::AreEqual(CalledOnce, MessageLoopRunner->RunMessageLoopCalled);
    }

    TEST_METHOD(Registering_a_message_handler_subscribes_the_domain_with_the_broker_to_receive_that_message_type)
    {
      SetupSimpleDomainAndRegisterTestMessageHandler();

      Assert::AreEqual(CalledOnce, broker->SubscribeCalled);
      Assert::IsTrue(connection == broker->SavedConnector);
      Assert::AreEqual(TestMessageType, broker->SavedType);
    }

    TEST_METHOD(Deregistering_a_message_handler_will_unsubscribe_the_domain_from_the_broker_to_no_longer_receive_that_message_type)
    {
      SetupSimpleDomainAndRegisterTestMessageHandler();

      domain->DeregisterHandler(Exchange::TestMessage::descriptor());

      Assert::AreEqual(CalledOnce, broker->UnsubscribeCalled);
      Assert::AreEqual(TestMessageType, broker->SavedType);
    }

    TEST_METHOD(Calling_Send_will_pass_the_test_message_object_on_to_the_broker)
    {
      SetupSimpleDomain();
      
      broker->SendExpectedResult = true;

      TestMessagePtr testMessage = std::make_shared<Exchange::TestMessage>();
      Assert::IsTrue(domain->Send(testMessage));

      TestMessagePtr result = std::dynamic_pointer_cast<Exchange::TestMessage>(broker->SavedMessage);
      Assert::AreEqual(CalledOnce, broker->SendCalled);
      Assert::IsTrue(testMessage == result);
    }

    TEST_METHOD(Calling_SetTimer_will_create_a_TimedEvent_on_the_broker_connection)
    {
      SetupSimpleDomain();

      FakeTimedEventPtr expected = std::make_shared<FakeTimedEvent>();
      connection->SetTimerExpectedResult = expected;

      TimedEventPtr result = domain->SetTimer(0, nullptr);

      Assert::IsTrue(expected == result);
      Assert::AreEqual(CalledOnce, connection->SetTimerCalled);
    }

    TEST_METHOD(Calling_StopTimer_will_return_the_TimedEvents_associated_MessagePtr_object)
    {
      SetupSimpleDomain();

      TestMessagePtr expected = std::make_shared<Exchange::TestMessage>();
      connection->StopTimerExpectedResult = expected;

      MessagePtr result = domain->StopTimer(0);

      Assert::IsTrue(result == expected);
      Assert::AreEqual(CalledOnce, connection->StopTimerCalled);
    }

    TEST_METHOD(When_a_domain_manager_receives_a_message_it_will_call_the_registered_handler)
    {
      SetupSimpleDomainAndRegisterTestMessageHandler();
      connection->ReadExpectedResult = std::make_shared<Exchange::TestMessage>();
      handler->ProcessExpectedResult = true;

      bool result = MessageLoopRunner->SavedMessageLoop->ProcessMessageLoop();

      Assert::AreEqual(true, result);
      Assert::AreEqual(CalledOnce, connection->ReadCalled);
      Assert::AreEqual(CalledOnce, handler->ProcessCalled);
      Assert::IsTrue(connection->ReadExpectedResult == handler->SavedMessage);
    }

    TEST_METHOD(When_a_domain_manager_receives_a_message_with_no_registered_handler_the_message_is_discarded_and_returns_false)
    {
      SetupSimpleDomain();
      connection->ReadExpectedResult = std::make_shared<Exchange::TestMessage>();

      bool result = MessageLoopRunner->SavedMessageLoop->ProcessMessageLoop();

      Assert::AreEqual(false, result);
      Assert::AreEqual(CalledOnce, connection->ReadCalled);
    }

    TEST_METHOD(When_a_domain_manager_receives_a_null_message_object_the_processing_attempt_returns_false)
    {
      SetupSimpleDomain();
      connection->ReadExpectedResult = nullptr;

      bool result = MessageLoopRunner->SavedMessageLoop->ProcessMessageLoop();

      Assert::AreEqual(false, result);
      Assert::AreEqual(CalledOnce, connection->ReadCalled);
    }

    TEST_METHOD(Destruction_of_a_DomainManager_unsubscribes_a_registered_handler)
    {
      SetupSimpleDomain();

      {
        auto testDummy = std::make_shared<DomainManager>(broker, connection, MessageLoopRunner);
        handler = std::make_shared<FakeHandler>(testDummy);
        testDummy->RegisterHandler(Exchange::TestMessage::descriptor(), handler);
        // Check things clean up when the testDummy domain manager goes out of scope...
      }

      Assert::AreEqual(CalledOnce, broker->UnsubscribeCalled);
    }
  };
}