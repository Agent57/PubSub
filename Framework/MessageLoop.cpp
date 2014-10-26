#include "MessageLoop.h"
#include "Logger.h"

MessageLoop::MessageLoop(const ConnectorPtr& connector)
{
  m_handlers = std::make_shared<HandlerMap>();
  m_running = false;
  m_inQueue = connector;
}

MessageLoop::~MessageLoop()
{
}

void MessageLoop::RegisterHandler(const ::google::protobuf::Descriptor* type, const HandlerPtr& handler)
{
  std::lock_guard<std::mutex> lock(m_lock);
  (*m_handlers)[type->full_name()] = handler;
}

void MessageLoop::DeregisterHandler(const ::google::protobuf::Descriptor* type)
{
  std::lock_guard<std::mutex> lock(m_lock);
  m_handlers->erase(type->full_name());
}

void MessageLoop::Execute()
{
  if (m_running)
    return;

  m_running = true;

  while(m_running)
    ProcessMessageLoop();
}

void MessageLoop::Shutdown()
{
  m_running = false;
  m_inQueue->Shutdown();
}

bool MessageLoop::ProcessMessageLoop()
{
  const MessagePtr pMsg = m_inQueue->Read();

  return CallHandlerForMessage(pMsg);
}

bool MessageLoop::CallHandlerForMessage(const MessagePtr& pMsg)
{
  if (!pMsg)
    return false;

  HandlerPtr handler = (*m_handlers)[pMsg->GetTypeName()];
  if(!handler)
  {
    LogEvent(Error, "No handler registered for " << pMsg->GetTypeName());
    return false;
  }

  LogEvent(Trace, "Calling handler for " << pMsg->GetTypeName());
  return pMsg->CallHandler(handler);
}

