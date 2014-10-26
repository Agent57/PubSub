#pragma once

#include "IMessage.h"
#include "ISerialiser.h"

#include <google/protobuf/io/coded_stream.h>
#include "google/protobuf/io/zero_copy_stream_impl.h"

class ProtobufWrapper : public Iserialiser
{
public:
  const DataBufferPtr Serialise(const MessagePtr& msg);
  const ProtobufMessagePtr Deserialise(const DataBufferPtr& buffer);
};
