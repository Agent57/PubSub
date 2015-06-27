
#include <iomanip>
#include <iostream>

#include "ConsoleLogger.h"

ConsoleLogger::~ConsoleLogger()
{
}

void ConsoleLogger::HandleLogEvent(const LogEventDataPtr& pLog)
{
  if (pLog == nullptr)
    return;

  // Format log message output for the console
  std::cout << std::right << std::setfill('0') << std::setw(8) << pLog->DisplayTime
    << "  " << std::setw(5) << pLog->ThreadId << std::setfill(' ')
    << "  " << std::left << std::setw(9) << pLog->Severity
    << pLog->Text << std::endl;
}
