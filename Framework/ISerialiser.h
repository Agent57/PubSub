#pragma once

#include <memory>
#include <vector>

typedef std::vector<char> DataBuffer;
typedef std::shared_ptr<DataBuffer> DataBufferPtr;

class Iserialiser
{
public:
  virtual ~Iserialiser(void) {};
  virtual const DataBufferPtr Serialise(const MessagePtr& msg) = 0;
  virtual const ProtobufMessagePtr Deserialise(const DataBufferPtr& buffer) = 0;
};

typedef std::shared_ptr<Iserialiser> SerialiserPtr;
