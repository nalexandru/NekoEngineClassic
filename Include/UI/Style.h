#ifndef _UI_STYLE_H_
#define _UI_STYLE_H_

#include <Engine/Types.h>
#include <Engine/Component.h>

struct UIStyle
{
	struct Font *font;
	
	float backgroundColor[4];
	float foregroundColor[4];
};

#endif /* _UI_STYLE_H_ */
