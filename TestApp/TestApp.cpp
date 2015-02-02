// TestApp.cpp : Defines the entry point for the console application.
//

#include "logger.h"

#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/coded_stream.h>
#include "google/protobuf/io/zero_copy_stream_impl.h"

#include "RemoteSubscription.pb.h"
#include "StringParameter.pb.h"
#include "wrapper.pb.h"
#include "ProtobufWrapper.h"
#include "ProtoMessage.h"

#include "FrameworkManager.h"
#include "Broker.h"
#include "MessageLoopRunner.h"
#include "ConsoleLogger.h"

#include <iostream>
#include <thread>


int main(int argc, char** argv)
{
  Logger::Singleton().AttachHandler(std::make_shared<ConsoleLogger>());
  Logger::Singleton().Start();

  LogEvent(Info, "Application Running...");
  //Logger::Singleton().SetLogLevel(Debug);

  // Set up the handler for a RemoteSubscription message
  MessageBroker broker;
  auto  visitor = std::make_shared<RemoteSubscriptionVisitor>();
  broker.Subscribe(Exchange::RemoteSubscription::descriptor()->full_name(), visitor);

  // Create a message
  Exchange::RemoteSubscription msg;
  msg.set_subscriptionid(123);
  auto proto = std::make_shared<RemoteSubscriptionMessage>(msg);

  // Do something with the message
  broker.Publish(proto);


  // Initialise the application framework
  auto SystemBroker = std::make_shared<Broker>();
  auto SystemQueue = std::make_shared<QueueConnector>();
  auto DefaultMessageLoopRunner = std::make_shared<MessageLoopRunner>();
  auto SystemFramework = std::make_shared<FrameworkManager>(SystemBroker, SystemQueue, DefaultMessageLoopRunner);
  SystemFramework->Initialise();

  auto SystemQueueY = std::make_shared<QueueConnector>();
  auto DefaultMessageLoopRunnerY = std::make_shared<MessageLoopRunner>();
  auto SystemFrameworkY = std::make_shared<FrameworkManagerY>(SystemBroker, SystemQueueY, DefaultMessageLoopRunnerY);
  SystemFrameworkY->Initialise();

  auto TimeoutMsg = std::make_shared<Exchange::StringParameter>();
  TimeoutMsg->set_data("Hello World");
  SystemFrameworkY->SetTimer(2000, TimeoutMsg, true);

  // Pause the console waiting on user input
  LogEvent(Info, "Press <Enter> key to exit...");
  std::cin.ignore();
  LogEvent(Info, "Application Shutdown Complete");


  return 0;
}
