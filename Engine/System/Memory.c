#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <Engine/Config.h>
#include <System/Log.h>
#include <System/Memory.h>
#include <System/AtomicLock.h>

#define MMOD "MemoryManager"
#define MAGIC 0xB15B00B5

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define ROUND_UP(v, powerOf2Alignment) (((v) + (powerOf2Alignment)-1) & ~((powerOf2Alignment)-1))

struct Allocation
{
	uint32_t magic;
	size_t size;
	enum MemoryHeap heap;
};

static uint8_t *_transientHeap, *_transientHeapPtr;
static uint32_t *_transientHeapSize;
static uint32_t _transientHeapPeak = 0;
static struct AtomicLock _lock = { 0, 0 };

void *
Sys_Alloc(size_t size, size_t count, enum MemoryHeap heap)
{
	void *ret = NULL;
	size_t totalSize = size * count;
	struct Allocation *alloc = NULL;

	// TODO: check overflow

	switch (heap) {
	case MH_Transient: {
		Sys_AtomicLockWrite(&_lock);

		totalSize = ROUND_UP(totalSize, 4);
		if ((_transientHeapPtr - _transientHeap) + totalSize > * _transientHeapSize) {
			assert(!"Out of transient memory");
			break;
		}
		ret = _transientHeapPtr;
		_transientHeapPtr += totalSize;

		Sys_AtomicUnlockWrite(&_lock);
	} break;
	case MH_Persistent:
	case MH_Secure: {
		ret = calloc(1, totalSize + sizeof(struct Allocation));
		if (!ret)
			return NULL;

		alloc = ret;
		alloc->magic = MAGIC;
		alloc->heap = heap;
		alloc->size = totalSize;

		ret = (uint8_t *)ret + sizeof(*alloc);
	} break;
	}

	return ret;
}

void
Sys_Free(void *mem)
{
	struct Allocation *alloc;

	if (!mem)
		return;

	alloc = (struct Allocation *)((uint8_t *)mem - (sizeof(*alloc)));
	if (alloc->magic != MAGIC) {
		Sys_LogEntry(MMOD, LOG_CRITICAL, "Attempt to free unrecognized block %p.", mem);
		return;
	}

	if (alloc->heap == MH_Secure) {
		Sys_ZeroMemory(alloc, alloc->size);
		alloc->heap = MH_Persistent;
	}

	switch (alloc->heap) {
	case MH_Persistent:
		free(alloc);
		break;
	}

	// check end
}

bool
Sys_InitMemory(void)
{
	_transientHeapSize = &E_GetCVarU32("Engine_TransientHeapSize", 6 * 1024 * 1024)->u32;

	_transientHeap = Sys_AlignedAlloc(*_transientHeapSize, 16);
	memset(_transientHeap, 0x0, *_transientHeapSize);
	
	if (!_transientHeap)
		return false;

	_transientHeapPtr = _transientHeap;

	return true;
}

void
Sys_ResetHeap(enum MemoryHeap heap)
{
	Sys_AtomicLockWrite(&_lock);

	switch (heap) {
	case MH_Transient:
		_transientHeapPeak = MAX(_transientHeapPeak, (size_t)(_transientHeapPtr - _transientHeap));

		_transientHeapPtr = _transientHeap;
		break;
	}

	Sys_AtomicUnlockWrite(&_lock);
}

void
Sys_TermMemory(void)
{
	Sys_LogEntry(MMOD, LOG_DEBUG, "Transient heap peak: %u/%u B (%.02f%%)", _transientHeapPeak, *_transientHeapSize,
		((double)_transientHeapPeak / (double)*_transientHeapSize) * 100.0);

	Sys_AlignedFree(_transientHeap);
}
