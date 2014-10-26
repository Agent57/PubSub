#pragma once

#include <condition_variable>
#include <memory>

class ITimeout
{
public:
  virtual std::cv_status Wait(std::condition_variable& conditional, std::mutex& lock) = 0;
  virtual long long Remaining() = 0;
};

typedef std::shared_ptr<ITimeout> TimeoutPtr;

class TimeoutFactory
{
public:
  static const TimeoutPtr CreateTimeout(long long timeout);
};

class TimeoutWait : public ITimeout
{
private:
  long long m_remaining;
public:
  TimeoutWait(long long value);
  std::cv_status Wait(std::condition_variable& conditional, std::mutex& lock);
  long long Remaining();
};

class TimeoutExpired : public ITimeout
{
public:
  std::cv_status Wait(std::condition_variable& conditional, std::mutex& lock);
  long long Remaining();
};

class NoTimeout : public ITimeout
{
public:
  std::cv_status Wait(std::condition_variable& conditional, std::mutex& lock);
  long long Remaining();
};
