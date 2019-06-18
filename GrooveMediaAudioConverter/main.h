#pragma once
#include <windows.h>

inline bool stringsEqual(const char* leftHandSide, const char* rightHandSide);
inline bool stringsEqualWide(const wchar_t* leftHandSide, const wchar_t* rightHandSide);
void consoleLog(const char *buffer, int newline = true, int tab = false, bool err = false);
void help();

const size_t ID_SIZE = 4;
typedef int ID[ID_SIZE];
typedef int Error;

#define DEFAULT_ID {0x00000000, 0x00000000, 0x00000000, 0x000000000};

extern LPCSTR fromFileName;
extern LPCSTR toFileName;