#include "PlatformPrecomp.h"
#include "MainMenu.h"
#include "MiscGUI.h"
#include "App.h"
#include "Entity/CustomInputComponent.h"

//@STUB SMALLTODO
void MainMenuOnSelect(VariantList* pVList)
{
}

void AddMainMenuButton(Entity* pEnt, float& posX, float& posY,
	const std::string& entityName, const std::string& label, int buttonID)
{
	float desiredLinesY = 9.0f;

	if (IsDesktop())
	{
		desiredLinesY = 11.0f;
	}

	eFont font = FONT_SMALL;
	float fontScale = 0.0f;

	GetFontAndScaleToFitThisLinesPerScreenY(&font, &fontScale, desiredLinesY);

	float lineHeight = GetBaseApp()->GetFont(font)->GetLineHeight(fontScale);

	Entity* pTextButtonEnt = CreateTextButtonEntity(pEnt, entityName, posX, posY, label, false);
	posY += iPadMapY(15.0f) + lineHeight;

	pTextButtonEnt->GetFunction("OnButtonSelected")->sig_function.connect(&MainMenuOnSelect);

	SetTextShadowColor(pTextButtonEnt, MAKE_RGBA(0, 0, 0, 150));
	SetupTextEntity(pTextButtonEnt, font, fontScale);
	RemovePaddingEntity(pTextButtonEnt);
	SetAlignmentEntity(pTextButtonEnt, ALIGNMENT_UPPER_CENTER);

	Entity* pBMPEnt = AddBMPRectAroundEntity(pTextButtonEnt, MAKE_RGBA(165, 227, 251, 255), MAKE_RGBA(165, 227, 251, 255), iPadMapY(5.0f), true, fontScale, font);
	FadeInEntity(pTextButtonEnt, true, 400, (buttonID * 300) + 400);

	ZoomToPositionFromThisOffsetEntity(pBMPEnt, CL_Vec2f(-GetScreenSizeXf(), 0), 500, INTERPOLATE_SMOOTHSTEP, (buttonID * 300) + 100);

	GetMessageManager()->SendGame(MESSAGE_TYPE_PLAY_SOUND, Variant(std::string("audio/tile_created.wav")));
}

void OnStartLogoBob(VariantList* pVList)
{
	Entity* pLogoEntity = pVList->m_variant[0].GetEntity();

	BobEntity(pLogoEntity, iPadMapY(3.0));
}

void MainMenuCreate(Entity* pParentEnt)
{
	GetMessageManager()->SendGame(MESSAGE_TYPE_PLAY_MUSIC_FORCE_STREAMING,
		Variant("audio/mp3/theme.mp3"), 200);

	Entity* pBG = pParentEnt->AddEntity(new Entity("MainMenu"));
	
	// we'd want to do : pBG->AddComponent(new MapBGComponent());

	float baseX = iPadMapX(512.0f);
	float baseY = iPadMapY(420.0f);

	AddMainMenuButton(pBG, baseX, baseY, "Online", "Play Online", 0);
	AddMainMenuButton(pBG, baseX, baseY, "Options", "Options", 1);
	AddMainMenuButton(pBG, baseX, baseY, "About", "About", 2);

	if (IsDesktop())
	{
		AddMainMenuButton(pBG, baseX, baseY, "Quit", "Quit", 3);
	}

	Entity* pMainLogoEnt = CreateOverlayEntity(pBG, "MainLogo", "interface/large/game_title.rttex",
		GetScreenSizeXf() * 0.5, iPhoneMapY(20.0));

	SetAlignmentEntity(pMainLogoEnt, ALIGNMENT_UPPER_CENTER);
	EntitySetScaleBySize(pMainLogoEnt, CL_Vec2f(GetScreenSizeXf() * 0.7f, GetScreenSizeYf() * 0.4f), true);

	ZoomToPositionFromThisOffsetEntity(pMainLogoEnt, CL_Vec2f(0, -1000), 1400, INTERPOLATE_EASE_TO, 500);


	VariantList varList{};
	varList.Get(0).Set(pMainLogoEnt);

	GetMessageManager()->CallEntityFunction(pMainLogoEnt, 1700, "StartBob", &varList);

	pMainLogoEnt->GetFunction("StartBob")->sig_function.connect(&OnStartLogoBob);


	std::string version = "";
	version += "DEA'S ";
	version += GetApp()->GetVersionString();

	Entity* pVersionEnt = CreateTextLabelEntity(pBG, "version", GetScreenSizeXf() - iPhoneMapX(5.0), GetScreenSizeYf() - iPhoneMapY(5.0), version);

	eFont versionFont = FONT_SMALL;
	float versionScale = 0.0f;
	GetFontAndScaleToFitThisLinesPerScreenY(&versionFont, &versionScale, 20.0f);

	SetupTextEntity(pVersionEnt, versionFont, versionScale);
	SetAlignmentEntity(pVersionEnt, ALIGNMENT_DOWN_RIGHT);
	FadeInEntity(pVersionEnt, false, 500, 500);

	//so android's back key or ESC on windows will quit.
	EntityComponent* pComp = pBG->AddComponent(new CustomInputComponent());
	pComp->GetFunction("OnActivated")->sig_function.connect(1, boost::bind(&App::OnExitApp, GetApp(), _1));
	pComp->GetVar("keycode")->Set(uint32(VIRTUAL_KEY_BACK));

	pBG->GetParent()->MoveEntityToBottomByAddress(pBG);

	static bool bFirstTime = false;

	if (!bFirstTime)
	{
		bFirstTime = true;
	}
	else
	{
		SlideScreen(pBG, true);
	}
}