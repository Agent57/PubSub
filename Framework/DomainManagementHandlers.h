#pragma once

#include "IHandler.h"
#include "IDomainManager.h"

#include "RemoteSubscription.pb.h"
#include "StringParameter.pb.h"
#include "wrapper.pb.h"

// Define a handler class for a single message type
class StringParameterHandler
  : public MessageHandler<Exchange::StringParameter>
{
  const std::weak_ptr<IDomainManager> m_domain;

public:
  StringParameterHandler(const DomainManagerPtr& domain)
    : m_domain(domain) {}

  bool Process(const  std::shared_ptr<Exchange::StringParameter const>& pMsg) override;
};


// Define multiple handlers in a single class
class DomainManagementHandlers
  : public MessageHandler<Exchange::RemoteSubscription>,
  public MessageHandler<Exchange::Wrapper>
{
  const std::weak_ptr<IDomainManager> m_domain;

public:
  DomainManagementHandlers(const DomainManagerPtr& domain)
    : m_domain(domain) {}

  bool Process(const  std::shared_ptr<Exchange::RemoteSubscription const>& pMsg) override;
  bool Process(const  std::shared_ptr<Exchange::Wrapper const>& pMsg) override;
};
