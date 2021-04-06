#ifndef _RE_BUFFER_H_
#define _RE_BUFFER_H_

#include <Render/Types.h>
#include <Render/Device.h>

struct BufferCreateInfo
{
	uint32_t size;
	enum BufferType type;
	enum BufferUsage usage;

	enum IndexType indexType;
	
	void *data;
	uint32_t dataOffset, dataSize;
	bool keepData;
};

static inline void Re_BindIndexBuffer(struct Buffer *buff) { Re_deviceProcs.BindIndexBuffer(buff); }
static inline void Re_BindVertexBuffer(struct Buffer *buff) { Re_deviceProcs.BindVertexBuffer(buff); }
static inline void Re_BindTexture(Handle texture, uint32_t unit) { Re_deviceProcs.BindTexture(texture, unit); }

static inline struct Buffer *Re_CreateBuffer(const struct BufferCreateInfo *bci) { return Re_deviceProcs.CreateBuffer(bci); }
static inline void Re_UpdateBuffer(struct Buffer *buff, const void *data, uint32_t offset, uint32_t size) { Re_deviceProcs.UpdateBuffer(buff, data, offset, size); }
static inline void *Re_LockBuffer(struct Buffer *buff) { return Re_deviceProcs.LockBuffer(buff); }
static inline void Re_UnlockBuffer(struct Buffer *buff) { Re_deviceProcs.UnlockBuffer(buff); }
static inline void Re_DestroyBuffer(struct Buffer *buff) { Re_deviceProcs.DestroyBuffer(buff); }

#endif /* _RE_BUFFER_H_ */
