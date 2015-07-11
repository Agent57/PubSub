#include "ConsoleLogger.h"

#include <iomanip>
#include <iostream>
#include <sstream>

std::string ConsoleLogFormatter::FormatLogOutput(const LogEventData& event)
{
  // Format log message output for the console
  std::stringstream text;
  text << std::right << std::setfill('0') << std::setw(8) << event.DisplayTime
       << "  " << std::setw(5) << event.ThreadId << std::setfill(' ')
       << "  " << std::left << std::setw(9) << event.Severity()
       << event.Text << std::endl;
  return text.str();
}

void ConsoleLogWriter::WriteLogEvent(std::string text)
{
  std::cout << text;
}