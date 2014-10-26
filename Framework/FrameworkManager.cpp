#include "FrameworkManager.h"
#include "DomainManagementHandlers.h"

FrameworkManager::~FrameworkManager()
{
  DeregisterHandler(Exchange::RemoteSubscription::descriptor());
  DeregisterHandler(Exchange::Wrapper::descriptor());
}

bool FrameworkManager::Initialise()
{
  // Register handler class supporting multiple message types
  auto dmh = std::make_shared<DomainManagementHandlers>(shared_from_this());
  RegisterHandler(Exchange::RemoteSubscription::descriptor(), dmh);
  RegisterHandler(Exchange::Wrapper::descriptor(), dmh);

  return true;
}

FrameworkManagerY::~FrameworkManagerY()
{
  DeregisterHandler(Exchange::StringParameter::descriptor());
}

bool FrameworkManagerY::Initialise()
{
  // Register handler class for single message type
  auto sph = std::make_shared<StringParameterHandler>(shared_from_this());
  RegisterHandler(Exchange::StringParameter::descriptor(), sph);

  return true;
}
