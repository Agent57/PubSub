#include "DomainManagementHandlers.h"
#include "Logger.h"

bool DomainManagementHandlers::Process(const std::shared_ptr<Exchange::RemoteSubscription const>& pMsg)
{
  int subscriptionId = pMsg->subscriptionid();
  m_domain.lock()->SetTimer(1500, nullptr);

  std::ostringstream stringStream;
  stringStream << "Remote Subscription Id: " << subscriptionId;

  auto msg = std::make_shared<Exchange::StringParameter>();
  msg->set_data(stringStream.str());

  m_domain.lock()->Send(msg);

  return true;
}

bool DomainManagementHandlers::Process(const std::shared_ptr<Exchange::Wrapper const>& pMsg)
{
  //pMsg->set_messageid("Hello World");  // Won't compile due to pMsg being a const object
  return true;
}

bool StringParameterHandler::Process(const std::shared_ptr<Exchange::StringParameter const>& pMsg)
{
  static int x = 0;
  LogEvent(Debug, "Message content... [" << pMsg->data() << "]");
  auto msg = std::make_shared<Exchange::RemoteSubscription>();
  msg->set_subscriptionid(x++);

  if(pMsg->data() == "Hello World")
    m_domain.lock()->Send(msg);
  
  return true;
}

