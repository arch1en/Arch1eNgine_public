
template<class T>
Array<T>::Array()
	: MemoryBlock{ nullptr }
	, ElementsNum{ 0 }
{}

template<class T>
Array<T>::~Array()
{
	Reset();
}

template<class T>
void Array<T>::Add(T Element)
{
	T* NewMemoryBlock = new T[ElementsNum + 1];
	for (size_t i = 0; i < ElementsNum; i++)
	{
		NewMemoryBlock[i] = MemoryBlock[i];
	}

	delete[] MemoryBlock;
	NewMemoryBlock[ElementsNum] = Element;
	ElementsNum += 1;
	MemoryBlock = NewMemoryBlock;
}

template<class T>
void Array<T>::Initiate(int NumberOfElements)
{
	T* NewMemoryBlock = new T[NumberOfElements];
	ElementsNum = NumberOfElements;
}

template<class T>
void Array<T>::Reset()
{
	delete[] MemoryBlock;
	ElementsNum = 0;
}

template<class T>
void Array<T>::Remove(T Element)
{
	for (size_t i = ElementsNum - 1; i >= 0; i--)
	{
		if (MemoryBlock[i] == Element)
		{
			Remove((int) i);
			break;
		}
	}
}

// Auto-shrink
template<class T>
void Array<T>::Remove(int Index)
{
	T* NewMemoryBlock = new T[ElementsNum - 1];

	WUL::MemoryCopy(NewMemoryBlock, MemoryBlock, (Index) * sizeof(T));
	WUL::MemoryCopy(NewMemoryBlock + (Index), MemoryBlock + (Index + 1), (ElementsNum - 1 - Index) * sizeof(T));

	delete[] MemoryBlock;	
	MemoryBlock = NewMemoryBlock;
	ElementsNum = ElementsNum - 1;
}

template<class T>
int Array<T>::Num() const
{
	return ElementsNum;
}

template<class T>
int Array<T>::ByteSize() const
{
	return sizeof(T) * ElementsNum;
}

template<class T>
T Array<T>::Get(int Index)
{
	return MemoryBlock[Index];
}

template<class T>
T& Array<T>::operator[] (int Index)
{
	return MemoryBlock[Index];
}

template<class T>
T Array<T>::operator[] (int Index) const
{
	return MemoryBlock[Index];
}
