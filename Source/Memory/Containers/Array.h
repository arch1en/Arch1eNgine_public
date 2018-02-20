#pragma once

namespace an
{
	template <class T>
	class Array
	{
	public:
		Array(T aData);

		const size_t		Size() { return mSize; }
		T&					operator[](int aIndex) { return mData + aIndex; }

	private:

		T*		mData;
		size_t	mSize;

	};

	// Static Array
	template <class T>
	class SArray : public Array
	{
	public:


	private:

	};

	// Dynamic Array
	template <class T>
	class DArray
	{
	public:

		void	Add(T aElement);
		int		Remove(T aElement);
		void	Empty();
		void	Count();
	};

}

