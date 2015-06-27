#include "DomainManager.h"

// Constructor
DomainManager::DomainManager(const BrokerPtr& broker, const ConnectorPtr& connector, const MessageLoopRunnerPtr& messageLoopRunner)
{
  m_inQueue = connector;
  m_broker = broker;
  m_messageLoopRunner = messageLoopRunner;
  m_handlers = std::make_shared<MessageHandlerRegister>();

  DomainManager::Initialise();
}

// Destructor
DomainManager::~DomainManager()
{
  m_broker->Unsubscribe(m_handlers->GetHandlerTypes(), m_inQueue);
}

// Public Methods
bool DomainManager::Initialise()
{
  m_messageLoop = std::make_shared<MessageLoop>(m_inQueue, m_handlers);
  m_messageLoopRunner->RunMessageLoop(m_messageLoop);
  return true;
}

void DomainManager::RegisterHandler(const ::google::protobuf::Descriptor* type, const HandlerPtr& handler)
{
  m_handlers->RegisterHandler(type, handler);
  m_broker->Subscribe(type->full_name(), m_inQueue);
}

void DomainManager::DeregisterHandler(const ::google::protobuf::Descriptor* type)
{
  m_broker->Unsubscribe(type->full_name(), m_inQueue);
  m_handlers->DeregisterHandler(type);
}

bool DomainManager::Send(const MessagePtr& msg)
{
  return m_broker->Send(msg);
}

TimedEventPtr DomainManager::SetTimer(int milliseconds, const MessagePtr& msg, bool repeatable)
{
  return m_inQueue->SetTimer(milliseconds, msg, repeatable);
}

MessagePtr DomainManager::StopTimer(int TimerId)
{
  return m_inQueue->StopTimer(TimerId);
}