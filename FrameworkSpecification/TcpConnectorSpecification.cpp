#include "CppUnitTest.h"

#include "TcpConnector.h"
#include "ProtobufWrapper.h"

#include "MessageDefinitions\TestMessage.pb.h"

#include <memory>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FrameworkSpecification
{
  typedef std::shared_ptr<Exchange::TestMessage> TestMessagePtr;

  TEST_CLASS(TcpConnectorSpecification)
  {
  public:
    
    TEST_METHOD(TestMethod1)
    {
      //// Create one TCP socket connector
      //TcpConnector TestSubject;

      //// Throw in some connection parameters
      //TcpConnectionParametersPtr TestParams = std::make_shared<TcpConnectionParameters>();

      //// Add a serialiser to the params
      //TestParams->set_serialiser(std::make_shared<ProtobufWrapper>());

      //// Open our connection
      //TestSubject.Open(TestParams);

      //// Create a test message to send
      //TestMessagePtr TestMessage = std::make_shared<Exchange::TestMessage>();

      //// Put something 'meaningful' into our test message
      //TestMessage->set_number(3);
  
      //// Send the message
      //bool TestResult = TestSubject.Send(TestMessage);

      //// Check the result of our test is as expected
      //Assert::IsTrue(TestResult);
    }

  };
}