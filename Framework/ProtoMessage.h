#pragma once

#include "RemoteSubscription.pb.h"
#include <map>
#include <memory>

// Framework code
class MessageVisitor
{
public:
  virtual ~MessageVisitor() {}
};
typedef std::shared_ptr<MessageVisitor> MessageVisitorPtr;

class ProtoMessage
{
public:
  virtual ~ProtoMessage() {}
  virtual void Accept(MessageVisitorPtr visitor) const = 0;
  virtual std::string GetTypeName() const = 0;
};
typedef std::shared_ptr<ProtoMessage> ProtoMessagePtr;

class MessageReceiver
{
  std::map<std::string, MessageVisitorPtr> m_visitors;

public:
  void AddVisitor(std::string type, const MessageVisitorPtr& visitor);
  void ProcessMessage(const ProtoMessagePtr& message);
};

// Boilerplate message wrapper
class RemoteSubscriptionMessage : public ProtoMessage
{
  Exchange::RemoteSubscription m_message;

public:
  RemoteSubscriptionMessage(const Exchange::RemoteSubscription &message)
    : m_message(message) {}

  std::string GetTypeName() const { return m_message.GetTypeName(); }

  void Accept(MessageVisitorPtr visitor) const;
};

// Message handler
class RemoteSubscriptionVisitor : public MessageVisitor
{
public:
  virtual void Visit(const Exchange::RemoteSubscription &message);
};