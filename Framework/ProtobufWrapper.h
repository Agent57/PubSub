#pragma once

#include "IMessage.h"
#include "ISerialiser.h"

class ProtobufWrapper : public Iserialiser
{
public:
  DataBufferPtr Serialise(const MessagePtr& msg) override;
  ProtobufMessagePtr Deserialise(const DataBufferPtr& buffer) override;
};
