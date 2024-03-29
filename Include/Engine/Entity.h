#ifndef _E_ENTITY_H_
#define _E_ENTITY_H_

#include <stdarg.h>

#include <Engine/Types.h>

#define MAX_ENTITY_COMPONENTS		30
#define ES_INVALID_COMPONENT		-1
#define ES_INVALID_COMPONENT_TYPE	-1
#define ES_INVALID_ENTITY			NULL
#define ES_INVALID_ECSYS			-1

struct EntityCompInfo
{
	const char *type;
	const void **args;
};

ENGINE_API extern struct Scene *Scn_activeScene;

EntityHandle E_CreateEntityS(struct Scene *s, const char *type);
static inline EntityHandle E_CreateEntity(const char *type) { return E_CreateEntityS(Scn_activeScene, type); }

EntityHandle E_CreateEntityWithArgsS(struct Scene *s, const CompTypeId *compTypes, const void ***compArgs, uint8_t typeCount);
static inline EntityHandle E_CreateEntityWithArgs(const CompTypeId *compTypes, const void ***compArgs, uint8_t typeCount)
{ return E_CreateEntityWithArgsS(Scn_activeScene, compTypes, compArgs, typeCount); }

EntityHandle E_CreateEntityVS(struct Scene *s, int count, const struct EntityCompInfo *info);
static inline EntityHandle E_CreateEntityV(int count, const struct EntityCompInfo *info) { return E_CreateEntityVS(Scn_activeScene, count, info); }

EntityHandle E_CreateEntityWithComponentsS(struct Scene *s, int count, ...);

bool E_AddComponentS(struct Scene *s, EntityHandle ent, CompTypeId type, CompHandle comp);
static inline bool E_AddComponent(EntityHandle ent, CompTypeId type, CompHandle comp) { return E_AddComponentS(Scn_activeScene, ent, type, comp); }

bool E_AddNewComponentS(struct Scene *s, EntityHandle ent, CompTypeId type, const void **args);
static inline bool E_AddNewComponent(EntityHandle ent, CompTypeId type, const void **args) { return E_AddNewComponentS(Scn_activeScene, ent, type, args); }

void *E_GetComponentS(struct Scene *s, EntityHandle ent, CompTypeId type);
static inline void *E_GetComponent(EntityHandle ent, CompTypeId type) { return E_GetComponentS(Scn_activeScene, ent, type); }

void E_RemoveComponentS(struct Scene *s, EntityHandle ent, CompTypeId type);
static inline void E_RemoveComponent(EntityHandle ent, CompTypeId type) { E_RemoveComponentS(Scn_activeScene, ent, type); }

void E_DestroyEntityS(struct Scene *s, EntityHandle ent);
static inline void E_DestroyEntity(EntityHandle ent) { E_DestroyEntityS(Scn_activeScene, ent); }

void *E_EntityPtrS(struct Scene *s, EntityHandle ent);
static inline void *E_EntityPtr(EntityHandle ent) { return E_EntityPtrS(Scn_activeScene, ent); }

bool E_RegisterEntityType(const char *name, const CompTypeId *comp_types, uint8_t type_count);

#endif /* _E_ENTITY_H_ */
