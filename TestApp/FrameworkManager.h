#pragma once

#include <winsock2.h>

#include "domainmanager.h"

class FrameworkManager :
  public DomainManager, public std::enable_shared_from_this<FrameworkManager>
{
public:
  FrameworkManager (const BrokerPtr& broker, const ConnectorPtr& connector, const MessageLoopRunnerPtr& messageLoop)
    : DomainManager(broker, connector, messageLoop) {}

  virtual ~FrameworkManager ();

  bool Initialise();
};

class FrameworkManagerY :
  public DomainManager, public std::enable_shared_from_this<FrameworkManagerY>
{
public:
  FrameworkManagerY (const BrokerPtr& broker, const ConnectorPtr& connector, const MessageLoopRunnerPtr& messageLoop)
    : DomainManager(broker, connector, messageLoop) {}

  virtual ~FrameworkManagerY ();

  bool Initialise();
};

typedef std::shared_ptr<FrameworkManagerY> FrameworkManagerYPtr;
