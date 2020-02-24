#pragma once
class SystemFrame;
class GameUser;

class LobbySystem : public SystemFrame
{
public:
	LobbySystem();
	~LobbySystem();

	virtual void Update(GameUser*);
	virtual void Recv();
	virtual bool CheckPacket();
	virtual void Send();

	void SetPacket();

	static unsigned int WINAPI Communication(void*);
};