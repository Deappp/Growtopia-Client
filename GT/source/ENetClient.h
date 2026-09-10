#ifndef ENetClient_h__
#define ENetClient_h__
#include "enet/enet.h"

class ENetClient
{
public:
	bool IsLoggedOn() const { return m_bIsLoggedOn; }
	void Update();
	void OneTimeInit();

	ENetPeer* GetPeer() { return peer; }

private:
	ENetPeer* peer = NULL;
	bool m_bIsLoggedOn = false;
};

#endif // ENetClient_h__