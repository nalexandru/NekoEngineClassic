#include <Scene/Scene.h>
#include <Scene/Transform.h>
#include <Scene/Components.h>

#include <Engine/Engine.h>
#include <Engine/Events.h>
#include <Engine/Entity.h>
#include <Engine/ECSystem.h>
#include <Engine/Resource.h>
#include <Engine/Component.h>
#include <Engine/Application.h>

#include <Input/Input.h>

#include <System/Log.h>

#include <UI/UI.h>

#include <Render/Render.h>

#include "TestApplication.h"

#ifndef _countof
#	define _countof(array) (sizeof(array) / sizeof(array[0]))
#endif

struct ApplicationInfo App_applicationInfo =
{
	A_PGM_NAME,
	A_CPY_STR,
	{ A_VER_MAJOR, A_VER_MINOR, A_VER_BUILD, A_VER_REVISION }
};

static uint64_t _sceneLoadedEvt;

struct PlayerMovement
{
	COMPONENT_BASE;

	float movementSpeed, rotationSpeed;
	uint32_t moveForward, moveRight, moveUp, rotateHorizontal, rotateVertical;
};

struct Statistics
{
	COMPONENT_BASE;

	uint32_t frames;
	double time;
	char fpsBuff[20];
	char ftBuff[30];
};

static struct Vertex _vertices[] =
{
	{ -.5f, -.5f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f },
	{ -.5f,  .5f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f },
	{  .5f,  .5f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f },
	{  .5f, -.5f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f }
};
static uint16_t _indices[] = { 0, 1, 2, 0, 2, 3 };
static struct Mesh _mesh = { 4, 0, 6, "/Materials/Anna.mat" };

static bool App_InitPlayerMovement(struct PlayerMovement *comp, const void **args);
static void App_TermPlayerMovement(struct PlayerMovement *comp) { }
static void App_PlayerMovement(void **comp, void *args);
static void App_SceneLoaded(void *user, void *args);

static bool App_InitStatistics(struct PlayerMovement *comp, const void **args) { return true; }
static void App_TermStatistics(struct PlayerMovement *comp) { }
static void App_DrawStatistics(void **comp, void *args);

bool
App_InitApplication(int argc, char *argv[])
{
	struct Scene *scn;
	EntityHandle ent;
	CompHandle compHandle;
	const char *comp[] = { TRANSFORM_COMP, "PlayerMovement" };
	const void *xfArgs[] =
	{
		"Position", "0.0, 0.0, 0.0",
		"Rotation", "0.0, 0.0, 0.0",
		"Scale", "1.0, 1.0, 1.0",
		NULL
	};
	const void *camArgs[] =
	{
		"Active", "true",
		NULL
	};
	//const char *mrArgs[] = { "Model", "/Models/Anna.glb", NULL };
	struct ModelRender *mr = NULL;
	struct ModelCreateInfo mci = { 0 };

	E_RegisterComponent("PlayerMovement", sizeof(struct PlayerMovement), 1, (CompInitProc)App_InitPlayerMovement, (CompTermProc)App_TermPlayerMovement);
	E_RegisterSystem("App_PlayerMovement", ECSYS_GROUP_LOGIC, comp, _countof(comp), (ECSysExecProc)App_PlayerMovement, 0);

	comp[0] = UI_CONTEXT_COMP; comp[1] = "Statistics";
	E_RegisterComponent("Statistics", sizeof(struct Statistics), 1, (CompInitProc)App_InitStatistics, (CompTermProc)App_TermStatistics);
	E_RegisterSystem("App_DrawStatistics", ECSYS_GROUP_LOGIC, comp, _countof(comp), (ECSysExecProc)App_DrawStatistics, 0);

//	_sceneLoadedEvt = E_RegisterHandler(EVT_SCENE_LOADED, App_SceneLoaded, NULL);
//	Scn_StartSceneLoad("/Scenes/Sponza.scn");

	scn = Scn_CreateScene("TestScene");

	ent = E_CreateEntityS(scn, NULL);
	E_AddNewComponentS(scn, ent, E_ComponentTypeId(TRANSFORM_COMP), xfArgs);
	compHandle = E_AddNewComponentS(scn, ent, E_ComponentTypeId(MODEL_RENDER_COMP), NULL);

	mci.vertices = _vertices;
	mci.vertexCount = sizeof(_vertices) / sizeof(_vertices[0]);
	mci.indices16 = _indices;
	mci.indexCount = sizeof(_indices) / sizeof(_indices[0]);
	mci.meshes = &_mesh;
	mci.meshCount = 1;

	mr = E_ComponentPtrS(scn, compHandle);
	
	mr->model = E_CreateResource("Anna", RES_MODEL, &mci);
	mr->materials = calloc(1, sizeof(*mr->materials));
	mr->materials[0] = E_LoadResource("/Materials/Anna.mat", RES_MATERIAL);

	ent = E_CreateEntityS(scn, NULL);
	
	E_AddNewComponentS(scn, ent, E_ComponentTypeId("PlayerMovement"), NULL);
	E_AddNewComponentS(scn, ent, E_ComponentTypeId(TRANSFORM_COMP), xfArgs);
	E_AddNewComponentS(scn, ent, E_ComponentTypeId(CAMERA_COMP), camArgs);
	
	ent = E_CreateEntityS(scn, NULL);
	E_AddNewComponentS(scn, ent, E_ComponentTypeId(UI_CONTEXT_COMP), NULL);
	E_AddNewComponentS(scn, ent, E_ComponentTypeId("Statistics"), NULL);

	scn->loaded = true;
	Scn_ActivateScene(scn);

	return true;
}

