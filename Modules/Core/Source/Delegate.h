#ifndef DELEGATE_H
#define DELEGATE_H

#include <list>
#include <functional>

template<typename T>
class DelegateImpl;

template <typename R, typename... Args>
class DelegateImpl<R(Args...)>
{
protected:

	struct InvocationData
	{
		void* Object = nullptr;
		std::function<R(Args...)> FunctionPointer;
	};
};

template<typename T>
class Delegate;

template<typename R, typename... Args>
class Delegate<R(Args...)> final : private DelegateImpl<R(Args...)>
{
public:

	using FunctionPtr = R(Args...);
	using InvocationData = DelegateImpl<R(Args...)>::InvocationData;

	Delegate()
		: IsBound{ false }
	{}

	Delegate(const Delegate&) = delete;
	Delegate(const Delegate&&) = delete;
	~Delegate()
	{
		this->Binding.FunctionPointer = nullptr;
	}

	template<typename T, R(T:: * TMethod)(Args...)>
	void Bind(T* Object)
	{
		if (!GetIsBound())
		{
			this->Binding.Object = Object;
			this->Binding.FunctionPointer = [Object](Args... Arg)->R { return (Object->*TMethod)(Arg...); };
			IsBound = true;
		}
	}

	template<R(*TMethod)(Args...)>
	void Bind()
	{
		if (!GetIsBound())
		{
			this->Binding.FunctionPointer = [](Args... Arg)->R { return (*TMethod)(Arg...); };
			IsBound = true;
		}
	}

	void Invoke(Args... Arg)
	{
		Binding.FunctionPointer(Arg...);
	}

	bool GetIsBound() const
	{
		return IsBound;
	}
private:

	InvocationData Binding;
	bool IsBound;
};

template<typename T>
class MulticastDelegate;

template<typename R, typename... Args>
class MulticastDelegate<R(Args...)> final : private DelegateImpl<R(Args...)>
{

public:

	using FunctionPtr = R(Args...);
	using InvocationData = DelegateImpl<R(Args...)>::InvocationData;

	MulticastDelegate() = default;
	MulticastDelegate(const MulticastDelegate&) = delete;
	MulticastDelegate(const MulticastDelegate&&) = delete;

	~MulticastDelegate()
	{
		this->mBindings.clear();
	}

	template<typename T, void(T:: * TMethod)(Args...)>
	void Bind(T* Object)
	{
		this->mBindings.push_back({ Object, [Object](Args... Arg)->void { (Object->*TMethod)(Arg...); } });
	}


	template<typename T, void(T:: * TMethod)(Args...)>
	void Unbind(T* Object)
	{
		this->mBindings.remove_if([Object](const InvocationData& Data) { return Data.Object == Object; });
	}

	void Broadcast(Args... Arg)
	{
		for (auto Binding : mBindings)
		{
			Binding.FunctionPointer(Arg...);
		}
	}

private:

	std::list<InvocationData> mBindings;
};

#endif