#pragma once

#include "domainmanager.h"

class FrameworkManager :
  public DomainManager, public std::enable_shared_from_this<FrameworkManager>
{
public:
  FrameworkManager (const BrokerPtr& broker, const ConnectorPtr& connector, const MessageLoopRunnerPtr& messageLoop)
    : DomainManager(broker, connector, messageLoop) {}

  virtual ~FrameworkManager ();

  bool Initialise() override;
};

class FrameworkManagerY :
  public DomainManager, public std::enable_shared_from_this<FrameworkManagerY>
{
public:
  FrameworkManagerY (const BrokerPtr& broker, const ConnectorPtr& connector, const MessageLoopRunnerPtr& messageLoop)
    : DomainManager(broker, connector, messageLoop) {}

  virtual ~FrameworkManagerY ();

  bool Initialise() override;
};

typedef std::shared_ptr<FrameworkManagerY> FrameworkManagerYPtr;