void
App_TermApplication(void)
{
//	Scn_UnloadScene(Scn_activeScene);
}

bool
App_InitPlayerMovement(struct PlayerMovement *comp, const void **args)
{
	comp->movementSpeed = 100.f;
	comp->rotationSpeed = 50.f;

	for (; args && *args; ++args) {
		const char *arg = (const char *)*args;
		size_t len = strlen(arg);

		if (!strncmp(arg, "MovementSpeed", len))
			comp->movementSpeed = strtof((const char *)*(++args), NULL);
		else if (!strncmp(arg, "RotationSpeed", len))
			comp->rotationSpeed = strtof((const char *)*(++args), NULL);
	}

	comp->moveForward = In_CreateMap("forward");
	comp->moveRight = In_CreateMap("lateral");
	comp->moveUp = In_CreateMap("vertical");
	comp->rotateHorizontal = In_CreateMap("rotlMap");
	comp->rotateVertical = In_CreateMap("rotvMap");

	return true;
}

void
App_PlayerMovement(void **comp, void *args)
{
	struct Transform *xform = (struct Transform *)comp[0];
	struct PlayerMovement *mvmt = (struct PlayerMovement *)comp[1];
	struct vec3 raxis = { 0.f, -1.f, 0.f };
	float xlate, rot;

	xlate = mvmt->movementSpeed * (float)E_deltaTime;
	rot = mvmt->rotationSpeed * (float)E_deltaTime;

	xform_rotate(xform, In_Axis(mvmt->rotateHorizontal) * rot, &raxis);

	v3_copy(&raxis, &xform->right);
	xform_rotate(xform, (In_Axis(mvmt->rotateVertical) * -1.f) * rot, &raxis);
	
	xform_move_forward(xform, In_Axis(mvmt->moveForward) * xlate);
	xform_move_right(xform, In_Axis(mvmt->moveRight) * xlate);
	xform_move_up(xform, In_Axis(mvmt->moveUp) * -xlate);
}

void
App_SceneLoaded(void *user, void *args)
{
	Scn_ActivateScene((struct Scene *)args);
}

void
App_DrawStatistics(void **comp, void *args)
{
	struct UIContext *ctx = (struct UIContext *)comp[0];
	struct Statistics *stats = (struct Statistics *)comp[1];
	double delta = E_Time() - stats->time;

	++stats->frames;

	if (delta > 1.0) {
		double ft = (delta / (double)stats->frames) * 1000;
	
		sprintf(stats->fpsBuff, "FPS: %d", stats->frames);
		sprintf(stats->ftBuff, "Frame Time: %.02f ms", ft);

		stats->time += delta;
		stats->frames = 0;
	}

	UI_DrawText(ctx, stats->fpsBuff, 0.f, 0.f, 20.f, NULL);
	UI_DrawText(ctx, stats->ftBuff, 0.f, 20.f, 20.f, NULL);
}

