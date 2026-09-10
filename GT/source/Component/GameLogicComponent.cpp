#include "PlatformPrecomp.h"
#include "GameLogicComponent.h"

GameLogicComponent::GameLogicComponent()
{
	SetName("GameLogic");
}

GameLogicComponent::~GameLogicComponent()
{
	KillWorld(true);
}

void GameLogicComponent::OnAdd(Entity* pEnt)
{
	EntityComponent::OnAdd(pEnt);
	m_textManager.Init();

	m_pPos2d = &GetParent()->GetVar("pos2d")->GetVector2();
	m_pScale2d = &GetParent()->GetVarWithDefault("scale2d", Variant(1.0f, 1.0f))->GetVector2();
	m_pSize2d = &GetParent()->GetVarWithDefault("size2d", Variant(GetScreenSize()))->GetVector2();

	GetParent()->GetFunction("OnRender")->sig_function.connect(1, boost::bind(&GameLogicComponent::OnRender, this, _1));
	GetParent()->GetFunction("OnUpdate")->sig_function.connect(1, boost::bind(&GameLogicComponent::OnUpdate, this, _1));
}

void GameLogicComponent::OnRemove()
{
	EntityComponent::OnRemove();
}

//@STUB MEDIUMTODO
void GameLogicComponent::KillWorld(bool bJustChangingMap)
{
}

//@STUB MEDIUMTODO
void GameLogicComponent::OnRender(VariantList* pVList)
{
}

//@STUB MEDIUMTODO
void GameLogicComponent::OnUpdate(VariantList* pVList)
{
}