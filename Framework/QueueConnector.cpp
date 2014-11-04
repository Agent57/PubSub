#include "Logger.h"
#include "QueueConnector.h"
#include "TimedEventQueue.h"

QueueConnector::QueueConnector()
{
  m_notified = false;
}

QueueConnector::~QueueConnector()
{
  Shutdown();
}

bool QueueConnector::Open(const ConnectionParametersPtr& connectionParameters)
{
  return true;
}

const TimedEventPtr QueueConnector::SetTimer(const int milliseconds, const MessagePtr& msg, bool repeatable)
{
  TimedEventPtr timer = m_timer.SetTimer(milliseconds, msg, repeatable);
  m_conditional.notify_all();

  return timer;
}

const MessagePtr QueueConnector::StopTimer(const int timerId)
{
  MessagePtr pMsg = m_timer.StopTimer(timerId);
  m_conditional.notify_all();

  return pMsg;
}

void QueueConnector::Shutdown()
{
  m_conditional.notify_all();
}

bool QueueConnector::Send(const MessagePtr& msg)
{
  std::lock_guard<std::mutex> lock(m_lock);

  m_queue.push(msg);
  m_conditional.notify_all();
    
  return true;
}

const MessagePtr QueueConnector::Read()
{
  if(CheckForMessage())
    return FirstQueuedMessage();

  LogEvent(Trace, "QueueConnector::Read() timed out");
  return m_timer.TimeoutEvent();
}

bool QueueConnector::CheckForMessage()
{
  return m_queue.empty()
    ? m_timer.Timeout()->Wait(m_conditional, m_lock) != std::cv_status::timeout
    : true;
}

const MessagePtr QueueConnector::FirstQueuedMessage()
{
  if(m_queue.empty())
    return nullptr;

  std::lock_guard<std::mutex> lock(m_lock);
  MessagePtr pMsg = m_queue.front();
  m_queue.pop();
  LogEvent(Trace, pMsg->GetTypeName() << " message received");
  return pMsg;
}
