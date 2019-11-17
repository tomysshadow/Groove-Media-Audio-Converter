#pragma once
#include <windows.h>

enum REL_FILE_POS {
	REL_FILE_POS_START,
	REL_FILE_POS_RELATIVE,
	REL_FILE_POS_END
};

class MemoryFile {
public:
	bool open(LPCSTR fileName);
	bool close();

	struct Read {
		virtual int __thiscall incrementInstanceCount();
		virtual int __thiscall decrementInstanceCount();
		virtual MemoryFile::Read* __thiscall deleteInstance(bool free);
		virtual size_t __thiscall getLength();
		virtual size_t __thiscall getLengthUnsafe();
		virtual size_t __thiscall getFilePos();
		virtual size_t __thiscall setFilePos(size_t filePos, REL_FILE_POS relFilePos);
		virtual size_t __thiscall readCount(unsigned char* destination, size_t count);
		virtual unsigned char* __thiscall readRange(size_t posStart, size_t posEnd);
		virtual void __thiscall doNothing();
		virtual int __thiscall getFlags(int unknown);
		int instanceCount = 0;
		unsigned char* start = 0;
		unsigned char* filePointer = 0;
		size_t length = 0;
	};

	HANDLE file;
	SIZE_T bufferSize = 0;
	PBYTE buffer = NULL;
	Read read;
};