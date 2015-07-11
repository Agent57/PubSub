#pragma once

#include <memory>
#include <string>

class ILogWriter
{
public:
  virtual ~ILogWriter() {}
  virtual void WriteLogEvent(std::string) {};
};
typedef std::unique_ptr<ILogWriter> LogWriterPtr;
