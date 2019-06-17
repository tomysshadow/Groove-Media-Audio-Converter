#pragma once
#include "main.h"

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