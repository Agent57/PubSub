#include "Broker.h"

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
  RemoveSubscription(type, connector);
}

void Broker::RemoveSubscription(const std::string& type, const ConnectorPtr& connector)
{
  if (m_subscriptions.count(type) > 0)
    m_subscriptions[type]->erase(connector);
}

void Broker::Unsubscribe(const std::set<std::string>& types, const ConnectorPtr& connector)
{
  std::lock_guard<std::mutex> lock(m_lock);

  for (auto type = types.begin(); type != types.end(); ++type)
    RemoveSubscription(*type, connector);
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
