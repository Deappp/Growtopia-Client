#include "PlatformPrecomp.h"
#include "App.h"
#include "Renderer/LinearParticle.h"
#include "Entity/EntityUtils.h"
#include "Renderer/SoftSurface.h"
#include "Ad/AdProviderChartBoost.h"
#include "Gamepad/GamepadManager.h"
#include "GUI/InputMenu.h"
#include "GUI/MainMenu.h"

const char* GetAppName() { return "Growtopia"; }
const char* GetBundlePrefix() { return "com.rtsoft."; }
const char* GetBundleName() { return "growtopia"; }

GamepadManager g_gamepadManager{};
GamepadManager* GetGamepadManager() { return &g_gamepadManager; }

MessageManager g_messageManager{};
MessageManager * GetMessageManager() {return &g_messageManager;}

FileManager g_fileManager{};
FileManager * GetFileManager() {return &g_fileManager;}

#ifdef __APPLE__

#if TARGET_OS_IPHONE == 1
	//it's an iPhone or iPad
	//#include "Audio/AudioManagerOS.h"
	//AudioManagerOS g_audioManager;
	#include "Audio/AudioManagerDenshion.h"
	
	AudioManagerDenshion g_audioManager;
#else
	//it's being compiled as a native OSX app
   #include "Audio/AudioManagerFMOD.h"
  AudioManagerFMOD g_audioManager; //dummy with no sound

//in theory, CocosDenshion should work for the Mac builds, but right now it seems to want a big chunk of
//Cocos2d included so I'm not fiddling with it for now

//#include "Audio/AudioManagerDenshion.h"
//AudioManagerDenshion g_audioManager;
#endif
	
#else

#if defined RT_WEBOS || defined RTLINUX
#include "Audio/AudioManagerSDL.h"
AudioManagerSDL g_audioManager; //sound in windows and WebOS
//AudioManager g_audioManager; //to disable sound
#elif defined ANDROID_NDK
#include "Audio/AudioManagerAndroid.h"
AudioManagerAndroid g_audioManager; //sound for android
#elif defined PLATFORM_BBX
#include "Audio/AudioManagerBBX.h"
//AudioManager g_audioManager; //to disable sound
AudioManagerBBX g_audioManager;
#elif defined PLATFORM_FLASH
//AudioManager g_audioManager; //to disable sound
#include "Audio/AudioManagerFlash.h"
AudioManagerFlash *g_audioManager = new AudioManagerFlash;
#else


//in windows
//AudioManager g_audioManager; //to disable sound

#ifdef RT_FLASH_TEST
#include "Audio/AudioManagerFlash.h"
AudioManagerFlash g_audioManager;
#else

#include "Audio/AudioManagerAudiere.h"
AudioManagerAudiere g_audioManager;  //Use Audiere for audio
#endif
//#include "Audio/AudioManagerFMOD.h"
//AudioManagerFMOD g_audioManager; //if we wanted FMOD sound in windows

#endif
#endif

#if defined PLATFORM_FLASH
	AudioManager * GetAudioManager(){return g_audioManager;}
#else
	AudioManager * GetAudioManager(){return &g_audioManager;}
#endif

App *g_pApp = NULL;

BaseApp * GetBaseApp() 
{
	if (!g_pApp)
	{
#ifndef NDEBUG
		LogMsg("Creating app object");
#endif
		g_pApp = new App;
	}

	return g_pApp;
}

App * GetApp() 
{
	return g_pApp;
}

App::App()
{
	m_bDidPostInit = false;
}

App::~App()
{
	L_ParticleSystem::deinit();
#ifdef PLATFORM_FLASH
	SAFE_DELETE(g_audioManager);
#endif
}

void App::OnExitApp(VariantList *pVarList)
{
	LogMsg("Exiting the app");

	OSMessage o;
	o.m_type = OSMessage::MESSAGE_FINISH_APP;
	GetBaseApp()->AddOSMessage(o);
}

