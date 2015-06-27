#include "Logger.h"
#include "ITimeout.h"

TimeoutWait::TimeoutWait(long long value) : m_remaining(value)
{
}

std::cv_status TimeoutWait::Wait(std::condition_variable& conditional, std::mutex& mutex)
{
  LogEvent(Trace, "Timer set for " << m_remaining << " milliseconds");
  std::unique_lock<std::mutex> lock(mutex);
  return conditional.wait_for(lock, std::chrono::milliseconds(m_remaining));
}

long long TimeoutWait::Remaining()
{
  return m_remaining;
}

std::cv_status TimeoutExpired::Wait(std::condition_variable& conditional, std::mutex& mutex)
{
  return std::cv_status::timeout;
}

long long TimeoutExpired::Remaining()
{
  LogEvent(Trace, "Timer has already expired");
  return -1;
}

std::cv_status NoTimeout::Wait(std::condition_variable& conditional, std::mutex& mutex)
{
  std::unique_lock<std::mutex> lock(mutex);
  conditional.wait(lock);
  return std::cv_status::no_timeout;
}

long long NoTimeout::Remaining()
{
  return 0;
}

TimeoutPtr TimeoutFactory::CreateTimeout(long long timeout)
{
  if (timeout > 0)
    return std::make_shared<TimeoutWait>(timeout);

  if (timeout < 0)
    return std::make_shared<TimeoutExpired>();

  return std::make_shared<NoTimeout>();
}
