#pragma once
#include <iostream>
#include <windows.h>
#include <dsound.h>
#include <SDKwavefile.h>

inline bool stringsEqual(const char* leftHandSide, const char* rightHandSide) {
	return !strcmp(leftHandSide, rightHandSide);
}

inline bool stringsEqualWide(const wchar_t* leftHandSide, const wchar_t* rightHandSide) {
	return !wcscmp(leftHandSide, rightHandSide);
}

void consoleLog(const char *buffer, int newline = true, int tab = false, bool err = false) {
	for (int i = 0; i < tab; i++) {
		consoleLog("\t", false, false, err);
	}

	if (!err) {
		std::cout << buffer;

		for (int i = 0; i < newline; i++) {
			std::cout << std::endl;
		}
	} else {
		std::cerr << buffer;

		for (int i = 0; i < newline; i++) {
			std::cerr << std::endl;
		}
	}
}

void help() {
		consoleLog("Converts the Groove Media Audio (GMA) codec used by 3D Groove GX to PCM.", 2, 1);

		consoleLog("Groove Media Audio is a proprietary audio codec by OTOY with 2x better compression than MP3. The codec may be used in WAVE Audio Files.", 2, 1);

		consoleLog("Requires the Groove Media Audio Extension.", 2, 1);

	consoleLog("Usage: GrooveMediaAudioConverter runaway.WAV runaway_converted.WAV");
}

const size_t ID_SIZE = 4;
typedef int ID[ID_SIZE];
typedef int Error;

#define DEFAULT_ID {0x00000000, 0x00000000, 0x00000000, 0x000000000};

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
	SIZE_T bufferSize;
	BYTE* buffer;
	Read read;
};

struct RegisterInterface;

struct MediaFactoryInterface {
	virtual void __thiscall unknown();
	virtual bool __thiscall setupExtensionCallback(unsigned char* unknown, RegisterInterface* registerInterface);
	virtual void __thiscall closeExtensionCallback(ID mediaFactoryID);
};

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

struct GMACodecMediaFactory {
	virtual GrooveCompressAudio* __thiscall destroyInstance(bool flags);
	virtual GrooveCompressAudio* __thiscall createInstance(MemoryFile::Read* file, ID id, int zero, Error* err);
};

struct RegisterInterface {
	virtual void __thiscall unknown(bool unknown);
	virtual bool __thiscall setupExtensionCallback(ID mediaFactoryID, bool unknown, GMACodecMediaFactory* gmaCodecMediaFactory);
	virtual void __thiscall closeExtensionCallback();
};

struct GrooveExtensionIdentifier {
	int size = 0;
	ID id = DEFAULT_ID;
	wchar_t redirection[64];
	wchar_t name[64];
	wchar_t copyrightInfo[64];
	float unknown;
	float unknown2;
	bool unknown3;
	bool unknown4;
	short int unknown5;
};

/*
typedef int32_t ChunkID;
const int16_t AUDIO_FORMAT_PCM = 1;

struct WAVEFile {
	struct RIFFHeader {
		const ChunkID chunkID = 0x52494646;
		int32_t size = 0;
		ChunkID format = 0x57415645;
	};


	struct FormatChunk {
		const ChunkID chunkID = 0x666d7420;
		int32_t size = 16;
		int16_t audioFormat = AUDIO_FORMAT_PCM;
		int16_t numChannels = 1;
		int32_t sampleRate = 44100;
		int32_t byteRate = sampleRate * numChannels * (bitsPerSample / 8);
		int16_t blockAlign = numChannels * (bitsPerSample / 8);
		int16_t bitsPerSample = 16;
	};

	struct DataChunk {
		const ChunkID chunkID = 0x64617461;
		int32_t size = 0;
		unsigned char* data = 0;
	};

	RIFFHeader riffHeader;
	FormatChunk formatChunk;
	DataChunk dataChunk;
};
*/

LPCSTR fromFileName;
LPCSTR toFileName;