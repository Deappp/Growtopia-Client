/*
 *  App.h
 *  Created by Seth Robinson on 3/6/09.
 *  For license info, check the license.txt file that should have come with this.
 *
 */

#pragma once

#include "BaseApp.h"
#include "ENetClient.h"
#include "Manager/AdManager.h"
#include "Manager/IAPManager.h"
#include "Component/GameLogicComponent.h"

class App: public BaseApp
{
public:
	
	App();
	virtual ~App();
	
	virtual bool Init();
	virtual void Kill();
	virtual void Draw();
	virtual void OnScreenSizeChange();
	virtual void Update();
	virtual void OnEnterBackground();

	string GetVersionString();
	float GetVersion();
	void GetServerInfo(string &server, uint32 &port);
	VariantDB * GetShared() {return &m_varDB;}
	Variant * GetVar(const string &keyName );
	Variant * GetVarWithDefault(const string &varName, const Variant &var) {return m_varDB.GetVarWithDefault(varName, var);}
	int GetSpecial();
	void OnExitApp(VariantList *pVarList);

private:

	void Save();

	bool m_bDidPostInit;
	VariantDB m_varDB; //holds all data we want to save/load
	int m_special;
	ENetClient* m_pClient = NULL;
	GameLogicComponent* m_pGameLogicComponent = NULL;
	AdManager m_adManager{};
	IAPManager m_iapManager{};
	float m_graphicDetail = 1.0f;
	std::string m_cachePath = "";
	uint32 m_disconnectionDelayTick = 0;
};
 

extern App g_App;

App * GetApp();
const char * GetAppName();
const char * GetBundleName();
const char * GetBundlePrefix();
