#pragma once

#include <memory>

class IConnectionParameters
{
public:
  virtual ~IConnectionParameters(void) {};
};

typedef std::shared_ptr<IConnectionParameters> ConnectionParametersPtr;
