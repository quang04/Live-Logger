#pragma once
#include <algorithm>
#include <functional>
#include <set>


template<typename... Args>
class CallbackEventHandler
{
public:
	CallbackEventHandler() {}
	virtual ~CallbackEventHandler() {}


public:
	using handler = std::function<void(Args...)>;

	struct HandlerWrapper {
		// id is unique
		int id;
		handler h;

		HandlerWrapper(const handler& _h, int _id)
			: h(_h), id(_id) {};

		bool operator=(const HandlerWrapper& other)
		{
			return id == other.id;
		}
		bool operator<(const HandlerWrapper& other) const
		{
			return id < other.id;
		}
	};
public:
	bool AddHandler(const handler& h, int id)
	{
		if (handlers.find(HandlerWrapper(h, id)) != handlers.end())
			return false;

		handlers.emplace(HandlerWrapper(h, id));

		return true;
	}

	bool RemoveHandler(int id)
	{
		
		auto it = std::find_if(handlers.begin(), handlers.end(), [id](const auto& e) {
			return e.id == id;
			});

		if (it == handlers.end()) return false;
		
		handlers.erase(it);
		
		return true;
	}

	void InvokeAll(const Args&... args)
	{
		for (const HandlerWrapper& h : handlers)
		{
			if(h.h) h.h(args...);
		}
	}

	bool InvokeOne(const Args&... arg, int id)
	{
		auto it = std::find_if(handlers.begin(), handlers.end(), [id](const auto& e) {
			return e.id == id;
			});

		if (it == handlers.end()) return false;

		it->h(arg...);

		return true;
	}

private:
	std::set<HandlerWrapper> handlers;
};