#pragma once
#include "shared.h"
#include <windows.h>

class MemoryFile {
	public:
	bool open(LPCSTR fileName);
	bool close();

	enum MOVE_METHOD {
		MOVE_METHOD_BEGIN,
		MOVE_METHOD_CURRENT,
		MOVE_METHOD_END
	};

	struct Read {
		virtual int __thiscall incrementReferenceCount();
		virtual int __thiscall decrementReferenceCount();
		virtual Read* __thiscall release(bool free);
		virtual size_t __thiscall getLength();
		virtual size_t __thiscall getRawLength();
		virtual size_t __thiscall getPosition();
		virtual size_t __thiscall setPosition(size_t distance, MOVE_METHOD moveMethod);
		virtual size_t __thiscall readData(unsigned char* destination, size_t size);
		virtual unsigned char* __thiscall readDataRange(size_t begin, size_t end);
		virtual void __thiscall unknown();
		virtual int __thiscall getFlags(int unknown);

		int referenceCount = 0;

		unsigned char* data = 0;
		unsigned char* position = 0;
		size_t length = 0;
	};

	HANDLE file = NULL;

	SIZE_T dataSize = 0;
	PBYTE data = NULL;

	Read read;
};