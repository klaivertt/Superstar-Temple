#pragma once

#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>

template<typename... Args>
class DelegateBase
{
private:
	struct CallBackWrapper
	{
		std::function<void(Args...)> callback;
		int id;
	};

	std::vector<CallBackWrapper> callbacks;
	int nextId = 0;

public:
	int Add(std::function<void(Args...)> _callback)
	{
		callbacks.push_back({ _callback, nextId });
		nextId++;

		return nextId - 1;
	}

	// Add your function pointer with the object instance and the method to call
	// @return the id of the callback, you can use it to remove it later
	template<typename T>
	int Add(T* object, void (T::* method)(Args...))
	{
		return Add([object, method](Args... args) { (object->*method)(args...); });
	}

	void Remove(int _id)
	{
		callbacks.erase(std::remove_if(callbacks.begin(), callbacks.end(),
			[_id](const CallBackWrapper& wrapper) { return wrapper.id == _id; }),
			callbacks.end());
	}

	void RemoveAll(void)
	{
		callbacks.clear();
	}

	// Call all the functions you added
	void Broadcast(Args... _args)
	{
		for (auto& wrapper : callbacks)
		{
			wrapper.callback(_args...);
		}
	}
};

// Delegate sans paramètre
#define DECLARE_DELEGATE(DelegateName) \
class DelegateName : public DelegateBase<> {};

// Delegate avec 1 paramètre
#define DECLARE_DELEGATE_OneParam(DelegateName, Param1Type) \
class DelegateName : public DelegateBase<Param1Type> {};

// Delegate avec 2 paramètres
#define DECLARE_DELEGATE_TwoParams(DelegateName, Param1Type, Param2Type) \
class DelegateName : public DelegateBase<Param1Type, Param2Type> {};

// Delegate avec 3 paramètres
#define DECLARE_DELEGATE_ThreeParams(DelegateName, Param1Type, Param2Type, Param3Type) \
class DelegateName : public DelegateBase<Param1Type, Param2Type, Param3Type> {};

// Delegate avec 4 paramètres
#define DECLARE_DELEGATE_FourParams(DelegateName, Param1Type, Param2Type, Param3Type, Param4Type) \
class DelegateName : public DelegateBase<Param1Type, Param2Type, Param3Type, Param4Type> {};
