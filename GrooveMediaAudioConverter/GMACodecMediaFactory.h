#pragma once
#include "main.h"
#include "GrooveCompressAudio.h"
#include "MemoryFile.h"

struct GMACodecMediaFactory {
	virtual GrooveCompressAudio* __thiscall destroyInstance(bool flags);
	virtual GrooveCompressAudio* __thiscall createInstance(MemoryFile::Read* file, ID id, int zero, Error* err);
};