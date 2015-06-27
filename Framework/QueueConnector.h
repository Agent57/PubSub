#pragma warning (disable:4251)
#pragma once

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
  std::queue<MessagePtr> m_queue;
  std::mutex m_lock;
  std::condition_variable m_conditional;
  std::atomic_bool m_notified;
  TimedEventQueue m_timer;

public:
  QueueConnector(void);
  ~QueueConnector(void);

  bool Open(const ConnectionParametersPtr& connectionParameters) override;
  void Shutdown() override;
  bool Send(const MessagePtr& msg) override;
  MessagePtr Read() override;
  TimedEventPtr SetTimer(const int milliseconds, const MessagePtr& msg, bool repeatable = false) override;
  MessagePtr StopTimer(const int timerId) override;
  MessagePtr FirstQueuedMessage();

private:
  bool CheckForMessage();
};
