#pragma once
class SystemFrame;

class LobbySystem : public SystemFrame
{
public:
	LobbySystem();
	~LobbySystem();

	virtual void CheckPacket(void*);

	static unsigned int WINAPI Communication(void*);
};