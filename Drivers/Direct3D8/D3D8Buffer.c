#include <System/Log.h>
#include <Render/Buffer.h>

#include "D3D8Drv.h"

#ifdef _XBOX
#	define D3DFMT_INDEX32	0
#endif

struct Buffer *
D3D8_CreateBuffer(const struct BufferCreateInfo *bci)
{
	DWORD usage;
	HRESULT hr;

	struct Buffer *b = calloc(1, sizeof(*b));
	if (!b)
		return NULL;

	switch (bci->usage) {
	case BU_STATIC: usage = D3DUSAGE_WRITEONLY; break;
	case BU_DYNAMIC: usage = D3DUSAGE_DYNAMIC; break;
	}

	b->type = bci->type;
	b->size = bci->size;

	switch (b->type) {
	case BT_VERTEX_BUFFER: {
		hr = IDirect3DDevice8_CreateVertexBuffer(D3D8_device, b->size, usage,
												0, D3DPOOL_DEFAULT, &b->vtxBuff);
	} break;
	case BT_INDEX_BUFFER: {
		D3DFORMAT indexFormat = bci->indexType == IT_UINT_16 ? D3DFMT_INDEX16 : D3DFMT_INDEX32;
		hr = IDirect3DDevice8_CreateIndexBuffer(D3D8_device, b->size, usage,
											indexFormat, D3DPOOL_DEFAULT, &b->idxBuff);
	} break;
	}

	if (FAILED(hr)) {
		Sys_LogEntry(D3D8DRV_MOD, LOG_CRITICAL, "Failed to create buffer");
		free(b);
		return NULL;
	}

	if (bci->data)
		D3D8_UpdateBuffer(b, bci->data, bci->dataOffset, bci->dataSize);
	
	return b;
}

void
D3D8_UpdateBuffer(struct Buffer *b, const void *data, uint32_t offset, uint32_t dataSize)
{
	uint8_t *ptr;
	HRESULT hr;

	switch (b->type) {
	case BT_VERTEX_BUFFER: hr = IDirect3DVertexBuffer8_Lock(b->vtxBuff, 0, b->size, &ptr, 0); break;
	case BT_INDEX_BUFFER: hr = IDirect3DIndexBuffer8_Lock(b->idxBuff, 0, b->size, &ptr, 0); break;
	}

	if (FAILED(hr)) {
		Sys_LogEntry(D3D8DRV_MOD, LOG_CRITICAL, "Failed to lock vertex buffer");
		return;
	}

	memcpy(ptr + offset, data, dataSize);

	switch (b->type) {
	case BT_VERTEX_BUFFER: hr = IDirect3DVertexBuffer8_Unlock(b->vtxBuff); break;
	case BT_INDEX_BUFFER: hr = IDirect3DIndexBuffer8_Unlock(b->idxBuff); break;
	}
}

void *
D3D8_LockBuffer(struct Buffer *b)
{
	uint8_t *ptr = NULL;

	switch (b->type) {
	case BT_VERTEX_BUFFER: IDirect3DVertexBuffer8_Lock(b->vtxBuff, 0, b->size, &ptr, 0); break;
	case BT_INDEX_BUFFER: IDirect3DIndexBuffer8_Lock(b->idxBuff, 0, b->size, &ptr, 0); break;
	}

	return ptr;
}

void
D3D8_UnlockBuffer(struct Buffer *b)
{
	switch (b->type) {
	case BT_VERTEX_BUFFER: IDirect3DVertexBuffer8_Unlock(b->vtxBuff); break;
	case BT_INDEX_BUFFER: IDirect3DIndexBuffer8_Unlock(b->idxBuff); break;
	}
}

void
D3D8_DestroyBuffer(struct Buffer *b)
{
	switch (b->type) {
	case BT_VERTEX_BUFFER: IDirect3DVertexBuffer8_Release(b->vtxBuff); break;
	case BT_INDEX_BUFFER: IDirect3DIndexBuffer8_Release(b->idxBuff); break;
	}

	free(b);
}
