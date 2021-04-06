#ifndef _E_ENGINE_H_
#define _E_ENGINE_H_

#include <stdint.h>
#include <stdbool.h>

#include <Engine/Types.h>

#ifdef __cplusplus
extern "C" {
#endif

ENGINE_API extern void *E_screen;
ENGINE_API extern uint32_t *E_screenWidth;
ENGINE_API extern uint32_t *E_screenHeight;
ENGINE_API extern double E_deltaTime;

bool E_Init(int argc, char *argv[]);
void E_Term(void);

int E_Run(void);
void E_Frame(void);

double E_Time(void);

void E_Shutdown(void);

#ifdef __cplusplus
}
#endif

#endif /* _E_ENGINE_H_ */