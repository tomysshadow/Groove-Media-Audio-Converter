#pragma once
#include "shared.h"
#include "GrooveCompressAudio.h"
#include "MemoryFile.h"

class GMACodecMediaFactory {
	public:
	virtual GrooveCompressAudio* __thiscall destroy(bool free);
	virtual GrooveCompressAudio* __thiscall create(MemoryFile::Read* read, ID id, int zero, int* errPointer);
};