#ifndef _RE_TEXTURE_H_
#define _RE_TEXTURE_H_

#include <Render/Types.h>
#include <Render/Device.h>

struct TextureCreateInfo
{
	enum TextureType type;
	enum TextureFormat format;
	uint32_t width, height;
	uint32_t levels;

	void *data;
	uint32_t dataSize;
	bool keepData;
};

static inline struct Texture *Re_CreateTexture(const struct TextureCreateInfo *tci) { return Re_deviceProcs.CreateTexture(tci); }
static inline void Re_UpdateTexture(struct Texture *tex, const void *data) { Re_deviceProcs.UpdateTexture(tex, data); }
static inline void Re_DestroyTexture(struct Texture *tex) { Re_deviceProcs.DestroyTexture(tex); }

bool Re_CreateTextureResource(const char *name, const struct TextureCreateInfo *ci, struct Texture **tex, Handle h);
bool Re_LoadTextureResource(struct ResourceLoadInfo *li, const char *args, struct Texture **tex, Handle h);
void Re_UnloadTextureResource(struct Texture **tex, Handle h);

#endif /* _RE_TEXTURE_H_ */
