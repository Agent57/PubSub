#include "DomainManager.h"
#include "DomainManagementHandlers.h"
#include "Logger.h"

#include <typeinfo>

// Constructor
DomainManager::DomainManager(const BrokerPtr& broker, const ConnectorPtr& connector, const MessageLoopRunnerPtr& messageLoopRunner)
{
  m_inQueue = connector;
  m_broker = broker;
  m_messageLoopRunner = messageLoopRunner;

  DomainManager::Initialise();
}

// Destructor
DomainManager::~DomainManager()
{
  m_messageLoopRunner->StopMessageLoop();

  auto handlers = m_messageLoop->GetHandlerMap();
  for(HandlerMapItem handler = handlers->begin(); handler != handlers->end(); ++handler)
  {
    m_broker->Unsubscribe(handler->first, m_inQueue);
  }
}

// Public Methods
bool DomainManager::Initialise()
{
  m_messageLoop = std::make_shared<MessageLoop>(m_inQueue);
  m_messageLoopRunner->RunMessageLoop(m_messageLoop);
  return true;
}

void DomainManager::RegisterHandler(const ::google::protobuf::Descriptor* type, const HandlerPtr& handler)
{
  m_messageLoop->RegisterHandler(type, handler);
  m_broker->Subscribe(type->full_name(), m_inQueue);
}

void DomainManager::DeregisterHandler(const ::google::protobuf::Descriptor* type)
{
  m_broker->Unsubscribe(type->full_name(), m_inQueue);
  m_messageLoop->DeregisterHandler(type);
}

bool DomainManager::Send(const MessagePtr& msg)
{
  return m_broker->Send(msg);
}

const TimedEventPtr DomainManager::SetTimer(const int milliseconds, const MessagePtr& msg, bool repeatable)
{
  return m_inQueue->SetTimer(milliseconds, msg, repeatable);
}

const MessagePtr DomainManager::StopTimer(const int TimerId)
{
  return m_inQueue->StopTimer(TimerId);
}