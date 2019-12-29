#ifndef DELEGATE_H
#define DELEGATE_H

template<class OwnerType, class Args...>
class Delegate
{
public:

	void BindRaw(OwnerType* CallbackObject, Args FunctionArguments...)
};

#endif