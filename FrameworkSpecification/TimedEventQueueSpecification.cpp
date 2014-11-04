#include "CppUnitTest.h"

#include "TimedEventQueue.h"
#include "MessageDefinitions\TestMessage.pb.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CoreSpecification
{
  typedef std::shared_ptr<Exchange::TestMessage> TestMessagePtr;

  TEST_CLASS(TimedEventQueueSpecification)
  {
  private:
    TimedEventQueue teq;
    TestMessagePtr TestMessage1;
    TestMessagePtr TestMessage2;
    TestMessagePtr TestMessage3;
    TestMessagePtr TestMessage4;

  public:
    // TimedEventQueue is a collection of TimedEvents, sorted into
    // chronological order for when the event timer is set to expire.
    TEST_METHOD_INITIALIZE(inti)
    {
      TestMessage1 = std::make_shared<Exchange::TestMessage>();
      TestMessage1->set_number(1);

      // Set an expired event second
      TestMessage2 = std::make_shared<Exchange::TestMessage>();
      TestMessage2->set_number(2);

      // Set an event to occur between the previous two
      TestMessage3 = std::make_shared<Exchange::TestMessage>();
      TestMessage3->set_number(3);

      // Set an event to occur between the previous two
      TestMessage4 = std::make_shared<Exchange::TestMessage>();
      TestMessage4->set_number(4);
    }

    TEST_METHOD(Setting_a_timer_returns_a_valid_TimedEventPtr)
    {
      TimedEventPtr pEvent = teq.SetTimer(0, nullptr);
      Assert::IsTrue(pEvent != nullptr);
    }

    TEST_METHOD(Setting_a_repeatable_timer_returns_a_valid_TimedEventPtr)
    {
      TimedEventPtr pEvent = teq.SetTimer(0, nullptr, true);
      Assert::IsTrue(pEvent != nullptr);
    }

    TEST_METHOD(Setting_a_timer_returns_a_TimedEventPtr_with_a_positive_id_value)
    {
      TimedEventPtr pEvent = teq.SetTimer(0, nullptr);
      Assert::IsTrue(pEvent->Id() >= 0);
    }

    TEST_METHOD(Stopping_a_timer_returns_the_stored_message_pointer_for_the_given_event)
    {
      TimedEventPtr pEvent = teq.SetTimer(0, TestMessage1);
      TestMessagePtr iMsgPtrOut = std::dynamic_pointer_cast<Exchange::TestMessage>(teq.StopTimer(pEvent->Id()));
      Assert::IsTrue(TestMessage1 == iMsgPtrOut);
    }

    TEST_METHOD(When_checking_the_timeout_on_an_expired_event_the_returned_expiration_time_is_minus_one)
    {
      // Set a repeatable event for 2147483647ms in the past
      teq.SetTimer(0x80000001 , nullptr, true);

      // Get the expiration time for the expired event
      const long long expires = teq.Timeout()->Remaining();
      Assert::IsTrue(expires == -1);
    }

    TEST_METHOD(Attempting_to_retrieve_a_timeout_event_from_an_empty_queue_returns_nullptr)
    {
      // Get an event from an empty queue
      auto iMsgPtrOut = std::dynamic_pointer_cast<Exchange::TestMessage>(teq.TimeoutEvent());
      Assert::IsTrue(iMsgPtrOut == nullptr);
    }

    TEST_METHOD(When_setting_multiple_timers_the_next_timeouts_expiration_value_will_be_the_lowest_one)
    {
      // Set the last event first
      teq.SetTimer(5000, nullptr);

      // Set an expired event second
      teq.SetTimer(-1, nullptr);

      // Set an event to occur between the previous two
      teq.SetTimer(2500, nullptr);

      // Get the expiration time for the event at the front of the queue
      const long long expires = teq.Timeout()->Remaining();
      Assert::IsTrue(expires == -1);
    }

    TEST_METHOD(When_setting_an_odd_number_of_timers_the_queued_events_are_retrieved_in_chronological_order)
    {
      // Set the last event first
      teq.SetTimer(5000, TestMessage3);

      // Set an expired event second
      teq.SetTimer(-1, TestMessage1);

      // Set an event to occur between the previous two
      teq.SetTimer(2500, TestMessage2);

      // Get the three events back in the correct order, 1-2-3
      auto iMsgPtrOut1 = std::dynamic_pointer_cast<Exchange::TestMessage>(teq.TimeoutEvent());
      Assert::IsTrue(iMsgPtrOut1->number() == 1);
      auto iMsgPtrOut2 = std::dynamic_pointer_cast<Exchange::TestMessage>(teq.TimeoutEvent());
      Assert::IsTrue(iMsgPtrOut2->number() == 2);
      auto iMsgPtrOut3 = std::dynamic_pointer_cast<Exchange::TestMessage>(teq.TimeoutEvent());
      Assert::IsTrue(iMsgPtrOut3->number() == 3);
    }

    TEST_METHOD(When_setting_an_even_number_of_timers_the_queued_events_are_retrieved_in_chronological_order)
    {
      // Set the second event first
      teq.SetTimer(2500, TestMessage2);

      // Set event 3
      teq.SetTimer(5000, TestMessage3);

      // Set an expired event
      teq.SetTimer(-1, TestMessage1);

      // Set the last event to be at the back of the queue
      teq.SetTimer(10000, TestMessage4);

      // Get the four events back in the correct order, 1-2-3-4
      auto iMsgPtrOut1 = std::dynamic_pointer_cast<Exchange::TestMessage>(teq.TimeoutEvent());
      Assert::IsTrue(iMsgPtrOut1->number() == 1);
      auto iMsgPtrOut2 = std::dynamic_pointer_cast<Exchange::TestMessage>(teq.TimeoutEvent());
      Assert::IsTrue(iMsgPtrOut2->number() == 2);
      auto iMsgPtrOut3 = std::dynamic_pointer_cast<Exchange::TestMessage>(teq.TimeoutEvent());
      Assert::IsTrue(iMsgPtrOut3->number() == 3);
      auto iMsgPtrOut4 = std::dynamic_pointer_cast<Exchange::TestMessage>(teq.TimeoutEvent());
      Assert::IsTrue(iMsgPtrOut4->number() == 4);
    }

    TEST_METHOD(When_stopping_a_timer_only_the_event_with_the_given_id_is_cancelled)
    {
      // Set the last event first
      teq.SetTimer(5000, TestMessage3);

      // Set an expired event second
      teq.SetTimer(-1, TestMessage1);

      // Set an event to occur between the previous two then cancel it
      TimedEventPtr pEvent = teq.SetTimer(2500, TestMessage2);
      teq.StopTimer(pEvent->Id());

      // Get the two remaining events back in the correct order, 1-3
      auto iMsgPtrOut1 = std::dynamic_pointer_cast<Exchange::TestMessage>(teq.TimeoutEvent());
      Assert::IsTrue(iMsgPtrOut1->number() == 1);
      auto iMsgPtrOut2 = std::dynamic_pointer_cast<Exchange::TestMessage>(teq.TimeoutEvent());
      Assert::IsTrue(iMsgPtrOut2->number() == 3);
    }

    TEST_METHOD(Retrieving_a_repeatable_timed_event_will_cause_the_events_timer_to_be_reset)
    {
      // Set a repeatable event
      const TimedEventPtr event = teq.SetTimer(5000, nullptr, true);

      // Pop the repeatable event off the queue
      teq.TimeoutEvent();

      // Check the timeout value was extended to now be between 5 and 10 seconds
      long long expires = teq.Timeout()->Remaining();
      Assert::IsTrue(expires > 5000 && expires <= 10000);
    }

    TEST_METHOD(Retrieving_a_repeatable_timed_event_will_requeue_the_event_after_any_others_with_a_shorter_timeout)
    {
      // Set a repeatable event, closely followed by a second
      teq.SetTimer(4000, nullptr, true);
      teq.SetTimer(5000, nullptr);

      // Pop the repeatable event off the queue
      teq.TimeoutEvent();

      // Check the timeout value was recalculated to now be between 4 and 5 seconds
      long long expires = teq.Timeout()->Remaining();
      Assert::IsTrue(expires > 4000 && expires <= 5000);

      // Pop the non repeatable event off the queue
      teq.TimeoutEvent();

      // Check the timeout value was extended to now be between 5 and 10 seconds
      expires = teq.Timeout()->Remaining();
      Assert::IsTrue(expires > 5000 && expires <= 10000);
    }
  };
}
