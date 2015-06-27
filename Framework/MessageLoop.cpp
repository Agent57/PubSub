#include "MessageLoop.h"
#include "Logger.h"

MessageLoop::MessageLoop(const ConnectorPtr& connector, MessageHandlerRegisterPtr handlers)
{
  m_handlers = handlers;
  m_running = false;
  m_inQueue = connector;
}

MessageLoop::~MessageLoop()
{
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
  auto pMsg = m_inQueue->Read();

  return pMsg ? CallHandlerForMessage(pMsg) : false;
}

bool MessageLoop::CallHandlerForMessage(const MessagePtr& pMsg)
{
  LogEvent(Trace, "Calling handler for " << pMsg->GetTypeName());

  auto handler = m_handlers->GetHandlerForMessage(pMsg->GetTypeName());
  return pMsg->CallHandler(handler);
}
