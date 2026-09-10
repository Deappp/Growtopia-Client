#include "PlatformPrecomp.h"
#include "InputMenu.h"
#include "Gamepad/GamepadManager.h"

ArcadeInputComponent* g_pArcadeComponent = NULL;

ArcadeInputComponent* GetArcadeComponent()
{
	return g_pArcadeComponent;
}

//@STUB SMALLTODO
void SetupInputGamepadsAndHotkeys()
{
}