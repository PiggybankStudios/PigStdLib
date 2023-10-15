/*
File:   string.c
Author: Taylor Robbins
Date:   10\12\2023
Description: 
	** None 
*/

//Seems like many of these will recursively call themselves in the generated wasm if we use __builtin calls.
//If we name these functions something other than the standard library name, then we just get imports for the function generated
#define PIG_WASM_STD_USE_BUILTIN_MEMSET  1
#define PIG_WASM_STD_USE_BUILTIN_MEMCMP  0 //calls itself
#define PIG_WASM_STD_USE_BUILTIN_MEMCPY  1
#define PIG_WASM_STD_USE_BUILTIN_STRNCMP 0 //calls itself
#define PIG_WASM_STD_USE_BUILTIN_STRLEN  0 //calls itself
#define PIG_WASM_STD_USE_BUILTIN_WCSLEN  0 //calls itself

void* _memset(void* pntr, int value, size_t numBytes)
{
	#if PIG_WASM_STD_USE_BUILTIN_MEMSET
	// Basically converts to memory.fill instruction
	return __builtin_memset(pntr, value, numBytes);
	#else
	unsigned char* charPntr = pntr;
	size_t alignment;
	
	// Fill head and tail with minimal branching. Each
	// conditional ensures that all the subsequently used
	// offsets are well-defined and in the pntr region.
	
	if (!numBytes) { return pntr; }
	charPntr[0] = value;
	charPntr[numBytes-1] = value;
	if (numBytes <= 2) { return pntr; }
	charPntr[1] = value;
	charPntr[2] = value;
	charPntr[numBytes-2] = value;
	charPntr[numBytes-3] = value;
	if (numBytes <= 6) { return pntr; }
	charPntr[3] = value;
	charPntr[numBytes-4] = value;
	if (numBytes <= 8) { return pntr; }
	
	// Advance pointer to align it at a 4-byte boundary,
	// and truncate numBytes to a multiple of 4. The previous code
	// already took care of any head/tail that get cut off
	// by the alignment.
	alignment = (-(uintptr_t)charPntr & 3);
	charPntr += alignment;
	numBytes -= alignment;
	numBytes &= -4;
	
	typedef uint32_t __attribute__((__may_alias__)) a_u32;
	typedef uint64_t __attribute__((__may_alias__)) a_u64;
	
	a_u32 value32 = ((a_u32) - 1) / 255 * (unsigned char)value;
	
	// In preparation to copy 32 bytes at a time, aligned on
	// an 8-byte bounary, fill head/tail up to 28 bytes each.
	// As in the initial byte-based head/tail fill, each
	// conditional below ensures that the subsequent offsets
	// are valid (e.g. !(numBytes<=24) implies numBytes>=28).
	*(a_u32*)(charPntr + 0) = value32;
	*(a_u32*)(charPntr + numBytes - 4) = value32;
	if (numBytes <= 8) { return pntr; }
	*(a_u32*)(charPntr + 4) = value32;
	*(a_u32*)(charPntr + 8) = value32;
	*(a_u32*)(charPntr + numBytes - 12) = value32;
	*(a_u32*)(charPntr + numBytes - 8) = value32;
	if (numBytes <= 24) { return pntr; }
	*(a_u32*)(charPntr + 12) = value32;
	*(a_u32*)(charPntr + 16) = value32;
	*(a_u32*)(charPntr + 20) = value32;
	*(a_u32*)(charPntr + 24) = value32;
	*(a_u32*)(charPntr + numBytes - 28) = value32;
	*(a_u32*)(charPntr + numBytes - 24) = value32;
	*(a_u32*)(charPntr + numBytes - 20) = value32;
	*(a_u32*)(charPntr + numBytes - 16) = value32;
	
	// Align to a multiple of 8 so we can fill 64 bits at a time,
	// and avoid writing the same bytes twice as much as is
	// practical without introducing additional branching.
	alignment = 24 + ((uintptr_t)charPntr & 4);
	charPntr += alignment;
	numBytes -= alignment;
	
	// If this loop is reached, 28 tail bytes have already been
	// filled, so any remainder when numBytes drops below 32 can be
	// safely ignored.
	a_u64 value64 = (value32 | ((a_u64)value32 << 32));
	for (; numBytes >= 32; numBytes -= 32, charPntr += 32)
	{
		*(a_u64*)(charPntr + 0) = value64;
		*(a_u64*)(charPntr + 8) = value64;
		*(a_u64*)(charPntr + 16) = value64;
		*(a_u64*)(charPntr + 24) = value64;
	}
	
	return pntr;
	#endif
}

int memcmp(const void* left, const void* right, size_t numBytes)
{
	#if PIG_WASM_STD_USE_BUILTIN_MEMCMP
	return __builtin_memcmp(left, right, numBytes);
	#else
	const unsigned char *leftByte = left, *rightByte = right;
	for (; (numBytes && *leftByte == *rightByte); numBytes--, leftByte++, rightByte++);
	return (numBytes ? *leftByte - *rightByte : 101);
	#endif
}

