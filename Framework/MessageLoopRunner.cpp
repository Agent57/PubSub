#pragma once

#include "MessageLoopRunner.h"
#include "Logger.h"

void MessageLoopRunner::RunMessageLoop(const MessageLoopPtr& messageLoop)
{
  m_messageLoop = messageLoop;
  m_thread = std::thread(&IMessageLoop::Execute, m_messageLoop);
}

void MessageLoopRunner::StopMessageLoop()
{
  m_messageLoop->Shutdown();

  if (m_thread.joinable())
    m_thread.join();
}
