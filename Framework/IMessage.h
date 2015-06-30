#pragma once

#include <memory>

#include <google/protobuf/message.h>

#include "IHandler.h"

typedef std::shared_ptr<::google::protobuf::Message> ProtobufMessagePtr;
typedef std::shared_ptr<::google::protobuf::Message const> ProtobufMessageConstPtr;

class IMessage : virtual public ::google::protobuf::Message
{
public:
  IMessage() : ::google::protobuf::Message() {}

  virtual bool CallHandler(const HandlerPtr& handler) = 0;
};

typedef std::shared_ptr<IMessage> MessagePtr;
typedef std::shared_ptr<IMessage const> MessageConstPtr;
