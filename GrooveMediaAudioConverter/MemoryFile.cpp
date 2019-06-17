#include "MemoryFile.h"
#include "main.h"
#include <windows.h>

bool MemoryFile::open(LPCSTR fileName) {
	file = CreateFile(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (!file || file == INVALID_HANDLE_VALUE) {
		consoleLog("Failed to Create File", true, false, true);
		return false;
	}

	LARGE_INTEGER fileSize;

	if (!GetFileSizeEx(file, &fileSize)) {
		consoleLog("Failed to Get File Size Ex", true, false, true);
		return false;
	}

	if (buffer) {
		delete[] buffer;
		buffer = NULL;
	}

	bufferSize = fileSize.QuadPart;
	buffer = new BYTE[bufferSize];

	if (!buffer) {
		consoleLog("Failed to Allocate buffer", true, false, true);
		return false;
	}

	SIZE_T numberOfBytesCopied = 0;

	if (!ReadFile(file, buffer, bufferSize, &numberOfBytesCopied, NULL) || !buffer || bufferSize != numberOfBytesCopied) {
		consoleLog("Failed to Read File", true, false, true);
		delete[] buffer;
		buffer = NULL;
		return false;
	}

	read.start = buffer;
	read.filePointer = buffer;
	read.length = bufferSize;

	return true;
}

bool MemoryFile::close() {
	if (buffer) {
		delete[] buffer;
		buffer = NULL;
	}

	bufferSize = 0;

	if (file || file != INVALID_HANDLE_VALUE) {
		if (!CloseHandle(file)) {
			consoleLog("Failed to Close File Handle", true, false, true);
			return false;
		}
	}

	return true;
}

int __thiscall MemoryFile::Read::incrementInstanceCount() {
	consoleLog("Incrementing Instance Count of Memory File Read");

	return ++instanceCount;
}

int __thiscall MemoryFile::Read::decrementInstanceCount() {
	consoleLog("Decrementing Instance Count of Memory File Read");

	instanceCount--;

	if (!instanceCount) {
		deleteInstance(1);
		return 0;
	}

	return instanceCount;
}

MemoryFile::Read* __thiscall MemoryFile::Read::deleteInstance(bool free) {
	consoleLog("Deleting Instance of Memory File Read");

	if (free) {
		delete this;
	}

	return this;
}

size_t __thiscall MemoryFile::Read::getLength() {
	consoleLog("Getting Length of Memory File Read");

	if (!start) {
		return -1;
	}

	return length;
}

size_t __thiscall MemoryFile::Read::getLengthUnsafe() {
	consoleLog("Getting Length Unsafe of Memory File Read");

	return length;
}

size_t __thiscall MemoryFile::Read::getFilePos() {
	consoleLog("Getting File Pos of Memory File Read");

	if (!start) {
		return -1;
	}

	return filePointer - start;
}

size_t __thiscall MemoryFile::Read::setFilePos(size_t filePos, REL_FILE_POS relFilePos) {
	consoleLog("Setting File Pos of Memory File Read");

	if (!start) {
		return -1;
	}

	size_t filePosOld = filePointer - start;
	size_t rel = 0;

	switch (relFilePos) {
		case REL_FILE_POS_START:
		break;
		case REL_FILE_POS_RELATIVE:
		rel = filePosOld;
		break;
		case REL_FILE_POS_END:
		rel = length;
		break;
		default:
		return -1;
	}

	filePos = rel + filePos;

	if (filePos < 0 || filePos > length) {
		return -1;
	}

	filePointer = start + filePos;

	return filePosOld;
}

size_t __thiscall MemoryFile::Read::readCount(unsigned char* destination, size_t count) {
	consoleLog("Reading Count of Memory File Read");

	if (!start) {
		return -1;
	}

	size_t maxCount = start + length - filePointer;

	if (maxCount < count) {
		count = maxCount;
	}

	if (count > 0) {
		if (memcpy_s(destination, count, filePointer, count)) {
			return -1;
		}

		filePointer += count;
	}

	return count;
}

unsigned char* __thiscall MemoryFile::Read::readRange(size_t posStart, size_t posEnd) {
	consoleLog("Reading Range of Memory File Read");

	unsigned char* range = 0;

	if (!start || posEnd <= posStart || posStart > length) {
		return range;
	}

	if (posEnd != -1) {
		if (posEnd > length) {
			return range;
		}

		posEnd = length;
	}

	size_t count = posEnd - posStart;
	range = new unsigned char[count];

	if (!range) {
		consoleLog("Failed to Allocate range", true, false, true);
		return range;
	}

	if (memcpy_s(range, count, start + posStart, count)) {
		delete[] range;
		range = 0;
		return range;
	}

	return range;
}

void MemoryFile::Read::doNothing() {
	consoleLog("Doing Nothing with Memory File Read");

	return;
}

int __thiscall MemoryFile::Read::getFlags(int unknown) {
	consoleLog("Getting Flags of Memory File Read");

	if (start) {
		return 11;
	}

	return 3;
}