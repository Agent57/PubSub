#pragma warning (disable:4251)
#pragma once

#include <memory>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>

#include "IConnector.h"
#include "IMessage.h"
#include "TimedEventQueue.h"

class QueueConnector :
  public IConnector
{
private:
  std::queue<MessagePtr> m_queue;
  std::mutex m_lock;
  std::condition_variable m_conditional;
  std::atomic_bool m_notified;
  TimedEventQueue m_timer;

public:
  QueueConnector(void);
  ~QueueConnector(void);

  bool Open(const ConnectionParametersPtr& connectionParameters);
  void Shutdown();
  bool Send(const MessagePtr& msg);
  const MessagePtr Read();
  const TimedEventPtr SetTimer(const int milliseconds, const MessagePtr& msg, bool repeatable = false);
  const MessagePtr StopTimer(const int timerId);
  const MessagePtr FirstQueuedMessage();

private:
  bool CheckForMessage();
};
