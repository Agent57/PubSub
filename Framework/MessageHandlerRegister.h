#pragma once

#include "IMessage.h"

#include <mutex>
#include <set>
class MessageHandlerRegister
{
private:
  std::mutex m_lock;
  HandlerMapPtr m_handlers;
  std::set<std::string> m_types;

public:
  MessageHandlerRegister();
  ~MessageHandlerRegister();

  void RegisterHandler(const ::google::protobuf::Descriptor* type, const HandlerPtr& handler);
  void DeregisterHandler(const ::google::protobuf::Descriptor* type);
  const HandlerPtr GetHandlerForMessage(std::string type);
  const std::set<std::string> GetHandlerTypes() { return m_types; }
};

typedef std::shared_ptr<MessageHandlerRegister> MessageHandlerRegisterPtr;
