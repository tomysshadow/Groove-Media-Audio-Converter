#include "main.h"
#include <string.h>
#include <ios>
#include <windows.h>
#include <atlbase.h>

bool MemoryFile::open(LPCSTR fileName) {
	file = CreateFile(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (!file || file == INVALID_HANDLE_VALUE) {
		consoleLog("Failed to Create File", true, false, true);
		return false;
	}

	LARGE_INTEGER fileSize;

	if (!GetFileSizeEx(file, &fileSize)) {
		consoleLog("Failed to Get File Size Ex", true, false, true);
		return false;
	}

	bufferSize = fileSize.QuadPart;
	buffer = new BYTE[bufferSize];

	if (!buffer) {
		consoleLog("Failed to Allocate buffer", true, false, true);
		return false;
	}

	SIZE_T numberOfBytesCopied = 0;

	if (!ReadFile(file, buffer, bufferSize, &numberOfBytesCopied, NULL) || !buffer || bufferSize != numberOfBytesCopied) {
		consoleLog("Failed to Read File", true, false, true);
		delete[] buffer;
		buffer = 0;
		return false;
	}

	read.start = buffer;
	read.filePointer = buffer;
	read.length = bufferSize;

	return true;
}

bool MemoryFile::close() {
	delete[] buffer;
	buffer = 0;
	bufferSize = 0;

	if (file || file != INVALID_HANDLE_VALUE) {
		if (!CloseHandle(file)) {
			consoleLog("Failed to Close File Handle", true, false, true);
			return false;
		}
	}

	return true;
}

int __thiscall MemoryFile::Read::incrementInstanceCount() {
	consoleLog("Incrementing Instance Count of Memory File Read");

	return ++instanceCount;
}

int __thiscall MemoryFile::Read::decrementInstanceCount() {
	consoleLog("Decrementing Instance Count of Memory File Read");

	instanceCount--;

	if (!instanceCount) {
		deleteInstance(1);
		return 0;
	}

	return instanceCount;
}

MemoryFile::Read* __thiscall MemoryFile::Read::deleteInstance(bool free) {
	consoleLog("Deleting Instance of Memory File Read");

	// TODO: what is IDA telling me here? does this inherit from std::ios_base?
	// this is what the decompilation says, but it causes an exception...
	delete this;

	/*
	if (free) {
		delete this;
	}
	*/

	return this;
}

size_t __thiscall MemoryFile::Read::getLength() {
	consoleLog("Getting Length of Memory File Read");

	if (!start) {
		return -1;
	}

	return length;
}

size_t __thiscall MemoryFile::Read::getLengthUnsafe() {
	consoleLog("Getting Length Unsafe of Memory File Read");

	return length;
}

size_t __thiscall MemoryFile::Read::getFilePos() {
	consoleLog("Getting File Pos of Memory File Read");

	if (!start) {
		return -1;
	}

	return filePointer - start;
}

size_t __thiscall MemoryFile::Read::setFilePos(size_t filePos, REL_FILE_POS relFilePos) {
	consoleLog("Setting File Pos of Memory File Read");

	if (!start) {
		return -1;
	}

	size_t filePosOld = filePointer - start;
	size_t rel = 0;

	switch (relFilePos) {
		case REL_FILE_POS_START:
		break;
		case REL_FILE_POS_RELATIVE:
		rel = filePosOld;
		break;
		case REL_FILE_POS_END:
		rel = length;
		break;
		default:
		return -1;
	}

	filePos = rel + filePos;

	if (filePos < 0 || filePos > length) {
		return -1;
	}

	filePointer = start + filePos;

	return filePosOld;
}

size_t __thiscall MemoryFile::Read::readCount(unsigned char* destination, size_t count) {
	consoleLog("Reading Count of Memory File Read");

	if (!start) {
		return -1;
	}

	size_t maxCount = start + length - filePointer;

	if (maxCount < count) {
		count = maxCount;
	}

	if (count > 0) {
		if (memcpy_s(destination, count, filePointer, count)) {
			return -1;
		}

		filePointer += count;
	}

	return count;
}

unsigned char* __thiscall MemoryFile::Read::readRange(size_t posStart, size_t posEnd) {
	consoleLog("Reading Range of Memory File Read");

	unsigned char* range = 0;

	if (!start || posEnd <= posStart || posStart > length) {
		return range;
	}

	if (posEnd != -1) {
		if (posEnd > length) {
			return range;
		}

		posEnd = length;
	}

	size_t count = posEnd - posStart;
	range = new unsigned char[count];

	if (!range) {
		consoleLog("Failed to Allocate range", true, false, true);
		return range;
	}

	if (memcpy_s(range, count, start + posStart, count)) {
		delete[] range;
		range = 0;
		return range;
	}

	return range;
}

void MemoryFile::Read::doNothing() {
	consoleLog("Doing Nothing with Memory File Read");

	return;
}

int __thiscall MemoryFile::Read::getFlags(int unknown) {
	consoleLog("Getting Flags of Memory File Read");

	if (start) {
		return 11;
	}

	return 3;
}

void MediaFactoryInterface::unknown() {
	consoleLog("Unknown of Media Factory Interface");
	return;
}

bool MediaFactoryInterface::setupExtensionCallback(unsigned char* unknown, RegisterInterface* registerInterface) {
	consoleLog("Setting Up Extension Callback of Media Factory Interface");
	return true;
}

void MediaFactoryInterface::closeExtensionCallback(ID mediaFactoryID) {
	consoleLog("Closing Extension Callback of Media Factory Interface");
	return;
}

void RegisterInterface::unknown(bool unknown) {
	consoleLog("Unknown of Register Interface");
	return;
}

bool RegisterInterface::setupExtensionCallback(ID mediaFactoryID, bool unknown, GMACodecMediaFactory* gmaCodecMediaFactory) {
	consoleLog("Setting Up Extension Callback of Register Interface");

	if (!gmaCodecMediaFactory) {
		consoleLog("GMA Codec Media Factory cannot be NULL", true, false, true);
		return false;
	}

	if (!fromFileName) {
		consoleLog("From File Name cannot be NULL", true, false, true);
		return false;
	}

	if (!toFileName) {
		consoleLog("To File Name cannot be NULL", true, false, true);
		return false;
	}

	const ID GMA_CODEC_MEDIA_FACTORY_ID = {0x6F08D354, 0x4EB31ABE, 0x3A95FFAF, 0x57684665};

	for (size_t i = 0; i < ID_SIZE; i++) {
		if (mediaFactoryID[i] != GMA_CODEC_MEDIA_FACTORY_ID[i]) {
			consoleLog("GMA Codec Media Factory ID Does Not Match", true, false, true);
			return false;
		}
	}

	MemoryFile memoryFromFile;
	ID id = DEFAULT_ID;
	Error err = 0;
	// please don't delete me
	memoryFromFile.read.instanceCount = 1;

	if (!memoryFromFile.open(fromFileName)) {
		consoleLog("Failed to Open Memory File", true, false, true);
		return false;
	}

	GrooveCompressAudio* grooveCompressAudioPointer = gmaCodecMediaFactory->createInstance(&memoryFromFile.read, id, 0, &err);

	if (err || !grooveCompressAudioPointer) {
		consoleLog("Failed to Create Instance of GMA Codec Media Factory", true, false, true);

		if (!memoryFromFile.close()) {
			consoleLog("Failed to Close Memory File", true, false, true);
		}
		return false;
	}

	GrooveCompressAudioFormat grooveCompressAudioFormat;

	if (!grooveCompressAudioPointer->getFormat(&grooveCompressAudioFormat)) {
		consoleLog("Failed to Get Format", true, false, true);
		grooveCompressAudioPointer = gmaCodecMediaFactory->destroyInstance(1);

		if (!memoryFromFile.close()) {
			consoleLog("Failed to Close Memory File", true, false, true);
		}
		return false;
	}

	CWaveFile waveFile;
	WAVEFORMATEX waveFormatEx;
	waveFormatEx.wFormatTag = WAVE_FORMAT_PCM;
	waveFormatEx.nChannels = grooveCompressAudioFormat.channels;
	waveFormatEx.nSamplesPerSec = grooveCompressAudioFormat.samplesPerSec;
	waveFormatEx.wBitsPerSample = grooveCompressAudioFormat.bitsPerSample;
	waveFormatEx.nBlockAlign = waveFormatEx.nChannels * (waveFormatEx.wBitsPerSample / 8);
	waveFormatEx.nAvgBytesPerSec = waveFormatEx.nSamplesPerSec * waveFormatEx.nBlockAlign;
	waveFormatEx.cbSize = 0;

	if (FAILED(waveFile.Open(CA2W(toFileName), &waveFormatEx, NULL))) {
		consoleLog("Failed to Open Wave File", true, false, true);
		grooveCompressAudioPointer = gmaCodecMediaFactory->destroyInstance(1);

		if (!memoryFromFile.close()) {
			consoleLog("Failed to Close Memory File", true, false, true);
		}
		return false;
	}

	UINT sizeToWrite = grooveCompressAudioFormat.size;
	// data is a terrible variable name - but I didn't choose the name, Microsoft/IBM did. And 3D Groove!
	BYTE* data = new BYTE[sizeToWrite];

	if (!data) {
		consoleLog("Failed to Allocate data", true, false, true);
		grooveCompressAudioPointer = gmaCodecMediaFactory->destroyInstance(1);

		if (!memoryFromFile.close()) {
			consoleLog("Failed to Close Memory File", true, false, true);
		}
		return false;
	}

	if (!grooveCompressAudioPointer->getData(data, sizeToWrite)) {
		consoleLog("Failed to Get Data", true, false, true);
		delete[] data;
		data = 0;
		grooveCompressAudioPointer = gmaCodecMediaFactory->destroyInstance(1);

		if (!memoryFromFile.close()) {
			consoleLog("Failed to Close Memory File", true, false, true);
		}
		return false;
	}

	UINT sizeWrote = 0;

	if (FAILED(waveFile.Write(sizeToWrite, data, &sizeWrote)) || sizeToWrite != sizeWrote) {
		consoleLog("Failed to Write Wave File", true, false, true);
		delete[] data;
		data = 0;
		grooveCompressAudioPointer = gmaCodecMediaFactory->destroyInstance(1);

		if (!memoryFromFile.close()) {
			consoleLog("Failed to Close Memory File", true, false, true);
		}
		return false;
	}

	if (FAILED(waveFile.Close())) {
		consoleLog("Failed to Close Wave File", true, false, true);
		delete[] data;
		data = 0;
		grooveCompressAudioPointer = gmaCodecMediaFactory->destroyInstance(1);

		if (!memoryFromFile.close()) {
			consoleLog("Failed to Close Memory File", true, false, true);
		}
		return false;
	}

	/*
	HANDLE toFile = CreateFile(toFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (!toFile || toFile == INVALID_HANDLE_VALUE) {
		consoleLog("Failed to Create File", true, false, true);
		delete[] data;
		data = 0;
		grooveCompressAudioPointer = gmaCodecMediaFactory->destroyInstance(1);

		if (!memoryFromFile.close()) {
			consoleLog("Failed to Close Memory File", true, false, true);
		}
		return false;
	}

	DWORD numberOfBytesCopied = 0;

	if (!WriteFile(toFile, &waveFile.riffHeader, sizeof(waveFile.riffHeader), &numberOfBytesCopied, NULL) || sizeof(waveFile.riffHeader) != numberOfBytesCopied) {
		consoleLog("Failed to Write File", true, false, true);

		if (!DeleteFile(toFileName)) {
			consoleLog("Failed to Delete File", true, false, true);
		}

		delete[] data;
		data = 0;
		grooveCompressAudioPointer = gmaCodecMediaFactory->destroyInstance(1);

		if (!memoryFromFile.close()) {
			consoleLog("Failed to Close Memory File", true, false, true);
		}
		return false;
	}
	*/

	grooveCompressAudioPointer = gmaCodecMediaFactory->destroyInstance(1);

	if (!memoryFromFile.close()) {
		consoleLog("Failed to Close Memory File", true, false, true);
		delete[] data;
		data = 0;
		return false;
	}

	delete[] data;
	data = 0;
	return true;
};

void RegisterInterface::closeExtensionCallback() {
	consoleLog("Closing Extension Callback of Media Factory Interface");
	return;
}

typedef float(_cdecl *_Identify)(GrooveExtensionIdentifier*);
typedef bool(*_Install)();
typedef bool(_cdecl *_SetupExtension)(int, MediaFactoryInterface*, RegisterInterface*, unsigned char*);
typedef bool(_cdecl *_CloseExtension)(int, MediaFactoryInterface*);

int main(int argc, char** argv) {
	consoleLog("Groove Media Audio Converter 0.9.0");
	consoleLog("By Anthony Kleine", 2);

	if (argc < 3 || stringsEqual(argv[1], "--help")) {
		help();
		return -1;
	}

	fromFileName = argv[1];
	toFileName = argv[2];
	HMODULE groovePlayerExtension = LoadLibrary("GrooveMediaAudio.dll");

	if (!groovePlayerExtension) {
		consoleLog("Failed to Load Library", true, false, true);
		return -1;
	}

	_Identify groovePlayerExtensionIdentify = (_Identify)GetProcAddress(groovePlayerExtension, "Identify");

	if (!groovePlayerExtensionIdentify) {
		consoleLog("Failed to Get Proc Address", true, false, true);
		return -1;
	}

	GrooveExtensionIdentifier grooveMediaAudioExtensionIdentifier;
	float version = groovePlayerExtensionIdentify(&grooveMediaAudioExtensionIdentifier);
	const float GROOVE_PLAYER_EXTENSION_MINIMUM_VERSION = 1.0017701;

	if (version < GROOVE_PLAYER_EXTENSION_MINIMUM_VERSION) {
		consoleLog("Unsupported Version", true, false, true);
		return -1;
	}

	if (grooveMediaAudioExtensionIdentifier.size != sizeof(grooveMediaAudioExtensionIdentifier)) {
		consoleLog("Failed to Identify Groove Media Audio Extension", true, false, true);
		return -1;
	}

	const wchar_t grooveMediaAudioExtensionIdentifierRedirection[128] = L"c32da10cae75fc4ab17d3d92d21156e2";

	if (!stringsEqualWide(grooveMediaAudioExtensionIdentifier.redirection, grooveMediaAudioExtensionIdentifierRedirection)) {
		consoleLog("Missing Required Groove Player Extension (Groove Media Audio)", true, false, true);
		return -1;
	}

	_Install groovePlayerExtensionInstall = (_Install)GetProcAddress(groovePlayerExtension, "Install");

	if (!groovePlayerExtensionInstall) {
		consoleLog("Failed to Get Proc Address after Identifying Groove Media Audio Extension", true, false, true);
		return -1;
	}

	if (!groovePlayerExtensionInstall()) {
		consoleLog("Failed to Install Groove Media Audio Extension", true, false, true);
		return -1;
	}

	_SetupExtension groovePlayerExtensionSetupExtension = (_SetupExtension)GetProcAddress(groovePlayerExtension, "SetupExtension");

	if (!groovePlayerExtensionSetupExtension) {
		consoleLog("Failed to Get Proc Address after Installing Groove Media Audio Extension", true, false, true);
		return -1;
	}

	MediaFactoryInterface mediaFactoryInterface;
	RegisterInterface registerInterface;

	if (!groovePlayerExtensionSetupExtension(0, &mediaFactoryInterface, &registerInterface, 0)) {
		consoleLog("Failed to Setup Extension", true, false, true);
		return -1;
	}

	_CloseExtension groovePlayerExtensionCloseExtension = (_CloseExtension)GetProcAddress(groovePlayerExtension, "CloseExtension");

	if (!groovePlayerExtensionCloseExtension) {
		consoleLog("Failed to Get Proc Address after Setting Up Extension", true, false, true);
		return -1;
	}

	if (!groovePlayerExtensionCloseExtension(0, &mediaFactoryInterface)) {
		consoleLog("Failed to Close Extension", true, false, true);
		return -1;
	}

	return 0;
}