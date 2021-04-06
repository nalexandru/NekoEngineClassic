#include <stdlib.h>

#include "GLDrv.h"

struct Texture *
GL_CreateTexture(const struct TextureCreateInfo *tci)
{
	struct Texture *tex = malloc(sizeof(*tex));
	
	tex->width = tci->width;
	tex->height = tci->height;
	
	switch (tci->type) {
	case TT_2D: tex->target = GL_TEXTURE_2D; break;
	//case TT_CUBE: tex->target = GL_TEXTURE_CUBE; break;
	default:
		free(tex);
		return NULL;
	break;
	}
	
	switch (tci->format) {
	case TF_R8G8B8:
		tex->type = GL_UNSIGNED_BYTE;
		tex->format = GL_RGB;
		tex->internalFormat = GL_RGB8;
		tex->compressed = false;
	break;
	case TF_R8G8B8A8:
		// TODO: GL_UNSIGNED_INT_8_8_8_8_REV
		tex->type = GL_UNSIGNED_BYTE;
		tex->format = GL_RGBA;
		tex->internalFormat = GL_RGBA8;
		tex->compressed = false;
	break;
	case TF_R5G6G5:
		tex->type = GL_UNSIGNED_SHORT_5_6_5;
		tex->format = GL_RGB;
		tex->internalFormat = GL_RGB;
		tex->compressed = false;
	break;
	case TF_A8:
		tex->type = GL_UNSIGNED_BYTE;
		tex->format = GL_ALPHA;
		tex->internalFormat = GL_ALPHA8;
		tex->compressed = false;
	break;
	case TF_L8:
		tex->type = GL_UNSIGNED_BYTE;
		tex->format = GL_LUMINANCE;
		tex->internalFormat = GL_LUMINANCE8;
		tex->compressed = false;
	break;
	case TF_BC1:
		tex->type = GL_UNSIGNED_BYTE;
		tex->format = GL_RGB;
		tex->internalFormat = GL_RGB;
		tex->compressed = false;
	break;
	case TF_BC2:
		tex->type = GL_UNSIGNED_BYTE;
		tex->format = GL_RGB;
		tex->internalFormat = GL_RGB;
		tex->compressed = false;
	break;
	case TF_BC3:
		tex->type = GL_UNSIGNED_BYTE;
		tex->format = GL_RGB;
		tex->internalFormat = GL_RGB;
		tex->compressed = false;
	break;
	case TF_BC4:
		tex->type = GL_UNSIGNED_BYTE;
		tex->format = GL_RGB;
		tex->internalFormat = GL_RGB;
		tex->compressed = false;
	break;
	case TF_BC5:
		tex->type = GL_UNSIGNED_BYTE;
		tex->format = GL_RGB;
		tex->internalFormat = GL_RGB;
		tex->compressed = false;
	break;
	case TF_D16:
		tex->type = GL_UNSIGNED_BYTE;
		tex->format = GL_RGB;
		tex->internalFormat = GL_RGB;
		tex->compressed = false;
	break;
	case TF_D32:
		tex->type = GL_UNSIGNED_BYTE;
		tex->format = GL_RGB;
		tex->internalFormat = GL_RGB;
		tex->compressed = false;
	break;
	case TF_D24S8:
		tex->type = GL_UNSIGNED_BYTE;
		tex->format = GL_RGB;
		tex->internalFormat = GL_RGB;
		tex->compressed = false;
	break;
	default:
		free(tex);
		return NULL;
	}
	
	glGenTextures(1, &tex->id);
	
	glBindTexture(tex->target, tex->id);
	
	glTexParameteri(tex->target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(tex->target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	//if (!strncmp(_textureFilter, "Bilinear", strlen(_textureFilter))) {
		glTexParameteri(tex->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(tex->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	/*} else if (!strncmp(_textureFilter, "Trilinear", strlen(_textureFilter))) {
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	} else if (!strncmp(_textureFilter, "Anisotropic", strlen(_textureFilter))) {
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(target, GL_TEXTURE_MAX_ANISOTROPY, (float)*_aniso);
	} else {
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}*/
		
	glBindTexture(tex->target, 0);
	
	if (tci->data)
		GL_UpdateTexture(tex, tci->data);
	
	return tex;
}

void
GL_UpdateTexture(struct Texture *tex, const void *data)
{
	glBindTexture(tex->target, tex->id);
	glTexImage2D(tex->target, 0, tex->internalFormat, tex->width, tex->height,
				 0, tex->format, tex->type, data);
	glBindTexture(tex->target, 0);
}

void
GL_DestroyTexture(struct Texture *tex)
{
	glDeleteTextures(1, &tex->id);
	free(tex);
}