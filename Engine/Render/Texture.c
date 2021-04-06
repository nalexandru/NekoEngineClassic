#include <stdlib.h>

#include <Engine/IO.h>
#include <Engine/Asset.h>
#include <Engine/Config.h>
#include <Engine/Resource.h>
#include <Render/Render.h>
#include <Render/Texture.h>

bool
Re_CreateTextureResource(const char *name, const struct TextureCreateInfo *ci, struct Texture **tex, Handle h)
{
	*tex = Re_CreateTexture(ci);
	if (!*tex)
		return false;

	if (!ci->keepData)
		free(ci->data);

	return true;
}

bool
Re_LoadTextureResource(struct ResourceLoadInfo *li, const char *args, struct Texture **tex, Handle h)
{
	bool rc = false;
	struct TextureCreateInfo ci = { TT_2D };

	if (strstr(li->path, ".dds"))
		rc = false;
	else if (strstr(li->path, ".tga"))
		rc = E_LoadTGAAsset(&li->stm, &ci);
	else
		rc = E_LoadImageAsset(&li->stm, &ci);

	if (rc)
		*tex = Re_CreateTexture(&ci);

	free(ci.data);

	return *tex != NULL;
}

void
Re_UnloadTextureResource(struct Texture **tex, Handle h)
{
	Re_DestroyTexture(*tex);
}
