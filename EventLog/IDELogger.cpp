#include <windows.h>
#include <iomanip>
#include <sstream>

#include "IDELogger.h"


void IDELogger::HandleLogEvent(const LogEventDataPtr& pLog)
{
  if (pLog == nullptr)
    return;

  // Format log message output for the Visual Studio Debugger
  std::stringstream ss;

  ss << pLog->File
     << "(" << pLog->Line << "): "
     << std::setfill('0') << std::setw(8) << pLog->DisplayTime
     << " [" << pLog->ThreadId << "] "
     << pLog->Severity << ": "
     << pLog->Text << std::endl;

  OutputDebugString(ss.str().c_str());
}
