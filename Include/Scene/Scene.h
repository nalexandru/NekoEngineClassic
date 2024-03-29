#ifndef _SCN_SCENE_H_
#define _SCN_SCENE_H_

#include <Engine/Types.h>
#include <Scene/Scene.h>
#include <Runtime/Array.h>

#define RES_SCENE	"Scene"

struct Scene
{
	char name[64];
	char path[256];
	bool loaded;
	Array entities, compData, compHandle;
	Handle environmentMap;
	uint8_t renderDataStart;
};

ENGINE_API extern struct Scene *Scn_activeScene;
ENGINE_API extern struct Scene *Scn_loadingScene;

struct Scene *Scn_CreateScene(const char *name);
struct Scene *Scn_StartSceneLoad(const char *path);
void Scn_UnloadScene(struct Scene *scn);

bool Scn_ActivateScene(struct Scene *scn);

#endif /* _SCN_SCENE_H_ */
