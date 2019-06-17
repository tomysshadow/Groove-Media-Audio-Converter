#pragma once
#include <windows.h>
#include <dsound.h>

struct GrooveCompressAudioFormat {
	WORD channels = 1;
	WORD bitsPerSample = 16;
	DWORD samplesPerSec = 44100;
	DWORD size = 0;
	LPLPDIRECTSOUNDBUFFER directSoundBuffer = 0;
};

struct GrooveCompressAudio {
	virtual int __thiscall incrementInstanceCount();
	virtual int __thiscall decrementInstanceCount();
	virtual unsigned char* __thiscall deleteInstance(bool free);
	virtual bool __thiscall install();
	virtual bool __thiscall setLooped(bool looped);
	virtual bool __thiscall reset();
	virtual bool __thiscall getFormat(GrooveCompressAudioFormat* grooveCompressAudioFormat);
	virtual int __thiscall getZero();
	virtual bool __thiscall isZero(int integer);
	virtual size_t __thiscall getData(unsigned char* destination, size_t count);
	virtual size_t __thiscall getBytesAvailable();
	virtual size_t __thiscall getBytesAvailable2();
};