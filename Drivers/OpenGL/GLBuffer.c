#include <stdlib.h>

#include "GLDrv.h"

struct Buffer *
GL_CreateBuffer(const struct BufferCreateInfo *bci)
{
	struct Buffer *buff = malloc(sizeof(*buff));
	
	buff->usage = bci->usage == BU_STATIC ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
	
	switch (bci->type) {
	case BT_VERTEX_BUFFER: buff->target = GL_ARRAY_BUFFER; break;
	case BT_INDEX_BUFFER: buff->target = GL_ELEMENT_ARRAY_BUFFER; break;
	}
	
	glGenBuffers(1, &buff->id);
	if (glGetError() != GL_NO_ERROR) {
		free(buff);
		return NULL;
	}
	
	buff->size = bci->size;
	buff->indexType = bci->indexType;
	
	glBindBuffer(buff->target, buff->id);
	glBufferData(buff->target, bci->size, NULL, buff->usage);
	glBindBuffer(buff->target, 0);
	
	if (bci->data)
		GL_UpdateBuffer(buff, bci->data, bci->dataOffset, bci->dataSize);
	
	return buff;
}

void
GL_UpdateBuffer(struct Buffer *buff, const void *data, uint32_t offset, uint32_t size)
{
	glBindBuffer(buff->target, buff->id);
	glBufferSubData(buff->target, offset, size, data);
	glBindBuffer(buff->target, 0);
}

void *
GL_LockBuffer(struct Buffer *buff)
{
	glBindBuffer(buff->target, buff->id);
	return glMapBuffer(buff->target, GL_WRITE_ONLY);
}

void
GL_UnlockBuffer(struct Buffer *buff)
{
	glUnmapBuffer(buff->target);
	glBindBuffer(buff->target, 0);
}

void
GL_DestroyBuffer(struct Buffer *buff)
{
	glDeleteBuffers(1, &buff->id);
	free(buff);
}
