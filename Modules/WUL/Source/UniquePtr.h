#ifndef UNIQUEPTR_H
#define UNIQUEPTR_H

template<class T>
class UniquePtr
{
public:
	UniquePtr()
		: ObjectPtr{nullptr}
	{}
	UniquePtr(T* NewObjectPtr)
		: ObjectPtr{ NewObjectPtr }
	{}
	
	virtual ~UniquePtr()
	{
		delete ObjectPtr;
	}

	T* Get()
	{
		return ObjectPtr;
	}

private:

	T* ObjectPtr;

};

#endif