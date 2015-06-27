#include "ProtoMessage.h"

// Framework code
void MessageBroker::Subscribe(std::string type, const MessageVisitorPtr& visitor)
{
  m_visitors[type] = visitor;
}

void MessageBroker::Publish(const ProtoMessagePtr& message)
{
  auto visitor = m_visitors[message->GetTypeName()];
  message->Accept(visitor);
}

// Boilerplate message wrapper
void RemoteSubscriptionMessage::Accept(MessageVisitorPtr visitor) const
{
  if (auto remoteSubscriptionVisitor = std::dynamic_pointer_cast<RemoteSubscriptionVisitor>(visitor))
    remoteSubscriptionVisitor->Visit(m_message);
  else { /* Throw Error */ }
}

// Message handler
void RemoteSubscriptionVisitor::Visit(const Exchange::RemoteSubscription &message)
{
  //auto value = message.subscriptionid();
}