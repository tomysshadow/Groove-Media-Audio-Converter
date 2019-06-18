#include "main.h"
#include "GrooveExtensionIdentifier.h"
#include "MediaFactoryInterface.h"
#include "RegisterInterface.h"
#include <windows.h>
#include <string.h>
#include <ios>

inline bool stringsEqual(const char* leftHandSide, const char* rightHandSide) {
	return !strcmp(leftHandSide, rightHandSide);
}

inline bool stringsEqualWide(const wchar_t* leftHandSide, const wchar_t* rightHandSide) {
	return !wcscmp(leftHandSide, rightHandSide);
}

void consoleLog(const char *buffer, int newline, int tab, bool err) {
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

LPCSTR fromFileName;
LPCSTR toFileName;

typedef float(_cdecl *_Identify)(GrooveExtensionIdentifier*);
typedef bool(*_Install)();
typedef bool(_cdecl *_SetupExtension)(int, MediaFactoryInterface*, RegisterInterface*, unsigned char*);
typedef bool(_cdecl *_CloseExtension)(int, MediaFactoryInterface*);

int main(int argc, char** argv) {
	consoleLog("Groove Media Audio Converter 0.9.8");
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

	const wchar_t grooveMediaAudioExtensionIdentifierRedirection[GROOVE_EXTENSION_IDENTIFIER_REDIRECTION_SIZE] = L"c32da10cae75fc4ab17d3d92d21156e2";

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

// "Jesus replied 'Love the Lord your God with all your heart and with all your soul and with all your mind.'" - Matthew 22:37