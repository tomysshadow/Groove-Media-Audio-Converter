#pragma once
#define _WIN32_WINNT 0x0500
#include <windows.h>

static const size_t ID_SIZE = 4;
typedef unsigned int ID[ID_SIZE];

inline bool stringNullOrEmpty(const char* str) {
	return !str || !*str;
}

inline bool stringsEqualWide(const wchar_t* leftHandSide, const wchar_t* rightHandSide) {
	return !wcscmp(leftHandSide, rightHandSide);
}

void consoleLog(const char* str = 0, short newline = true, short tab = false, bool err = false, const char* file = 0, unsigned int line = 0);
bool readFileSafe(HANDLE file, LPVOID buffer, DWORD numberOfBytesToRead);
bool writeFileSafe(HANDLE file, LPCVOID buffer, DWORD numberOfBytesToWrite);