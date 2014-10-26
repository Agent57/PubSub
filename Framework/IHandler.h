#pragma once

#include <map>
#include <memory>

class IHandler
{
public:
  virtual ~IHandler(void)  {}
};

typedef std::shared_ptr<IHandler> HandlerPtr;

class IDomainManager;

template<class T>
class MessageHandler : public virtual IHandler
{
public:
  virtual bool Process(const std::shared_ptr<T const>& pMsg)=0;
};

typedef std::map<std::string, HandlerPtr> HandlerMap;
typedef std::shared_ptr<HandlerMap> HandlerMapPtr;
typedef HandlerMap::iterator HandlerMapItem;
