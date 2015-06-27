#include "ProtobufWrapper.h"
#include "Logger.h"

#include "wrapper.pb.h"

#include <google/protobuf/descriptor.h>

DataBufferPtr ProtobufWrapper::Serialise(const MessagePtr& msg)
{
  std::string msgType = typeid(*msg).name();

  // Serialise the message into a byte array
  int dataSize = msg->ByteSize();
  auto buffer = std::make_shared<DataBuffer>(dataSize);
  msg->SerializeToArray(buffer->data(), dataSize);

  // Wrap the message with a header for sending over a TCP socket
  Exchange::Wrapper wrapper;
  wrapper.set_messageid(msg->GetTypeName());
  wrapper.set_payload(buffer->data(), dataSize);
  wrapper.set_payloadsize(dataSize);

  // Serialise the wrapped message
  int bufferSize = wrapper.ByteSize();
  auto buffer2 = std::make_shared<DataBuffer>(bufferSize);
  wrapper.SerializeToArray(buffer2->data(), bufferSize);

  return buffer2;
}

ProtobufMessagePtr ProtobufWrapper::Deserialise(const DataBufferPtr& buffer)
{
  Exchange::Wrapper wrapper;
  wrapper.ParseFromArray(buffer->data(), buffer->size());
  std::string name = wrapper.messageid();

  // Construct instance of correct message type
  auto pool = google::protobuf::DescriptorPool::generated_pool();
  const google::protobuf::Descriptor* descriptor = pool->FindMessageTypeByName(name);
  if (!descriptor)
  {
    LogEvent(Error, "Failed to deserialise message of type [" << name << "]");
    return nullptr;
  }

  auto prototype = google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
  ProtobufMessagePtr protobuf = std::shared_ptr<google::protobuf::Message>(prototype->New());

  // Deserialise message payload
  protobuf->ParseFromString(wrapper.payload());

  return protobuf;
}
