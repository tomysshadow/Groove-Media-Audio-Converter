#include "MemoryFile.h"
#include <windows.h>

#define MEMORY_FILE_OUT true, 1
#define MEMORY_FILE_ERR true, 1, true, __FILE__, __LINE__

bool MemoryFile::open(LPCSTR fileName) {
	consoleLog("Opening Memory File", MEMORY_FILE_OUT);

	if (!fileName) {
		consoleLog("fileName must not be NULL", MEMORY_FILE_ERR);
		return false;
	}

	file = CreateFile(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (!file || file == INVALID_HANDLE_VALUE) {
		consoleLog("Failed to Create File", MEMORY_FILE_ERR);
		return false;
	}

	LARGE_INTEGER fileSize = {};
	fileSize.QuadPart = 0;

	if (!GetFileSizeEx(file, &fileSize)) {
		consoleLog("Failed to Get File Size", MEMORY_FILE_ERR);
		return false;
	}

	if (data) {
		delete[] data;
		data = NULL;
	}

	dataSize = (SIZE_T)fileSize.QuadPart;
	data = new BYTE[dataSize];

	if (!data) {
		consoleLog("Failed to Allocate data", MEMORY_FILE_ERR);
		return false;
	}

	bool result = false;

	if (!readFileSafe(file, data, dataSize)) {
		consoleLog("Failed to Read File Safe", MEMORY_FILE_ERR);
		goto error;
	}

	read.data = data;
	read.position = data;
	read.length = dataSize;

	result = true;
	error:
	if (!result) {
		delete[] data;
		data = NULL;
		dataSize = 0;
	}
	return result;
}

bool MemoryFile::close() {
	consoleLog("Closing Memory File", MEMORY_FILE_OUT);

	if (data) {
		delete[] data;
		data = NULL;
	}

	dataSize = 0;

	if (file || file != INVALID_HANDLE_VALUE) {
		if (!CloseHandle(file)) {
			consoleLog("Failed to Close File Handle", MEMORY_FILE_ERR);
			return false;
		}
	}
	return true;
}

int __thiscall MemoryFile::Read::incrementReferenceCount() {
	consoleLog("Incrementing Read Reference Count", MEMORY_FILE_OUT);
	return ++referenceCount;
}

int __thiscall MemoryFile::Read::decrementReferenceCount() {
	consoleLog("Decrementing Read Reference Count", MEMORY_FILE_OUT);

	referenceCount--;

	if (!referenceCount) {
		release(true);
		return 0;
	}
	return referenceCount;
}

MemoryFile::Read* __thiscall MemoryFile::Read::release(bool free) {
	consoleLog("Deleting Read Instance", MEMORY_FILE_OUT);

	data = 0;
	position = 0;
	length = 0;

	if (free) {
		delete this;
		return 0;
	}
	return this;
}

size_t __thiscall MemoryFile::Read::getLength() {
	consoleLog("Getting Read Length", MEMORY_FILE_OUT);

	if (!data) {
		return -1;
	}
	return length;
}

size_t __thiscall MemoryFile::Read::getRawLength() {
	consoleLog("Getting Read Raw Length", MEMORY_FILE_OUT);
	return length;
}

size_t __thiscall MemoryFile::Read::getPosition() {
	consoleLog("Getting Read Position", MEMORY_FILE_OUT);

	if (!data) {
		return -1;
	}
	return position - data;
}

size_t __thiscall MemoryFile::Read::setPosition(size_t distance, MOVE_METHOD moveMethod) {
	consoleLog("Setting Read Position", MEMORY_FILE_OUT);

	if (!data) {
		return -1;
	}

	size_t current = position - data;

	size_t move = (moveMethod == MOVE_METHOD_BEGIN) ? 0 :
		(moveMethod == MOVE_METHOD_CURRENT) ? current :
		(moveMethod == MOVE_METHOD_END) ? length : -1;

	if (move == -1) {
		return -1;
	}

	distance += move;

	if (distance < 0 || distance > length) {
		return -1;
	}

	position = data + distance;
	return current;
}

size_t __thiscall MemoryFile::Read::readData(unsigned char* destination, size_t size) {
	consoleLog("Reading Read Data", MEMORY_FILE_OUT);

	if (!data) {
		return -1;
	}

	size_t maxSize = data + length - position;

	if (maxSize < size) {
		size = maxSize;
	}

	if (size > 0) {
		if (memcpy_s(destination, size, position, size)) {
			return -1;
		}

		position += size;
	}
	return size;
}

unsigned char* __thiscall MemoryFile::Read::readDataRange(size_t begin, size_t end) {
	consoleLog("Reading Read Data Range", MEMORY_FILE_OUT);

	unsigned char* range = 0;

	if (!data || end <= begin || begin > length) {
		return range;
	}

	if (end != -1) {
		if (end > length) {
			return range;
		}

		end = length;
	}

	size_t rangeSize = end - begin;
	range = new unsigned char[rangeSize];
	ZeroMemory(range, rangeSize);

	if (!range) {
		consoleLog("Failed to Allocate range", MEMORY_FILE_ERR);
		return range;
	}

	bool result = false;

	if (memcpy_s(range, rangeSize, data + begin, rangeSize)) {
		consoleLog("Failed to Copy Memory", MEMORY_FILE_ERR);
		goto error;
	}

	result = true;
	error:
	if (!result) {
		delete[] range;
		range = 0;
		rangeSize = 0;
	}
	return range;
}

void MemoryFile::Read::unknown() {
}

int __thiscall MemoryFile::Read::getFlags(int unknown) {
	consoleLog("Getting Read Flags", MEMORY_FILE_OUT);

	const int OPEN = 11;
	const int CLOSED = 3;

	if (data) {
		return OPEN;
	}
	return CLOSED;
}