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

	GameLogicComponent* m_pGameLogicComponent = NULL;
	AdManager m_adManager{};
	ENetClient* m_pClient = NULL;
	bool m_bDidPostInit = false;
	VariantDB m_varDB{};
	int m_special = 0; //i don't know if this exists in gt but i'll keep it for now
	IAPManager m_IAPManager{};
	float m_graphicDetail = 1.0f;
	std::string m_cachePath = "";
	uint32 m_disconnectionDelayTick = 0;
};
 

extern App g_App;

App * GetApp();
const char * GetAppName();
const char * GetBundleName();
const char * GetBundlePrefix();
