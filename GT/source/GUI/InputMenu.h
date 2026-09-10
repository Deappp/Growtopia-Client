#ifndef InputMenu_h__
#define InputMenu_h__
#include "Entity/ArcadeInputComponent.h"

enum eCustomVirtualKeys
{
	VIRTUAL_KEY_CUSTOM_TOGGLE_FPS = 510000,
	VIRTUAL_KEY_CUSTOM_TOGGLE_SCREENSHOT_MODE
};

ArcadeInputComponent* GetArcadeComponent();
void SetupInputGamepadsAndHotkeys();

#endif // InputMenu_h__