#include <System/Log.h>
#include <System/System.h>
#include <Runtime/Runtime.h>

#include <Engine/ECSystem.h>
#include <Engine/Entity.h>
#include <Engine/Component.h>

#include "ECS.h"

#define ECSYS_MOD	"ECSystem"

bool E_RegisterSystems(void);

static Array _systems;
static Array _filteredEntities;

static void _ecsys_comp_update_proc(void *);
static void _ecsys_entity_update_proc(void *);

static int _ecsys_insert_cmp(const void *item, const void *data);
static inline void _sys_exec(struct Scene *s, struct ECSystem *sys, void *args);
static inline void _filterEntities(struct Scene *s, Array *ent, CompTypeId *comp_types, size_t type_count);

bool
E_RegisterSystem(const char *name, const char *group,
	const char **comp, size_t num_comp,
	ECSysExecProc proc, int32_t priority)
{
	size_t i = 0;
	CompTypeId types[MAX_ENTITY_COMPONENTS];

	if (num_comp > MAX_ENTITY_COMPONENTS)
		return false;

	for (i = 0; i < num_comp; ++i)
		types[i] = E_ComponentTypeId(comp[i]);

	return E_RegisterSystemId(name, group, types, num_comp, proc, priority);
}

bool
E_RegisterSystemId(const char *name, const char *group,
	const CompTypeId *comp, size_t num_comp,
	ECSysExecProc proc, int32_t priority)
{
	size_t pos = 0;
	struct ECSystem sys;

	memset(&sys, 0x0, sizeof(sys));

	if (num_comp > MAX_ENTITY_COMPONENTS)
		return false;

	if (!Rt_ArrayAllocate(&_systems))
		return false;
	--_systems.count;

	sys.name_hash = Rt_HashString(name);
	sys.group_hash = Rt_HashString(group);

	sys.comp_types = calloc(num_comp, sizeof(CompTypeId));
	if (!sys.comp_types)
		return false;

	memcpy(sys.comp_types, comp, sizeof(CompTypeId) * num_comp);
	sys.type_count = num_comp;
	sys.exec = proc;
	sys.priority = priority;

	pos = Rt_ArrayFindId(&_systems, &priority, _ecsys_insert_cmp);
	if (pos == RT_NOT_FOUND)
		pos = _systems.count;

	if (!Rt_ArrayInsert(&_systems, &sys, pos)) {
		free(sys.comp_types);
		return false;
	}

	return true;
}

void
E_ExecuteSystemS(struct Scene *s, const char *name, void *args)
{
	size_t i = 0;
	struct ECSystem *sys = NULL;
	uint64_t hash = Rt_HashString(name);

	for (i = 0; i < _systems.count; ++i) {
		sys = Rt_ArrayGet(&_systems, i);
		if (sys->name_hash == hash)
			break;
		sys = NULL;
	}

	if (!sys)
		return;

	_sys_exec(s, sys, args);
}

void
E_ExecuteSystemGroupS(struct Scene *s, const char *name)
{
	size_t i = 0;
	struct ECSystem *sys = NULL;
	uint64_t hash = Rt_HashString(name);

	for (i = 0; i < _systems.count; ++i) {
		sys = Rt_ArrayGet(&_systems, i);

		if (sys->group_hash != hash)
			continue;

		_sys_exec(s, sys, NULL);
	}
}

bool
E_InitECSystems(void)
{
	if (!Rt_InitArray(&_systems, 40, sizeof(struct ECSystem)))
		return false;

	if (!Rt_InitPtrArray(&_filteredEntities, 100))
		return false;

	return E_RegisterSystems();
}

void
E_TermECSystems(void)
{
	size_t i = 0;

	for (i = 0; i < _systems.count; ++i)
		free(((struct ECSystem *)Rt_ArrayGet(&_systems, i))->comp_types);

	Rt_TermArray(&_systems);
	Rt_TermArray(&_filteredEntities);
}

static int
_ecsys_insert_cmp(const void *item, const void *data)
{
	const struct ECSystem *sys = item;
	int32_t priority = *(int32_t *)data;

	if (priority > sys->priority)
		return 1;
	else if (priority < sys->priority)
		return -1;
	else
		return 0;
}

static inline void
_sys_exec(struct Scene *s, struct ECSystem *sys, void *args)
{
	size_t i = 0, j = 0;
	void *ptr = NULL;
	const Array *comp = NULL;
	EntityHandle handle = 0;
	void *components[MAX_ENTITY_COMPONENTS];

	if (sys->type_count == 1) {
		comp = E_GetAllComponentsS(s, sys->comp_types[0]);

		if (!comp)
			return;

		for (i = 0; i < comp->count; ++i) {
			ptr = Rt_ArrayGet(comp, i);
			sys->exec(&ptr, args);
		}
	} else {
		_filterEntities(s, &_filteredEntities, sys->comp_types, sys->type_count);

		for (i = 0; i < _filteredEntities.count; ++i) {
			handle = (EntityHandle *)Rt_ArrayGetPtr(&_filteredEntities, i);

			for (j = 0; j < sys->type_count; ++j)
				components[j] = E_GetComponentS(s, handle, sys->comp_types[j]);

			sys->exec(components, args);
		}
	}
}

static inline void
_filterEntities(struct Scene *s, Array *ent, CompTypeId *comp_types, size_t type_count)
{
	const Array *components;
	CompTypeId type = (size_t)-1;
	size_t count = 0, min_count = SIZE_MAX;
	struct CompBase *comp = NULL;
	bool valid = false;
	size_t i = 0, j = 0;

	for (i = 0; i < type_count; ++i) {
		count = E_ComponentCountS(s, comp_types[i]);

		if (count >= min_count)
			continue;

		type = comp_types[i];
		min_count = count;
	}

	if (type == (size_t)-1) {
		Sys_LogEntry(ECSYS_MOD, LOG_CRITICAL, "_filterEntities: Entity with least components not found. Is type_count set ?");
		return;
	}

	Rt_ClearArray(ent, false);
	components = E_GetAllComponentsS(s, type);

	if (ent->size < components->count)
		Rt_ResizeArray(ent, components->count);

	for (i = 0; i < components->count; ++i) {
		comp = Rt_ArrayGet(components, i);

		if (!comp->_owner)
			continue;

		valid = true;

		for (j = 0; j < type_count; ++j) {
			if (E_GetComponentS(s, comp->_owner, comp_types[j]))
				continue;

			valid = false;
			break;
		}

		if (valid)
			Rt_ArrayAddPtr(ent, comp->_owner);
	}
}
