#ifndef NetControllerLocal_h__
#define NetControllerLocal_h__
#include "NetControllerBase.h"

class NetControllerLocal :
	public NetControllerBase	
{
public:
	// base events
	virtual bool OnPunch();
	virtual void ResetKeys();
	virtual void StopAllKeys();

	// our methods
	void OnUpdate();
	void SetPunch(bool bKeyDown);
	void StopPunch();
	void OnArcadeInput(int vKey, bool bKeyDown);
};

#endif // NetControllerLocal_h__