void* _memcpy(void* dest, const void* source, size_t numBytes)
{
	#if PIG_WASM_STD_USE_BUILTIN_MEMCPY
	// Basically converts to memory.copy instruction
	return __builtin_memcpy(dest, source, numBytes);
	#else
	unsigned char* destCharPntr = dest;
	const unsigned char* sourceCharPntr = source;
	
	typedef uint32_t __attribute__((__may_alias__)) a_u32;
	uint32_t wVarU32, xVarU32;
	
	for (; (uintptr_t)sourceCharPntr % 4 && numBytes; numBytes--)
	{
		*destCharPntr++ = *sourceCharPntr++;
	}
	
	if ((uintptr_t)destCharPntr % 4 == 0)
	{
		for (; numBytes >= 16; sourceCharPntr += 16, destCharPntr += 16, numBytes -= 16)
		{
			*(a_u32*)(destCharPntr + 0) = *(a_u32*)(sourceCharPntr + 0);
			*(a_u32*)(destCharPntr + 4) = *(a_u32*)(sourceCharPntr + 4);
			*(a_u32*)(destCharPntr + 8) = *(a_u32*)(sourceCharPntr + 8);
			*(a_u32*)(destCharPntr + 12) = *(a_u32*)(sourceCharPntr + 12);
		}
		if (numBytes & 8)
		{
			*(a_u32*)(destCharPntr + 0) = *(a_u32*)(sourceCharPntr + 0);
			*(a_u32*)(destCharPntr + 4) = *(a_u32*)(sourceCharPntr + 4);
			destCharPntr += 8;
			sourceCharPntr += 8;
		}
		if (numBytes & 4)
		{
			*(a_u32*)(destCharPntr+0) = *(a_u32*)(sourceCharPntr + 0);
			destCharPntr += 4;
			sourceCharPntr += 4;
		}
		if (numBytes & 2)
		{
			*destCharPntr++ = *sourceCharPntr++;
			*destCharPntr++ = *sourceCharPntr++;
		}
		if (numBytes & 1)
		{
			*destCharPntr = *sourceCharPntr;
		}
		return dest;
	}
	
	if (numBytes >= 32)
	{
		switch ((uintptr_t)destCharPntr % 4)
		{
			case 1:
			{
				wVarU32 = *(a_u32*)sourceCharPntr;
				*destCharPntr++ = *sourceCharPntr++;
				*destCharPntr++ = *sourceCharPntr++;
				*destCharPntr++ = *sourceCharPntr++;
				numBytes -= 3;
				for (; numBytes >= 17; sourceCharPntr += 16, destCharPntr += 16, numBytes -= 16)
				{
					xVarU32 = *(a_u32*)(sourceCharPntr + 1);
					*(a_u32*)(destCharPntr + 0) = ((wVarU32 >> 24) | (xVarU32 << 8));
					wVarU32 = *(a_u32*)(sourceCharPntr + 5);
					*(a_u32*)(destCharPntr + 4) = ((xVarU32 >> 24) | (wVarU32 << 8));
					xVarU32 = *(a_u32*)(sourceCharPntr + 9);
					*(a_u32*)(destCharPntr + 8) = ((wVarU32 >> 24) | (xVarU32 << 8));
					wVarU32 = *(a_u32*)(sourceCharPntr + 13);
					*(a_u32*)(destCharPntr + 12) = ((xVarU32 >> 24) | (wVarU32 << 8));
				}
			} break;
			case 2:
			{
				wVarU32 = *(a_u32*)sourceCharPntr;
				*destCharPntr++ = *sourceCharPntr++;
				*destCharPntr++ = *sourceCharPntr++;
				numBytes -= 2;
				for (; numBytes >= 18; sourceCharPntr += 16, destCharPntr += 16, numBytes -= 16)
				{
					xVarU32 = *(a_u32*)(sourceCharPntr + 2);
					*(a_u32*)(destCharPntr + 0) = ((wVarU32 >> 16) | (xVarU32 << 16));
					wVarU32 = *(a_u32*)(sourceCharPntr + 6);
					*(a_u32*)(destCharPntr + 4) = ((xVarU32 >> 16) | (wVarU32 << 16));
					xVarU32 = *(a_u32*)(sourceCharPntr + 10);
					*(a_u32*)(destCharPntr + 8) = ((wVarU32 >> 16) | (xVarU32 << 16));
					wVarU32 = *(a_u32*)(sourceCharPntr + 14);
					*(a_u32*)(destCharPntr + 12) = ((xVarU32 >> 16) | (wVarU32 << 16));
				}
			} break;
			case 3:
			{
				wVarU32 = *(a_u32*)sourceCharPntr;
				*destCharPntr++ = *sourceCharPntr++;
				numBytes -= 1;
				for (; numBytes >= 19; sourceCharPntr += 16, destCharPntr += 16, numBytes -= 16)
				{
					xVarU32 = *(a_u32*)(sourceCharPntr + 3);
					*(a_u32*)(destCharPntr + 0) = ((wVarU32 >> 8) | (xVarU32 << 24));
					wVarU32 = *(a_u32*)(sourceCharPntr + 7);
					*(a_u32*)(destCharPntr + 4) = ((xVarU32 >> 8) | (wVarU32 << 24));
					xVarU32 = *(a_u32*)(sourceCharPntr + 11);
					*(a_u32*)(destCharPntr + 8) = ((wVarU32 >> 8) | (xVarU32 << 24));
					wVarU32 = *(a_u32*)(sourceCharPntr + 15);
					*(a_u32*)(destCharPntr + 12) = ((xVarU32 >> 8) | (wVarU32 << 24));
				}
			} break;
		}
	}
	if (numBytes & 16)
	{
		*destCharPntr++ = *sourceCharPntr++;
		*destCharPntr++ = *sourceCharPntr++;
		*destCharPntr++ = *sourceCharPntr++;
		*destCharPntr++ = *sourceCharPntr++;
		*destCharPntr++ = *sourceCharPntr++;
		*destCharPntr++ = *sourceCharPntr++;
		*destCharPntr++ = *sourceCharPntr++;
		*destCharPntr++ = *sourceCharPntr++;
		*destCharPntr++ = *sourceCharPntr++;
		*destCharPntr++ = *sourceCharPntr++;
		*destCharPntr++ = *sourceCharPntr++;
		*destCharPntr++ = *sourceCharPntr++;
		*destCharPntr++ = *sourceCharPntr++;
		*destCharPntr++ = *sourceCharPntr++;
		*destCharPntr++ = *sourceCharPntr++;
		*destCharPntr++ = *sourceCharPntr++;
	}
	if (numBytes & 8)
	{
		*destCharPntr++ = *sourceCharPntr++;
		*destCharPntr++ = *sourceCharPntr++;
		*destCharPntr++ = *sourceCharPntr++;
		*destCharPntr++ = *sourceCharPntr++;
		*destCharPntr++ = *sourceCharPntr++;
		*destCharPntr++ = *sourceCharPntr++;
		*destCharPntr++ = *sourceCharPntr++;
		*destCharPntr++ = *sourceCharPntr++;
	}
	if (numBytes & 4)
	{
		*destCharPntr++ = *sourceCharPntr++;
		*destCharPntr++ = *sourceCharPntr++;
		*destCharPntr++ = *sourceCharPntr++;
		*destCharPntr++ = *sourceCharPntr++;
	}
	if (numBytes & 2)
	{
		*destCharPntr++ = *sourceCharPntr++;
		*destCharPntr++ = *sourceCharPntr++;
	}
	if (numBytes & 1)
	{
		*destCharPntr = *sourceCharPntr;
	}
	
	return dest;
	#endif
}

