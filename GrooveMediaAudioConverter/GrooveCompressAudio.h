#pragma once
#include "shared.h"
#include <windows.h>
#include <dsound.h>

struct GrooveCompressAudioFormat {
	WORD channels = 1;
	WORD bitsPerSample = 16;
	DWORD samplesPerSec = 44100;
	SIZE_T dataSize = 0;
	LPLPDIRECTSOUNDBUFFER directSoundBuffer = NULL;
};

class GrooveCompressAudio {
	public:
	virtual int __thiscall incrementReferenceCount();
	virtual int __thiscall decrementReferenceCount();
	virtual GrooveCompressAudio* __thiscall release(bool free);
	virtual bool __thiscall install();
	virtual bool __thiscall setLooped(bool looped);
	virtual bool __thiscall reset();
	virtual bool __thiscall getFormat(GrooveCompressAudioFormat* grooveCompressAudioFormat);
	virtual int __thiscall getZero();
	virtual bool __thiscall isZero(int integer);
	virtual size_t __thiscall readData(unsigned char* destination, size_t size);
	virtual size_t __thiscall getBytesLeft();
	virtual size_t __thiscall getBytesLeft2();
};