#pragma once

#include "MessageLoopRunner.h"

MessageLoopRunner::~MessageLoopRunner()
{
  m_messageLoop->Shutdown();

  if (m_thread.joinable())
    m_thread.join();
}

void MessageLoopRunner::RunMessageLoop(const MessageLoopPtr& messageLoop)
{
  m_messageLoop = messageLoop;
  m_thread = std::thread(&IMessageLoop::Execute, m_messageLoop);
}
