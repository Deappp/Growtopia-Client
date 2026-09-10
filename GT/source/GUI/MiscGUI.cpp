#include "PlatformPrecomp.h"
#include "MiscGUI.h"

Entity* AddBMPRectAroundEntity(Entity* pEnt, uint32 middleColor, uint32 borderColor, float borderSizePixels, bool button, float fontScale, eFont fontID)
{
	CL_Vec2f pos2d = GetPos2DEntity(pEnt);
	CL_Vec2f size2d = GetSize2DEntity(pEnt);

	float lineHeight = GetBaseApp()->GetFont(fontID)->GetLineHeight(fontScale);

	if (size2d.y <= lineHeight)
	{
		lineHeight = GetBaseApp()->GetFont(fontID)->GetLineHeight(fontScale);
		size2d.y = lineHeight;
	}

	CL_Vec2f vAlignmentOffset = GetAlignmentOffset(size2d, GetAlignmentEntity(pEnt));

	size2d.x += borderSizePixels * 2;
	size2d.y += (borderSizePixels * 0.6f) * 3.f;

	pos2d.x -= vAlignmentOffset.x + borderSizePixels;
	pos2d.y -= vAlignmentOffset.y + (borderSizePixels * 0.6f);

	Entity* pRectEnt = CreateOverlayRectEntity(pEnt->GetParent(), pos2d, size2d, middleColor);

	if (button)
	{
		pRectEnt->GetComponentByName("RectRender")->GetVar("bmpBorderFileName")->Set("game/gui_box_upwhite.rttex");
	}
	else
	{
		pRectEnt->GetComponentByName("RectRender")->GetVar("bmpBorderFileName")->Set("game/gui_box_white.rttex");
	}

	pRectEnt->GetComponentByName("RectRender")->GetVar("borderColor")->Set(borderColor);

	pEnt->GetParent()->MoveEntityToBottomByAddress(pRectEnt);

	return pRectEnt;
}