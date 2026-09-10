#ifndef NetControllerBase_h__
#define NetControllerBase_h__
#include "PlatformSetup.h"

class NetAvatar;
class NetControllerBase
{
public:
	virtual ~NetControllerBase() = default;

	// derivable events
	virtual bool OnPunch();
	virtual void ResetKeys();
	virtual void StopAllKeys();

	// base functions
	void SetNetAvatar(NetAvatar* pAvatar) { m_pAvatar = pAvatar; }

private:
	NetAvatar* m_pAvatar = NULL;
};

#endif // NetControllerBase_h__