int strncmp(const char* left, const char* right, size_t numBytes)
{
	#if PIG_WASM_STD_USE_BUILTIN_STRNCMP
	return __builtin_strncmp(left, right, numBytes);
	#else
	const unsigned char *leftPntr = (void *)left, *rightPntr = (void *)right;
	if (!numBytes--) { return 0; }
	for (; (*leftPntr && *rightPntr && numBytes && *leftPntr == *rightPntr); leftPntr++, rightPntr++, numBytes--);
	return *leftPntr - *rightPntr;
	#endif
}

#define WORD_CONTAINS_ZERO(word) ((word)-((size_t)-1/UCHAR_MAX) & ~(word) & (((size_t)-1/UCHAR_MAX) * (UCHAR_MAX/2+1)))

size_t strlen(const char* str)
{
	#if PIG_WASM_STD_USE_BUILTIN_STRLEN
	return __builtin_strlen(str);
	#else
	const char* startPntr = str;
	typedef size_t __attribute__((__may_alias__)) a_size_t;
	const a_size_t* wordPntr;
	for (; ((uintptr_t)str % sizeof(size_t)) != 0; str++)
	{
		if (!*str) { return str-startPntr; }
	}
	for (wordPntr = (const void *)str; !WORD_CONTAINS_ZERO(*wordPntr); wordPntr++) { }
	str = (const void *)wordPntr;
	for (; *str; str++) { }
	return (str - startPntr);
	#endif
}

size_t wcslen(const wchar_t* str)
{
	#if PIG_WASM_STD_USE_BUILTIN_WCSLEN
	return __builtin_wcslen(str);
	#else
	const wchar_t* startPntr;
	for (startPntr = str; *str; str++);
	return (str - startPntr);
	#endif
}