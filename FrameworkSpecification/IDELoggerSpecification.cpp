#include "CppUnitTest.h"

#include "IDELogger.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FrameworkSpecification
{
  TEST_CLASS(IDELoggerSpecification)
  {
  public:
    TEST_METHOD(IDE_debug_output_must_be_structured_in_a_very_specific_format)
    {
      IDELogFormatter formatter;
      LogEventData event(Debug, "12:34:567", "Text", "Path\\Filename", "unused", 123);

      auto result = formatter.FormatLogOutput(event);

      std::stringstream expected;
      expected << "Path\\Filename(123): 12:34:567 [" << event.ThreadId << "] Debug: Text" << std::endl;
      Assert::AreEqual(expected.str().c_str(), result.c_str());
    }
  };
}
