#include "Memory.h"

#include <stdint.h>

namespace WUL
{
	void MemoryCopy(void* aDestination, const void* aSource, size_t aSize)
	{
		//uint8_t* pDestination = static_cast<uint8_t*>(aDestination);
		//const uint8_t* pSource = static_cast<const uint8_t*>(aSource);

		//// 4 byte copy
		//size_t Loops = (aSize / sizeof(uint32_t));
		//for (size_t i = 0; i < Loops; ++i)
		//{
		//	*((uint32_t*)pDestination) = *((uint32_t*)pSource);
		//	pDestination += sizeof(uint32_t);
		//	pSource += sizeof(uint32_t);
		//}

		//// 1 byte copy. If the memory is not WORD aligned, then remaining bytes will be copied now.
		//Loops = (aSize % sizeof(uint32_t));
		//for (size_t i = 0; i < Loops; ++i)
		//{
		//	*pDestination = *pSource;
		//	++pDestination;
		//	++pSource;
		//}
	}
}