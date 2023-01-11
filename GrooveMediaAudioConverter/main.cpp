#include "shared.h"
#include "GrooveExtensionIdentifier.h"
#include "MediaFactoryInterface.h"
#include "RegisterInterface.h"
#include <string>
#include <windows.h>

#define MAIN_OUT 2
#define MAIN_ERR true, false, true, __FILE__, __LINE__

typedef float(_cdecl *_Identify)(GrooveExtensionIdentifier*);
typedef bool(*_Install)();
typedef bool(_cdecl *_SetupExtension)(int, MediaFactoryInterface*, RegisterInterface*, unsigned char*);
typedef bool(_cdecl *_CloseExtension)(int, MediaFactoryInterface*);

void help() {
		consoleLog("Converts the Groove Media Audio (GMA) codec used by 3D Groove GX to PCM.", 2, true);

		consoleLog("Groove Media Audio is a proprietary audio codec by OTOY with 2x better compression than MP3. The codec may be used in WAVE Audio Files.", 2, true);

		consoleLog("Requires the Groove Media Audio Extension (GrooveMediaAudio.dll).", 2, true);

	consoleLog("Usage: GrooveMediaAudioConverter runaway.WAV runaway_converted.WAV", true);
}

int main(int argc, char** argv) {
	consoleLog("Groove Media Audio Converter 1.1.1");
	consoleLog("By Anthony Kleine", 2);

	const int MIN_ARGC = 3;

	int result = -1;

	if (argc < MIN_ARGC) {
		help();
		goto error;
	}

	{
		std::string arg = "";

		// loop through the optional arguments
		for (int i = MIN_ARGC; i < argc; i++) {
			arg = argv[i];

			// arguments without a value following them
			if (arg == "-h" || arg == "--help") {
				help();
				result = 0;
				goto error;
			}
		}
	}

	HMODULE groovePlayerExtensionModule = LoadLibrary("GrooveMediaAudio.DLL");

	if (!groovePlayerExtensionModule) {
		consoleLog("Failed to Load Library", MAIN_ERR);
		goto error;
	}

	_Identify groovePlayerExtensionIdentify = (_Identify)GetProcAddress(groovePlayerExtensionModule, "Identify");

	if (!groovePlayerExtensionIdentify) {
		consoleLog("Failed to Get Proc Address", MAIN_ERR);
		goto error;
	}

	{
		const float GROOVE_PLAYER_EXTENSION_MINIMUM_VERSION = 1.0017701f;

		GrooveExtensionIdentifier grooveMediaAudioExtensionIdentifier;
		float version = groovePlayerExtensionIdentify(&grooveMediaAudioExtensionIdentifier);

		if (version < GROOVE_PLAYER_EXTENSION_MINIMUM_VERSION) {
			consoleLog("Unsupported Version of Groove Media Audio Extension", MAIN_ERR);
			goto error;
		}

		const size_t GROOVE_MEDIA_AUDIO_EXTENSION_IDENTIFIER_SIZE = sizeof(grooveMediaAudioExtensionIdentifier);

		if (grooveMediaAudioExtensionIdentifier.length != GROOVE_MEDIA_AUDIO_EXTENSION_IDENTIFIER_SIZE) {
			consoleLog("Failed to Identify Groove Media Audio Extension", MAIN_ERR);
			goto error;
		}

		const wchar_t grooveMediaAudioExtensionIdentifierRedirection[GROOVE_EXTENSION_IDENTIFIER_REDIRECTION_SIZE] = L"c32da10cae75fc4ab17d3d92d21156e2";

		if (!stringsEqualWide(grooveMediaAudioExtensionIdentifier.redirection, grooveMediaAudioExtensionIdentifierRedirection)) {
			consoleLog("Missing Required Groove Player Extension", MAIN_ERR);
			goto error;
		}

		_Install groovePlayerExtensionInstall = (_Install)GetProcAddress(groovePlayerExtensionModule, "Install");

		if (!groovePlayerExtensionInstall) {
			consoleLog("Failed to Get Proc Address", MAIN_ERR);
			goto error;
		}

		if (!groovePlayerExtensionInstall()) {
			consoleLog("Failed to Install Groove Media Audio Extension", MAIN_ERR);
			goto error;
		}

		_SetupExtension groovePlayerExtensionSetupExtension = (_SetupExtension)GetProcAddress(groovePlayerExtensionModule, "SetupExtension");

		if (!groovePlayerExtensionSetupExtension) {
			consoleLog("Failed to Get Proc Address", MAIN_ERR);
			goto error;
		}

		{
			MediaFactoryInterface mediaFactoryInterface;
			RegisterInterface registerInterface(argv[1], argv[2]);

			if (!groovePlayerExtensionSetupExtension(0, &mediaFactoryInterface, &registerInterface, 0)) {
				consoleLog("Failed to Setup Extension", MAIN_ERR);
				goto error;
			}

			_CloseExtension groovePlayerExtensionCloseExtension = (_CloseExtension)GetProcAddress(groovePlayerExtensionModule, "CloseExtension");

			if (!groovePlayerExtensionCloseExtension) {
				consoleLog("Failed to Get Proc Address", MAIN_ERR);
				goto error;
			}

			if (!groovePlayerExtensionCloseExtension(0, &mediaFactoryInterface)) {
				consoleLog("Failed to Close Extension", MAIN_ERR);
				goto error;
			}
		}
	}

	result = 0;
	error:
	return result;
}

// "Jesus replied 'Love the Lord your God with all your heart and with all your soul and with all your mind.'" - Matthew 22:37