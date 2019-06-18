#pragma once
#include "main.h"

const size_t GROOVE_EXTENSION_IDENTIFIER_REDIRECTION_SIZE = 64;
const size_t GROOVE_EXTENSION_IDENTIFIER_NAME_SIZE = 64;
const size_t GROOVE_EXTENSION_IDENTIFIER_COPYRIGHT_INFO_SIZE = 64;

struct GrooveExtensionIdentifier {
	int size = 0;
	ID id = DEFAULT_ID;
	wchar_t redirection[GROOVE_EXTENSION_IDENTIFIER_REDIRECTION_SIZE];
	wchar_t name[GROOVE_EXTENSION_IDENTIFIER_NAME_SIZE];
	wchar_t copyrightInfo[GROOVE_EXTENSION_IDENTIFIER_COPYRIGHT_INFO_SIZE];
	float unknown;
	float unknown2;
	bool unknown3;
	bool unknown4;
	short int unknown5;
};