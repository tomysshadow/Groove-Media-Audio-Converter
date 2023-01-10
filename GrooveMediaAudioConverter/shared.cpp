#include "shared.h"
#include <iostream>
#include <windows.h>

#define SHARED_OUT true, 2
#define SHARED_ERR true, 2, true, __FILE__, __LINE__

void consoleLog(const char* str, short newline, short tab, bool err, const char* file, unsigned int line) {
	if (!str) {
		str = "";
	}

	if (err) {
		for (short i = 0; i < tab; i++) {
			std::cerr << "\t";
		}

		std::cerr << str;

		if (line || !stringNullOrEmpty(file)) {
			if (!file) {
				file = "";
			}

			std::cerr << " [" << file << ":" << line << "]";
		}

		std::cerr << " (" << GetLastError() << ")";

		for (short i = 0; i < newline; i++) {
			std::cerr << std::endl;
		}
		return;
	}

	for (short i = 0; i < tab; i++) {
		std::cout << "\t";
	}

	std::cout << str;

	if (line || !stringNullOrEmpty(file)) {
		if (!file) {
			file = "";
		}

		std::cout << " [" << file << ":" << line << "]";
	}

	for (short i = 0; i < newline; i++) {
		std::cout << std::endl;
	}
}

bool readFileSafe(HANDLE file, LPVOID buffer, DWORD numberOfBytesToRead) {
	//consoleLog("Reading File Safe", SHARED_OUT);

	if (!file || file == INVALID_HANDLE_VALUE) {
		consoleLog("file must not be NULL or INVALID_HANDLE_VALUE", SHARED_ERR);
		return false;
	}

	if (!buffer) {
		consoleLog("buffer must not be NULL", SHARED_ERR);
		return false;
	}

	DWORD numberOfBytesRead = 0;

	if (!ReadFile(file, buffer, numberOfBytesToRead, &numberOfBytesRead, NULL) || numberOfBytesToRead != numberOfBytesRead) {
		consoleLog("Failed to Read File", SHARED_ERR);
		return false;
	}
	return true;
}

bool writeFileSafe(HANDLE file, LPCVOID buffer, DWORD numberOfBytesToWrite) {
	//consoleLog("Writing File Safe", SHARED_OUT);

	if (!file || file == INVALID_HANDLE_VALUE) {
		consoleLog("file must not be NULL or INVALID_HANDLE_VALUE", SHARED_ERR);
		return false;
	}

	if (!buffer) {
		consoleLog("buffer must not be NULL", SHARED_ERR);
		return false;
	}

	DWORD numberOfBytesWritten = 0;

	if (!WriteFile(file, buffer, numberOfBytesToWrite, &numberOfBytesWritten, NULL) || numberOfBytesToWrite != numberOfBytesWritten) {
		consoleLog("Failed to Write File", SHARED_ERR);
		return false;
	}
	return true;
}