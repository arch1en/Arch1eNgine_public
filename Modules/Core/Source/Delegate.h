#ifndef DELEGATE_H
#define DELEGATE_H

#include <list>
#include <functional>

template<typename T>
class MulticastDelegate;

template<typename R, typename... Args>
class MulticastDelegate<R(Args...)>
{

public:

    using FunctionPtr = R(Args...);

    MulticastDelegate() = default;
    MulticastDelegate(const MulticastDelegate&) = delete;
    MulticastDelegate(const MulticastDelegate&&) = delete;

    ~MulticastDelegate()
    {
        mBindings.clear();
    }

    template<typename T, void(T:: * TMethod)(Args...)>
    void Bind(T* Object)
    {
        mBindings.push_back({ Object, [Object](Args... Arg)->void { (Object->*TMethod)(Arg...); } });
    }

    void Bind(void(*TMethod)(Args...))
    {
        mBindings.push_back({ nullptr, [](Args... Arg)->void { *TMethod(Arg...); } });
    }

    template<typename T, void(T:: * TMethod)(Args...)>
    void Unbind(T* Object)
    {
        mBindings.remove_if([Object](const InvocationData& Data) { return Data.Object == Object; });
    }

    void Broadcast(Args... Arg)
    {
        for (auto Binding : mBindings)
        {
            Binding.FunctionPointer(Arg...);
        }
    }

private:

    struct InvocationData
    {
        void* Object = nullptr;
        std::function<R(Args...)> FunctionPointer;
    };

    std::list<InvocationData> mBindings;

};

#endif