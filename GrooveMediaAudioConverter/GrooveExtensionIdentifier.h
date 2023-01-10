#pragma once
#include "shared.h"

static const size_t GROOVE_EXTENSION_IDENTIFIER_REDIRECTION_SIZE = 64;
static const size_t GROOVE_EXTENSION_IDENTIFIER_NAME_SIZE = 64;
static const size_t GROOVE_EXTENSION_IDENTIFIER_COPYRIGHT_INFO_SIZE = 64;

struct GrooveExtensionIdentifier {
	int length = 0;
	ID id = {};
	wchar_t redirection[GROOVE_EXTENSION_IDENTIFIER_REDIRECTION_SIZE] = {};
	wchar_t name[GROOVE_EXTENSION_IDENTIFIER_NAME_SIZE] = {};
	wchar_t copyrightInfo[GROOVE_EXTENSION_IDENTIFIER_COPYRIGHT_INFO_SIZE] = {};
	float unknown = 0;
	float unknown2 = 0;
	bool unknown3 = 0;
	bool unknown4 = 0;
	short int unknown5 = 0;
};