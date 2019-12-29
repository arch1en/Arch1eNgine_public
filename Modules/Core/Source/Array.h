#ifndef ARRAY_H
#define ARRAY_H

#include "Memory.h"

template<class T>
class Array
{
public:

	Array();
	virtual ~Array();
	void Add(T Element);
	void Initiate(int NumberOfElements);
	void Reset();
	void Remove(T Element);

	// Auto-shrink
	void Remove(int Index);
	int Num() const;
	int ByteSize() const;
	T Get(int Index);

	T& operator[] (int Index);
	T operator[] (int Index) const;

private:

	T* MemoryBlock;
	size_t ElementsNum;
};

#include "Array.inl"

#endif