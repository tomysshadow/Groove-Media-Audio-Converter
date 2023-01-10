#include "RegisterInterface.h"
#include "MemoryFile.h"
#include "GrooveCompressAudio.h"
#include <string>
#include <windows.h>
#include <atlbase.h>
#include <dsound.h>
#include <SDKwavefile.h>

#define REGISTER_INTERFACE_OUT true, 1
#define REGISTER_INTERFACE_ERR true, 1, true, __FILE__, __LINE__

RegisterInterface::RegisterInterface(std::string fromFileName, std::string toFileName) : fromFileName(fromFileName), toFileName(toFileName) {
}

void RegisterInterface::unknown(bool unknown) {
}

bool RegisterInterface::setupExtensionCallback(ID mediaFactoryID, bool unknown, GMACodecMediaFactory* gmaCodecMediaFactory) {
	consoleLog("Setting Up Register Interface Extension Callback", REGISTER_INTERFACE_OUT);

	if (!gmaCodecMediaFactory) {
		consoleLog("gmaCodecMediaFactory must not be NULL", REGISTER_INTERFACE_ERR);
		return false;
	}

	const ID GMA_CODEC_MEDIA_FACTORY_ID = { 0x6F08D354, 0x4EB31ABE, 0x3A95FFAF, 0x57684665 };

	for (size_t i = 0; i < ID_SIZE; i++) {
		if (mediaFactoryID[i] != GMA_CODEC_MEDIA_FACTORY_ID[i]) {
			consoleLog("GMA Codec Media Factory ID Mismatch", REGISTER_INTERFACE_ERR);
			return false;
		}
	}

	bool result = false;

	// be sure to increment the reference count
	MemoryFile memoryFile;
	memoryFile.read.incrementReferenceCount();

	if (!memoryFile.open(fromFileName.c_str())) {
		consoleLog("Failed to Open Memory File", REGISTER_INTERFACE_ERR);
		goto error;
	}

	int err = 0;

	ID id = {};
	GrooveCompressAudio* grooveCompressAudioPointer = gmaCodecMediaFactory->create(&memoryFile.read, id, 0, &err);

	if (!grooveCompressAudioPointer) {
		consoleLog("grooveCompressAudioPointer must not be NULL", REGISTER_INTERFACE_ERR);
		goto error2;
	}

	if (err) {
		consoleLog("Failed to Create GMA Codec Media Factory Instance", REGISTER_INTERFACE_ERR);
		goto error3;
	}

	SIZE_T dataSize = 0;
	PBYTE data = NULL;

	SIZE_T toFileNameSize = 0;
	PCHAR _toFileName = NULL;

	{
		GrooveCompressAudioFormat grooveCompressAudioFormat;

		if (!grooveCompressAudioPointer->getFormat(&grooveCompressAudioFormat)) {
			consoleLog("Failed to Get Format", REGISTER_INTERFACE_ERR);
			goto error3;
		}

		dataSize = grooveCompressAudioFormat.dataSize;
		data = new BYTE[dataSize];

		if (!data) {
			consoleLog("Failed to Allocate data", REGISTER_INTERFACE_ERR);
			goto error3;
		}

		if (!grooveCompressAudioPointer->readData(data, dataSize)) {
			consoleLog("Failed to Read Data", REGISTER_INTERFACE_ERR);
			goto error4;
		}

		{
			CWaveFile waveFile = {};

			SIZE_T toFileNameSize = toFileName.size() + 1;
			PCHAR _toFileName = new CHAR[toFileNameSize];

			if (!_toFileName) {
				consoleLog("Failed to Allocate toFileName", REGISTER_INTERFACE_ERR);
				goto error4;
			}

			if (strncpy_s(_toFileName, toFileNameSize, toFileName.c_str(), toFileNameSize)) {
				consoleLog("Failed to Copy String Maximum", REGISTER_INTERFACE_ERR);
				goto error5;
			}

			{
				WAVEFORMATEX waveFormatEx = {};
				waveFormatEx.wFormatTag = WAVE_FORMAT_PCM;
				waveFormatEx.nChannels = grooveCompressAudioFormat.channels;
				waveFormatEx.nSamplesPerSec = grooveCompressAudioFormat.samplesPerSec;
				waveFormatEx.wBitsPerSample = grooveCompressAudioFormat.bitsPerSample;
				waveFormatEx.nBlockAlign = waveFormatEx.nChannels * (waveFormatEx.wBitsPerSample / 8);
				waveFormatEx.nAvgBytesPerSec = waveFormatEx.nSamplesPerSec * waveFormatEx.nBlockAlign;
				waveFormatEx.cbSize = 0;

				if (waveFile.Open(CA2W(_toFileName), &waveFormatEx, 0) != S_OK) {
					consoleLog("Failed to Open Wave File", REGISTER_INTERFACE_ERR);
					goto error5;
				}

				UINT sizeToWrite = dataSize;
				UINT sizeWrote = 0;

				if (waveFile.Write(sizeToWrite, data, &sizeWrote) != S_OK || sizeToWrite != sizeWrote) {
					consoleLog("Failed to Write Wave File", REGISTER_INTERFACE_ERR);
					goto error5;
				}

				if (waveFile.Close() != S_OK) {
					consoleLog("Failed to Close Wave File", REGISTER_INTERFACE_ERR);
					goto error5;
				}
			}
		}
	}

	result = true;
	error5:
	delete[] _toFileName;
	_toFileName = NULL;
	toFileNameSize = 0;
	error4:
	delete[] data;
	data = NULL;
	dataSize = 0;
	error3:
	grooveCompressAudioPointer = gmaCodecMediaFactory->destroy(true);
	error2:
	if (!memoryFile.close()) {
		consoleLog("Failed to Close Memory File", REGISTER_INTERFACE_ERR);
		result = false;
		goto error;
	}
	error:
	memoryFile.read.decrementReferenceCount();
	return true;
};

void RegisterInterface::closeExtensionCallback() {
	consoleLog("Closing Extension Callback of Media Factory Interface");
	return;
}