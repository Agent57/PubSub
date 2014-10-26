#include "Broker.h"
#include "Logger.h"

void Broker::Subscribe(const std::string& type, const ConnectorPtr& connector)
{
  std::lock_guard<std::mutex> lock(m_lock);
  if (NewSubscriptionType(type))
    m_subscriptions[type] = std::make_shared<ConnectorSet>();

  m_subscriptions[type]->insert(connector);
}

void Broker::Unsubscribe(const std::string& type, const ConnectorPtr& connector)
{
  std::lock_guard<std::mutex> lock(m_lock);
  if (m_subscriptions.count(type) > 0)
    m_subscriptions[type]->erase(connector);
}

bool Broker::Send(const MessagePtr& msg)
{
  std::lock_guard<std::mutex> lock(m_lock);
  return m_subscriptions.count(msg->GetTypeName()) > 0
    ? PublishToSubscribers(msg)
    : false;
}

bool Broker::PublishToSubscribers(const MessagePtr& msg)
{
  bool SentMessage = true;

  ConnectorSetPtr set = m_subscriptions[msg->GetTypeName()];
  for (ConnectorSetItem item = set->begin(); item != set->end(); ++item)
    SentMessage &= (*item)->Send(msg);

  return SentMessage;
}

bool Broker::NewSubscriptionType(const std::string& type)
{
  return m_subscriptions.find(type) == m_subscriptions.end();
}
