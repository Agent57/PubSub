#pragma warning (disable:4251)
#pragma once

#include <unordered_set>
#include <map>
#include <mutex>

#include "IBroker.h"

class Broker: public IBroker
{
  typedef std::unordered_set<ConnectorPtr> ConnectorSet;
  typedef ConnectorSet::iterator ConnectorSetItem;
  typedef std::shared_ptr<ConnectorSet> ConnectorSetPtr;

  typedef std::map<std::string, ConnectorSetPtr> SubscriberMap;
  typedef SubscriberMap::iterator SubscriberMapItem;

private:
  std::mutex m_lock;
  SubscriberMap m_subscriptions;

  bool PublishToSubscribers(const MessagePtr& msg);
  bool NewSubscriptionType(const std::string& type);
  void Broker::RemoveSubscription(const std::string& type, const ConnectorPtr& connector);

public:
  void Subscribe(const std::string& type, const ConnectorPtr& connector) override;
  void Unsubscribe(const std::string& type, const ConnectorPtr& connector) override;
  void Unsubscribe(const std::set<std::string>& type, const ConnectorPtr& connector) override;
  bool Send(const MessagePtr& msg) override;
};

