#include "PlatformPrecomp.h"
#include "InputMenu.h"
#include "Gamepad/GamepadManager.h"
#include "Entity/EntityUtils.h"
#include "App.h"
#include "Net/NetControllerLocal.h"

ArcadeInputComponent* g_pArcadeComponent = NULL;

ArcadeInputComponent* GetArcadeComponent()
{
	return g_pArcadeComponent;
}

void OnArcadeInput(VariantList* pVList)
{
	uint32 vKey = pVList->Get(0).GetUINT32();
	uint32 keyState = pVList->Get(1).GetUINT32();

	if (vKey == VIRTUAL_KEY_CUSTOM_TOGGLE_SCREENSHOT_MODE)
	{
		ShowTextMessage("Toggling screenshot mode");
		GetApp()->SetScreenShotMode(!GetApp()->GetScreenShotMode());
		return;
	}
	else if (vKey == VIRTUAL_KEY_CUSTOM_TOGGLE_FPS)
	{
		LogMsg("Toggling FPS");
		GetBaseApp()->SetFPSVisible(!GetBaseApp()->GetFPSVisible());

		if (!GetBaseApp()->GetFPSVisible())
		{
			GetBaseApp()->SetFPSLimit(60.0f);
		}
		else {
			GetBaseApp()->SetFPSLimit(0.0f);
		}

		return;
	}

	if (GetGameLogic()->GetLocalPlayer() != NULL)
	{
		((NetControllerLocal*)(GetGameLogic()->GetLocalPlayer()->m_pController))
			->OnArcadeInput(vKey, keyState != 0);
	}
}

void AddWASDKeys()
{
	if (g_pArcadeComponent != NULL)
	{
		AddKeyBinding(g_pArcadeComponent, "chatkey_A", 65, VIRTUAL_KEY_DIR_LEFT);
		AddKeyBinding(g_pArcadeComponent, "chatkey_D", 68, VIRTUAL_KEY_DIR_RIGHT);
		AddKeyBinding(g_pArcadeComponent, "chatkey_W", 87, VIRTUAL_KEY_GAME_JUMP);
		AddKeyBinding(g_pArcadeComponent, "chatkey_S", 83, VIRTUAL_KEY_DIR_DOWN);
		AddKeyBinding(g_pArcadeComponent, "chatkey_Space", 32, VIRTUAL_KEY_GAME_JUMP);
	}
}

void OnNativeInputStateChanged(VariantList* pVList)
{
	//Ensure when typing we don't walk around.

	uint32 inputState = pVList->Get(0).GetUINT32();

	if (inputState == 0)
	{
		//add our wasds back if our input state changed back to normal
		AddWASDKeys();
	}
	else if (g_pArcadeComponent != NULL)
	{
		//disable wasds when we're typing something (in the chat, or similar)
		VariantList vList;
		vList.Get(0).Set("chatkey_");
		g_pArcadeComponent->GetFunction("RemoveKeyBindingsStartingWith")->sig_function(&vList);
		g_pArcadeComponent->ResetDirectionKeys(true);
	}

	LogMsg("Keyboard active: %d", inputState != 0);
}

void SetupInputGamepadsAndHotkeys()
{
	Entity* pInputMenu = GetEntityRoot()->AddEntity(new Entity("InputMenu"));

	g_pArcadeComponent = new ArcadeInputComponent();
	pInputMenu->AddComponent(g_pArcadeComponent);

	if (GetGamepadManager()->GetGamepadCount() != 0)
	{
		for (int gamepad = 0; gamepad < GetGamepadManager()->GetGamepadCount(); gamepad++)
		{
			Gamepad* pGamePad = GetGamepadManager()->GetGamepad((eGamepadID)gamepad);
			pGamePad->ConnectToArcadeComponent(g_pArcadeComponent, true, true);
		}
	}

	AddKeyBinding(g_pArcadeComponent, "Left", VIRTUAL_KEY_DIR_LEFT, VIRTUAL_KEY_DIR_LEFT);
	AddKeyBinding(g_pArcadeComponent, "Right", VIRTUAL_KEY_DIR_RIGHT, VIRTUAL_KEY_DIR_RIGHT);
	AddKeyBinding(g_pArcadeComponent, "Up", VIRTUAL_KEY_DIR_UP, VIRTUAL_KEY_DIR_UP);
	AddKeyBinding(g_pArcadeComponent, "Down", VIRTUAL_KEY_DIR_DOWN, VIRTUAL_KEY_DIR_DOWN);
	AddKeyBinding(g_pArcadeComponent, "Jump", VIRTUAL_DPAD_BUTTON_DOWN, VIRTUAL_KEY_GAME_JUMP);
	AddKeyBinding(g_pArcadeComponent, "Action", VIRTUAL_DPAD_BUTTON_LEFT, VIRTUAL_KEY_GAME_FIRE);
	AddKeyBinding(g_pArcadeComponent, "Jump", VIRTUAL_KEY_CONTROL, VIRTUAL_KEY_GAME_JUMP);
	AddKeyBinding(g_pArcadeComponent, "XperiaA", VIRTUAL_KEY_DIR_CENTER, VIRTUAL_KEY_GAME_JUMP);
	AddKeyBinding(g_pArcadeComponent, "Back", VIRTUAL_DPAD_RTRIGGER, VIRTUAL_KEY_BACK);

#ifdef RT_ADMIN
	if (IsDesktop())
	{
		//desktop only admin features:
		//Toggle FPS = CTRL + F
		AddKeyBinding(g_pArcadeComponent, "FPSToggle", 70, VIRTUAL_KEY_CUSTOM_TOGGLE_FPS, false, 1);
		AddKeyBinding(g_pArcadeComponent, "ScreenshotModeToggle", 72, VIRTUAL_KEY_CUSTOM_TOGGLE_SCREENSHOT_MODE, false, 1);
	}
#endif

	auto& sig = pInputMenu->GetShared()->GetFunction("OnArcadeInput")->sig_function;
	GetBaseApp()->m_sig_arcade_input.connect(sig);

	pInputMenu->GetShared()->GetFunction("OnArcadeInput")->sig_function.connect(&OnArcadeInput);

	AddWASDKeys();

	//submit our event to disable WASDs in input boxes and similar (so the player doesn't walk around while we type)
	GetBaseApp()->m_sig_native_input_state_changed.connect(&OnNativeInputStateChanged);
}