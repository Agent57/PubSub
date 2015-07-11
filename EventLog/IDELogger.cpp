#include <windows.h>
#include <iomanip>
#include <sstream>

#include "IDELogger.h"


void IDELogger::FormatLogOutput(const LogEventData& event)
{
  // Format log message output for the Visual Studio Debugger
  std::stringstream text;
  text << event.File
       << "(" << event.Line << "): "
       << std::setfill('0') << std::setw(8) << event.DisplayTime
       << " [" << event.ThreadId << "] "
       << event.Severity() << ": "
       << event.Text << std::endl;
  SetLogOutput(text.str());
}

void IDEWriter::WriteLogEvent(std::string text)
{
  OutputDebugString(text.c_str());
}