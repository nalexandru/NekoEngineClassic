#include <System/Log.h>
#include <Runtime/Runtime.h>
#include <Scene/Scene.h>
#include <Engine/Component.h>

#include "ECS.h"

static int64_t _next_handle;
static Array _component_types;

#define COMP_MOD	"Component"

bool E_LoadComponents(void);

static int _CompType_cmp(const void *item, const void *data);
static int _CompHandle_cmp(const void *item, const void *data);
static inline struct CompHandleData *_HandlePtr(struct Scene *s, CompHandle comp);

CompHandle
E_CreateComponentS(struct Scene *s, const char *typeName, EntityHandle owner, const void **args)
{
	CompTypeId id = 0;
	Array *a = NULL;
	struct CompBase *comp = NULL;
	struct CompType *type = NULL;
	struct CompHandleData *handle = NULL;

	id = E_ComponentTypeId(typeName);
	if (id == RT_NOT_FOUND) {
		Sys_LogEntry(COMP_MOD, LOG_CRITICAL, "Type [%s] does not exist", typeName);
		return ES_INVALID_COMPONENT;
	}
	type = Rt_ArrayGet(&_component_types, id);

	a = Rt_ArrayGet(&s->compData, id);
	if (!a) {
		Sys_LogEntry(COMP_MOD, LOG_CRITICAL, "Data for type [%s] does not exist", typeName);
		return ES_INVALID_COMPONENT;
	}

	comp = Rt_ArrayAllocate(a);
	if (!comp) {
		Sys_LogEntry(COMP_MOD, LOG_CRITICAL, "Failed to allocate component of type [%s]", typeName);
		return ES_INVALID_COMPONENT;
	}

	handle = Rt_ArrayAllocate(&s->compHandle);
	if (!handle) {
		Sys_LogEntry(COMP_MOD, LOG_CRITICAL,
			"Failed to allocate component handle of type [%s]", typeName);
		return ES_INVALID_COMPONENT;
	}

	handle->type = id;
	handle->handle = _next_handle++;
	handle->index = a->count - 1;

	comp->_owner = owner;
	comp->_self = handle;

	if (type->create && !type->create(comp, args)) {
		--a->count;
		--_next_handle;
		--s->compHandle.count;
		Sys_LogEntry(COMP_MOD, LOG_CRITICAL, "Failed to create component of type [%s]", typeName);
		return ES_INVALID_COMPONENT;
	}

	return handle->handle;
}

CompHandle
E_CreateComponentIdS(struct Scene *s, CompTypeId id, EntityHandle owner, const void **args)
{
	Array *a = NULL;
	struct CompType *type = NULL;
	struct CompBase *comp = NULL;
	struct CompHandleData *handle = NULL;

	type = Rt_ArrayGet(&_component_types, id);
	if (!type)
		return -1;

	a = Rt_ArrayGet(&s->compData, id);
	if (!a)
		return -1;

	comp = Rt_ArrayAllocate(a);
	if (!comp)
		return -1;

	handle = Rt_ArrayAllocate(&s->compHandle);
	if (!handle)
		return ES_INVALID_COMPONENT;

	handle->type = id;
	handle->handle = _next_handle++;
	handle->index = a->count - 1;

	comp->_owner = owner;
	comp->_self = handle;

	if (type->create && !type->create(comp, args)) {
		--a->count;
		--_next_handle;
		--s->compHandle.count;
		return ES_INVALID_COMPONENT;
	}

	return handle->handle;
}

void
E_DestroyComponentS(struct Scene *s, CompHandle comp)
{
	struct CompHandleData *handle = NULL;
	struct CompType *type = NULL;
	uint8_t *dst = NULL, *src = NULL;
	size_t dst_index = 0;
	Array *a = NULL;

	handle = _HandlePtr(s, comp);
	if (!handle)
		return;

	type = Rt_ArrayGet(&_component_types, handle->type);
	if (!type)
		return;

	a = Rt_ArrayGet(&s->compData, handle->type);
	if (!a)
		return;

	dst_index = handle->index;

	src = Rt_ArrayLast(a);
	dst = Rt_ArrayGet(a, dst_index);

	if (type->destroy)
		type->destroy(dst);

	memcpy(dst, src, a->elem_size);

	handle = ((struct CompBase *)dst)->_self;
	handle->index = dst_index;

	--a->count;
}

