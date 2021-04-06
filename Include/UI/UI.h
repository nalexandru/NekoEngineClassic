#ifndef _UI_UI_H_
#define _UI_UI_H_

#include <Engine/Types.h>
#include <Engine/Component.h>

#include <UI/Text.h>

#define UI_UPDATE_BUFFERS		"UI_UpdateBuffers"
#define UI_DRAW_CONTEXT			"UI_DrawContext"

struct UIRect
{
	uint16_t x, y, width, height;
};

struct UIContext
{
	COMPONENT_BASE;

	Array vertices, indices, draws;
};

bool UI_InitUI(void);
void UI_TermUI(void);

void UI_Render(struct Scene *s);

void UI_SetFont(struct UIContext *ctx, struct Font *font);
void UI_SetStyle(struct UIContext *ctx, struct UIStyle *style);

void UI_Box(struct UIContext *ctx, const struct UIRect *rc, const struct Texture *tex);
void UI_Text(struct UIContext *ctx, const struct UIRect *rc, const char *text);
void UI_Label(struct UIContext *ctx, const struct UIRect *rc, const char *text, const struct Texture *tex);
bool UI_Button(struct UIContext *ctx, const struct UIRect *rc, const char *text, const struct Texture *tex);

bool UI_InitContext(struct UIContext *ctx, const void **);
void UI_TermContext(struct UIContext *ctx);

void UI_UpdateBuffers(void **comp, void *args);
void UI_DrawContext(void **comp, void *args);

#endif /* _UI_UI_H_ */
