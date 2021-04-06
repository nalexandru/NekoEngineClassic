#include <System/Endian.h>
#include <Render/Texture.h>

#include "D3D8Drv.h"

#ifdef _XBOX
#	define D3DFMT_R8G8B8	0
#	define D3DFMT_D32		0
#	define D3DLOCK_DISCARD	0
#endif

static D3DFORMAT NeToD3DTextureFormat[] =
{
	D3DFMT_R8G8B8,
	D3DFMT_A8R8G8B8,
	D3DFMT_R5G6B5,
	D3DFMT_A8,
	D3DFMT_L8,
	D3DFMT_DXT1,
	D3DFMT_DXT2,
	D3DFMT_DXT3,
	D3DFMT_DXT4,
	D3DFMT_DXT5,
	D3DFMT_D16,
	D3DFMT_D32,
	D3DFMT_D24S8
};

static uint8_t NeToD3DTextureFormatSize[] =
{
	3,
	4,
	2,
	1,
	1,
	0, // dxt1
	0, // dxt2
	0, // dxt3
	0, // dxt4
	0, // dxt5
	2,
	4,
	4
};

struct Texture *
D3D8_CreateTexture(const struct TextureCreateInfo *tci)
{
	HRESULT hr;
	DWORD usage = 0;
	struct Texture *tex = malloc(sizeof(*tex));
	if (!tex)
		return NULL;

	tex->width = tci->width;
	tex->height = tci->height;
	tex->levels = tci->levels;
	tex->format = tci->format;

	hr = IDirect3DDevice8_CreateTexture(D3D8_device, tex->width, tex->height, tex->levels,
		usage, NeToD3DTextureFormat[tex->format], D3DPOOL_MANAGED, &tex->tex);
	if (FAILED(hr)) {
		free(tex);
		return NULL;
	}
	
	if (tci->data)
		D3D8_UpdateTexture(tex, tci->data);

	return tex;
}

void
D3D8_UpdateTexture(struct Texture *tex, const void *data)
{
	size_t size = 0;
	uint32_t *p = NULL;
	D3DLOCKED_RECT rect;

	IDirect3DTexture8_LockRect(tex->tex, 0, &rect, NULL, D3DLOCK_DISCARD);

	if (tex->format == TF_R8G8B8A8) {
		size_t i;
		size = tex->width * tex->height;
		p = malloc(size * 4);

		for (i = 0; i < size; ++i)
			p[i] = Sys_SwapUint32(((uint32_t *)data)[i]) >> 8 | 0xFF000000;

		data = p;
	}

	size = tex->width * tex->height * NeToD3DTextureFormatSize[tex->format];
	memcpy(rect.pBits, data, size);

	IDirect3DTexture8_UnlockRect(tex->tex, 0);

	free(p);
}

void
D3D8_DestroyTexture(struct Texture *tex)
{
	IDirect3DTexture8_Release(tex->tex);
	free(tex);
}