#pragma once

#include <condition_variable>
#include <memory>

class ITimeout
{
public:
  virtual ~ITimeout() {}
  virtual std::cv_status Wait(std::condition_variable& conditional, std::mutex& lock) = 0;
  virtual long long Remaining() = 0;
};

typedef std::shared_ptr<ITimeout> TimeoutPtr;

class TimeoutFactory
{
public:
  static TimeoutPtr CreateTimeout(long long timeout);
};

class TimeoutWait : public ITimeout
{
  long long m_remaining;

public:
  explicit TimeoutWait(long long value);
  std::cv_status Wait(std::condition_variable& conditional, std::mutex& lock) override;
  long long Remaining() override;
};

class TimeoutExpired : public ITimeout
{
public:
  std::cv_status Wait(std::condition_variable& conditional, std::mutex& lock) override;
  long long Remaining() override;
};

class NoTimeout : public ITimeout
{
public:
  std::cv_status Wait(std::condition_variable& conditional, std::mutex& lock) override;
  long long Remaining() override;
};
