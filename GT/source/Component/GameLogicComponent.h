#ifndef GameLogicComponent_h__
#define GameLogicComponent_h__
#include "Entity/Component.h"
#include "TextManager.h"

class GameLogicComponent : public EntityComponent
{
public:
	GameLogicComponent();
	~GameLogicComponent();

	void KillWorld(bool bJustChangingMap);

	virtual void OnAdd(Entity* pEnt);
	virtual void OnRemove();

private:
	void OnRender(VariantList* pVList);
	void OnUpdate(VariantList* pVList);

	CL_Vec2f* m_pPos2d;
	CL_Vec2f* m_pScale2d;
	CL_Vec2f* m_pSize2d;

	TextManager m_textManager{};
};

#endif // GameLogicComponent_h__