bool App::Init()
{
	switch (GetEmulatedPlatformID())
	{
	case PLATFORM_ID_OSX:
	case PLATFORM_ID_ANDROID:
	{
		SetLockedLandscape(false);
		break;
	}
	case PLATFORM_ID_WEBOS:
	{
		if ((GetScreenSizeX() == 1024 && GetScreenSizeY() == 768) ||
			GetScreenSizeX() == 768 && GetScreenSizeY() == 1024)
		{
			SetLockedLandscape(false);
			SetupScreenInfo(GetPrimaryGLX(), GetPrimaryGLY(), ORIENTATION_PORTRAIT);
		}

		break;
	}
	case PLATFORM_ID_BBX:
	{
		SetLockedLandscape(false);
		SetupScreenInfo(GetPrimaryGLX(), GetPrimaryGLY(), ORIENTATION_PORTRAIT);
		break;
	}
	default:
	{
		SetLockedLandscape(true);
		break;
	}
	}

	L_ParticleSystem::init(2000);

	if (m_bInitted)
	{
		return true;
	}

	if (!BaseApp::Init()) return false;

	LogMsg("Save path is %s", GetSavePath().c_str());

	if (!IsLargeScreen())
	{
		if (!GetFont(FONT_SMALL)->Load("interface/font_century_gothic.rtfont"))
		{
			return false;
		}

		if (!GetFont(FONT_LARGE)->Load("interface/font_century_gothic_big.rtfont"))
		{
			return false;
		}
	}
	else {
		if (!GetFont(FONT_SMALL)->Load("interface/font_century_gothicx2.rtfont"))
		{
			return false;
		}

		if (!GetFont(FONT_LARGE)->Load("interface/font_century_gothic_bigx2.rtfont"))
		{
			return false;
		}
	}

	bool bFileExisted;
	m_varDB.Load("save.dat", &bFileExisted);

	m_graphicDetail = m_varDB.GetVarWithDefault("graphic_detail", Variant(float(1.0f)))->GetFloat();

	float musicVol = m_varDB.GetVarWithDefault("music_vol", Variant(float(1.0f)))->GetFloat();
	uint32 enableSwearFilter = m_varDB.GetVarWithDefault("swearFilter", Variant(uint32(1)))->GetUINT32();
	uint32 soundIsDisabled = m_varDB.GetVarWithDefault("soundDisabled", Variant(uint32(1)))->GetUINT32();

	//set audio settings from game save file
	GetAudioManager()->SetSoundEnabled(soundIsDisabled == 0);
	GetAudioManager()->SetMusicVol(m_varDB.GetVar("music_vol")->GetFloat());

	//preload audio
	GetAudioManager()->Preload("audio/click.wav");

	if (IsDesktop())
	{
		GetAudioManager()->SetMusicStreaming(false);
	}

	LogMsg("Current locale is %s", GetRegionString().c_str());

	//@SMALLTODO flocks temp.tmp on macos

	return true;
}

void App::Save()
{
	m_varDB.Save("save.dat");
}

void App::Kill()
{
	if (m_pClient != NULL && m_pClient->IsLoggedOn())
	{
		enet_peer_disconnect(m_pClient->GetPeer(), 0);
		m_pClient->Update();
	}

	Save();

	SAFE_DELETE(m_pClient);

	BaseApp::Kill();
	g_pApp = NULL;
}

void CreateMainMenu(VariantList* pVList)
{
	MainMenuCreate(GetEntityRoot()->GetEntityByName("GUI"));
}

void App::Update()
{
	BaseApp::Update();
	GetGamepadManager()->Update();
	m_adManager.Update();
	m_iapManager.Update();

	if (!m_bDidPostInit)
	{
		m_iapManager.Init();
		m_adManager.Init();
		m_adManager.SetupBanner(CL_Vec2f(480.0f, 72.0f));

		AdProviderChartBoost* pProvider = new AdProviderChartBoost();
		pProvider->SetupInfo("50b359b916ba472719000000", "867af97763aedb0a40acf894649f79b90088802b");
		m_adManager.AddProvider(pProvider);
		pProvider->ShowInterstitial();
		m_adManager.GetProviderByType(AD_PROVIDER_CHARTBOOST)->Update();

		m_bDidPostInit = true;

		m_pClient = new ENetClient();
		m_pClient->OneTimeInit();

		m_pGameLogicComponent = new GameLogicComponent();

		Entity* pGameLogicEnt = GetEntityRoot()->AddEntity(new Entity("GameLogic"));

		pGameLogicEnt->AddComponent(m_pGameLogicComponent);
		AddFocusIfNeeded(GetEntityRoot());
		SetupInputGamepadsAndHotkeys();

		Entity* pGUIEnt = GetEntityRoot()->AddEntity(new Entity("GUI"));
		pGUIEnt->AddEntity(new Entity("WorldSpecificGUI"));
		CreateDirectoryRecursively(GetSavePath(), "cache");

		m_cachePath = GetAppCachePath() + "cache/";

		if (m_varDB.GetVar("fullscreen")->GetUINT32() == 0)
		{
			MainMenuCreate(pGUIEnt);
		}
		else
		{
			GetMessageManager()->CallStaticFunction(&CreateMainMenu, 200, 0);
			GetBaseApp()->OnFullscreenToggleRequest();
		}
	}   
}

void App::Draw()
{
	PrepareForGL();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	BaseApp::Draw();
	m_adManager.OnRender();
}

void App::OnEnterBackground()
{
	Save();

	if (m_pClient != NULL && m_pClient->IsLoggedOn())
	{
		m_disconnectionDelayTick = GetSystemTimeTick() + 20000;
	}

	BaseApp::OnEnterBackground();
}
void App::OnScreenSizeChange()
{
	if (GetArcadeComponent() != NULL)
	{
		if (GetPlatformID() != PLATFORM_ID_IOS)
		{
			GetArcadeComponent()->ResetDirectionKeys(true);
		}
	}

	BaseApp::OnScreenSizeChange();
}

void App::GetServerInfo( string &server, uint32 &port )
{
#if defined (_DEBUG) && defined(WIN32)
	server = "localhost";
	port = 8080;

	//server = "www.rtsoft.com";
	//port = 80;
#else

	server = "rtsoft.com";
	port = 80;
#endif
}

int App::GetSpecial()
{
	return m_special; //1 means pirated copy
}

Variant * App::GetVar( const string &keyName )
{
	return GetShared()->GetVar(keyName);
}

std::string App::GetVersionString()
{
	return "V1.44";
}

float App::GetVersion()
{
	return 1.44f;
}