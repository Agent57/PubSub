#include "CppUnitTest.h"

#include "ProtobufWrapper.h"

#include "MessageDefinitions\TestMessage.pb.h"

#include <memory>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FrameworkSpecification
{
  typedef std::shared_ptr<Exchange::TestMessage> TestMessagePtr;

  TEST_CLASS(ProtobufWrapperSpecification)
  {
  public:
    
    TEST_METHOD(Serialising_a_valid_protobuf_message_returns_a_non_null_array)
    {
      ProtobufWrapper TestSubject;
      TestMessagePtr TestMessage = std::make_shared<Exchange::TestMessage>();
      TestMessage->set_number(3);
      DataBufferPtr buffer = TestSubject.Serialise(TestMessage);
      Assert::IsTrue(buffer != nullptr);
    }

    TEST_METHOD(Deserialising_the_byte_stream_of_a_valid_protobuf_message_returns_a_copy_of_the_original_message)
    {
      Exchange::TestMessage testMsg;

      // Serialise a valid protobuf message
      ProtobufWrapper TestSubject;
      TestMessagePtr TestMessage = std::make_shared<Exchange::TestMessage>();
      TestMessage->set_number(3);
      DataBufferPtr buffer = TestSubject.Serialise(TestMessage);

      // Deserialise the byte stream
      ProtobufMessagePtr DeserialisedMessage = TestSubject.Deserialise(buffer);
      Assert::IsTrue(nullptr != DeserialisedMessage);

      auto TestMessageOut = std::dynamic_pointer_cast<Exchange::TestMessage>(DeserialisedMessage);
      Assert::AreEqual(TestMessage->number(), TestMessageOut->number());
    }

    TEST_METHOD(Deserialising_the_byte_stream_of_an_invalid_protobuf_message_returns_nullptr)
    {
      Exchange::TestMessage testMsg;

      // Prepare an invalid protobuf message
      ProtobufWrapper TestSubject;
      DataBufferPtr buffer = std::make_shared<DataBuffer>(6);

      // Deserialise the byte stream
      ProtobufMessagePtr DeserialisedNullMessage = TestSubject.Deserialise(buffer);
      Assert::IsTrue(nullptr == DeserialisedNullMessage);
    }

  };
}
