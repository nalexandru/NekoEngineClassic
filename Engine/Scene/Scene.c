#include <stdio.h>
#include <ctype.h>

#include <Engine/IO.h>
#include <Engine/Job.h>
#include <Engine/Config.h>
#include <System/Log.h>
#include <Engine/Events.h>
#include <Scene/Scene.h>
#include <Scene/Camera.h>
#include <Render/Render.h>
#include <System/System.h>
#include <System/Memory.h>
#include <Runtime/Runtime.h>
#include <Engine/Resource.h>

#include "../Engine/ECS.h"

#define SCNMOD	"Scene"
#define BUFF_SZ	512

struct Scene *Scn_activeScene = NULL;
static inline bool _InitScene(struct Scene *s);
static void _LoadJob(int worker, struct Scene *scn);
static inline void _ReadSceneInfo(struct Scene *s, struct Stream *stm, char *data);
static inline void _ReadEntity(struct Scene *s, struct Stream *stm, char *data, Array *args);

struct Scene *
Scn_CreateScene(const char *name)
{
	struct Scene *s = Sys_Alloc(sizeof(*s), 1, MH_Persistent);
	if (!s)
		return NULL;

	snprintf(s->name, sizeof(s->name), "%s", name);

	if (!_InitScene(s)) {
		Sys_Free(s);
		return NULL;
	}

	return s;
}

struct Scene *
Scn_StartSceneLoad(const char *path)
{
	struct Scene *s = Sys_Alloc(sizeof(*s), 1, MH_Persistent);
	if (!s)
		return NULL;

	snprintf(s->path, sizeof(s->path), "%s", path);

	if (!_InitScene(s)) {
		Sys_Free(s);
		return NULL;
	}

	if (E_GetCVarBln("Engine_SingleThreadSceneLoad", false))
		_LoadJob(0, s);
	else
		E_ExecuteJob((JobProc)_LoadJob, s, NULL);

	return s;
}

void
Scn_UnloadScene(struct Scene *s)
{
	E_TermSceneEntities(s);
	E_TermSceneComponents(s);

	Sys_Free(s);
}

bool
Scn_ActivateScene(struct Scene *s)
{
	if (!s->loaded)
		return false;

	Scn_activeScene = s;

	return true;
}

bool
_InitScene(struct Scene *s)
{
	if (!E_InitSceneComponents(s) || !E_InitSceneEntities(s))
		goto error;

	return true;

error:
	E_TermSceneEntities(s);
	E_TermSceneComponents(s);

	return false;
}

void
_LoadJob(int wid, struct Scene *s)
{
	struct Stream stm;
	char *data = NULL;
	Array args;

	if (!E_FileStream(s->path, IO_READ, &stm)) {
		Sys_LogEntry(SCNMOD, LOG_CRITICAL, "Failed to open scene file %s", s->path);
		Scn_UnloadScene(s);
		return;
	}

	E_Broadcast(EVT_SCENE_LOAD_STARTED, s);

	data = Sys_Alloc(sizeof(char), BUFF_SZ, MH_Transient);

	Rt_InitPtrArray(&args, 10);

	while (!E_EndOfStream(&stm)) {
		char *line = E_ReadStreamLine(&stm, data, BUFF_SZ);
		size_t len;

		if (!*(line = Rt_SkipWhitespace(line)) || line[0] == '#')
			continue;
		
		len = strlen(line);

		if (!strncmp(line, "SceneInfo", len)) {
			_ReadSceneInfo(s, &stm, data);
		} else if (!strncmp(line, "EndSceneInfo", len)) {
			//
		} else if (strstr(line, "Entity")) {
			_ReadEntity(s, &stm, data, &args);
		}

		memset(data, 0x0, BUFF_SZ);
	}

	E_CloseStream(&stm);

	s->loaded = true;
	E_Broadcast(EVT_SCENE_LOADED, s);
}

void
_ReadSceneInfo(struct Scene *s, struct Stream *stm, char *data)
{
	while (!E_EndOfStream(stm)) {
		char *line = E_ReadStreamLine(stm, data, BUFF_SZ);
		size_t len;

		if (!*(line = Rt_SkipWhitespace(line)) || line[0] == '#')
			continue;
		
		len = strlen(line);

		if (!strncmp(line, "Name", 4)) {
			snprintf(s->name, sizeof(s->name), "%s", strchr(line, '=') + 1);
		} else if (!strncmp(line, "EnvironmentMap", 14)) {
			s->environmentMap = E_LoadResource(strchr(line, '=') + 1, RES_TEXTURE);
		} else if (!strncmp(line, "EndSceneInfo", len)) {
			break;
		}
	}
}

void
_ReadEntity(struct Scene *s, struct Stream *stm, char *data, Array *args)
{
	EntityHandle entity = NULL;
	char *compType = NULL;

	compType = Sys_Alloc(sizeof(*compType), BUFF_SZ, MH_Transient);

	while (!E_EndOfStream(stm)) {
		char *line = E_ReadStreamLine(stm, data, BUFF_SZ);
		size_t len;

		if (!*(line = Rt_SkipWhitespace(line)) || line[0] == '#')
			continue;
		
		len = strlen(line);

		if (!strncmp(line, "Component=", 10)) {
			snprintf(compType, BUFF_SZ, "%s", strchr(line, '=') + 1);
			
			if (!entity)
				entity = E_CreateEntityS(s, NULL);

			Rt_ZeroArray(args);
		} else if (!strncmp(line, "EndComponent", len)) {
			E_AddNewComponentS(s, entity, E_ComponentTypeId(compType), (const void **)args->data);
			memset(compType, 0x0, sizeof(char) * BUFF_SZ);
		} else if (!strncmp(line, "EndEntity", len)) {
			break;
		} else if (compType[0] == 0x0) {
			if (!strncmp(line, "Type=", 5))
				E_CreateEntityS(s, line + 5);
		} else {
			char *arg = line, *dst = NULL;
			char *val = strchr(line, '=');
			if (!val)
				continue;

			*val++ = 0x0;

			len = strlen(arg) + 1;
			dst = Sys_Alloc(sizeof(char), len, MH_Transient);
			strncpy(dst, arg, len);
			Rt_ArrayAddPtr(args, dst);

			len = strlen(val) + 1;
			dst = Sys_Alloc(sizeof(char), len, MH_Transient);
			strncpy(dst, val, len);
			Rt_ArrayAddPtr(args, dst);
		}
	}
}
