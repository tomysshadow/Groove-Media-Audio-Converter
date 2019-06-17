#include "RegisterInterface.h"
#include "main.h"
#include "MemoryFile.h"
#include "GrooveCompressAudio.h"
#include <windows.h>
#include <atlbase.h>
#include <dsound.h>
#include <SDKwavefile.h>

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

	const ID GMA_CODEC_MEDIA_FACTORY_ID = { 0x6F08D354, 0x4EB31ABE, 0x3A95FFAF, 0x57684665 };

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

	if (!memoryFromFile.close()) {
		consoleLog("Failed to Close Memory File", true, false, true);
		delete[] data;
		data = 0;
		grooveCompressAudioPointer = gmaCodecMediaFactory->destroyInstance(1);
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
		delete[] data;
		data = 0;
		grooveCompressAudioPointer = gmaCodecMediaFactory->destroyInstance(1);
		return false;
	}

	UINT sizeWrote = 0;

	if (FAILED(waveFile.Write(sizeToWrite, data, &sizeWrote)) || sizeToWrite != sizeWrote) {
		consoleLog("Failed to Write Wave File", true, false, true);
		delete[] data;
		data = 0;
		grooveCompressAudioPointer = gmaCodecMediaFactory->destroyInstance(1);
		return false;
	}

	if (FAILED(waveFile.Close())) {
		consoleLog("Failed to Close Wave File", true, false, true);
		delete[] data;
		data = 0;
		grooveCompressAudioPointer = gmaCodecMediaFactory->destroyInstance(1);
		return false;
	}

	delete[] data;
	data = 0;
	grooveCompressAudioPointer = gmaCodecMediaFactory->destroyInstance(1);
	return true;
};

void RegisterInterface::closeExtensionCallback() {
	consoleLog("Closing Extension Callback of Media Factory Interface");
	return;
}