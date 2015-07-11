#include <windows.h>
#include <iomanip>
#include <sstream>

#include "IDELogger.h"


std::string IDELogFormatter::FormatLogOutput(const LogEventData& event)
{
  // Format log message output for the Visual Studio Debugger
  std::stringstream text;
  text << event.File
       << "(" << event.Line << "): "
       << std::setfill('0') << std::setw(8) << event.DisplayTime
       << " [" << event.ThreadId << "] "
       << event.Severity() << ": "
       << event.Text << std::endl;
  return text.str();
}

void IDELogWriter::WriteLogEvent(std::string text)
{
  OutputDebugString(text.c_str());
}