#include "MessageHandlerRegister.h"
#include "Logger.h"


MessageHandlerRegister::MessageHandlerRegister()
{
  m_handlers = std::make_shared<HandlerMap>();
}

MessageHandlerRegister::~MessageHandlerRegister()
{
  m_handlers->clear();
  m_types.clear();
}

void MessageHandlerRegister::RegisterHandler(const ::google::protobuf::Descriptor* type, const HandlerPtr& handler)
{
  std::lock_guard<std::mutex> lock(m_lock);
  (*m_handlers)[type->full_name()] = handler;
  m_types.insert(type->full_name());
}

void MessageHandlerRegister::DeregisterHandler(const ::google::protobuf::Descriptor* type)
{
  std::lock_guard<std::mutex> lock(m_lock);
  m_handlers->erase(type->full_name());
  m_types.erase(type->full_name());
}

const HandlerPtr MessageHandlerRegister::GetHandlerForMessage(std::string type)
{
  if (m_handlers->find(type) == m_handlers->end())
  {
    LogEvent(Error, "No handler registered for " << type);
    return nullptr;
  }

  return (*m_handlers)[type];
}