void *
E_ComponentPtrS(struct Scene *s, CompHandle comp)
{
	Array *a = NULL;
	struct CompHandleData *handle = NULL;

	handle = _HandlePtr(s, comp);
	if (!handle)
		return NULL;

	a = Rt_ArrayGet(&s->compData, handle->type);
	if (!a)
		return NULL;

	return Rt_ArrayGet(a, handle->index);
}

CompHandle
E_ComponentHandle(void *ptr)
{
	struct CompHandleData *handle = ((struct CompBase *)ptr)->_self;
	return handle->handle;
}

CompTypeId
E_ComponentTypeS(struct Scene *s, CompHandle comp)
{
	struct CompHandleData *handle = NULL;

	handle = _HandlePtr(s, comp);
	if (!handle)
		return (size_t)ES_INVALID_COMPONENT_TYPE;

	return handle->type;
}

CompTypeId
E_ComponentTypeId(const char *typeName)
{
	uint64_t hash = 0;
	hash = Rt_HashString(typeName);
	return Rt_ArrayFindId(&_component_types, &hash, _CompType_cmp);
}

size_t
E_ComponentCountS(struct Scene *s, CompTypeId type)
{
	return ((Array *)Rt_ArrayGet(&s->compData, type))->count;
}

EntityHandle
E_ComponentOwnerS(struct Scene *s, CompHandle comp)
{
	struct CompBase *ptr = E_ComponentPtrS(s, comp);
	return ptr ? ptr->_owner : ES_INVALID_ENTITY;
}

void
E_SetComponentOwnerS(struct Scene *s, CompHandle comp, EntityHandle owner)
{
	struct CompBase *ptr = E_ComponentPtrS(s, comp);
	ptr->_owner = owner;
}

bool
E_RegisterComponent(const char *name, size_t size, size_t alignment, CompInitProc create, CompTermProc destroy)
{
	struct CompType type;

	type.size = (size + alignment - 1) & ~(alignment - 1);
	type.alignment = alignment;
	type.hash = Rt_HashString(name);
	type.create = create;
	type.destroy = destroy;

	if (size < sizeof(struct CompBase))
		return false;

	if (Rt_ArrayFindId(&_component_types, &type.hash, _CompType_cmp) != RT_NOT_FOUND)
		return false;

	return Rt_ArrayAdd(&_component_types, &type);
}

const Array *
E_GetAllComponentsS(const struct Scene *s, CompTypeId type)
{
	if (type >= s->compData.count)
		return NULL;

	return Rt_ArrayGet(&s->compData, type);
}

bool
E_InitComponents(void)
{
	if (!Rt_InitArray(&_component_types, 40, sizeof(struct CompType)))
		return false;

	return E_LoadComponents();
}

void
E_TermComponents(void)
{
	Rt_TermArray(&_component_types);
}

bool
E_InitSceneComponents(struct Scene *s)
{
	size_t i = 0;

	if (!Rt_InitArray(&s->compData, _component_types.count, sizeof(Array)))
		return false;

	if (!Rt_InitArray(&s->compHandle, 100, sizeof(struct CompHandleData)))
		return false;

	Rt_FillArray(&s->compData);
	for (i = 0; i < _component_types.count; ++i) {
		struct CompType *type = Rt_ArrayGet(&_component_types, i);
		struct Array *a = Rt_ArrayGet(&s->compData, i);

		if (!Rt_InitAlignedArray(a, 10, type->size, type->alignment))
			return false;
	}

	return true;
}

void
E_TermSceneComponents(struct Scene *s)
{
	size_t i = 0, j = 0;
	for (i = 0; i < s->compData.count; ++i) {
		struct CompType *type = Rt_ArrayGet(&_component_types, i);
		Array *a = Rt_ArrayGet(&s->compData, i);

		if (type->destroy)
			for (j = 0; j < a->count; ++j)
				type->destroy(Rt_ArrayGet(a, j));

		Rt_TermArray(a);
	}

	Rt_TermArray(&s->compData);
	Rt_TermArray(&s->compHandle);
}

static int
_CompType_cmp(const void *item, const void *data)
{
	return !(((struct CompType *)item)->hash == *((uint64_t *)data));
}

static int
_CompHandle_cmp(const void *item, const void *data)
{
	const struct CompHandleData *handle = item;
	const CompHandle val = *(CompHandle *)data;

	if (handle->handle == val)
		return 0;
	else if (handle->handle > val)
		return 1;
	else
		return -1;
}

static inline struct CompHandleData *
_HandlePtr(struct Scene *s, CompHandle comp)
{
	return Rt_ArrayBSearch(&s->compHandle, &comp, _CompHandle_cmp);
}
