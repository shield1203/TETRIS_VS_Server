#pragma once
class SystemFrame;
class GameUser;
struct GameRoomPacket;

class GameRoomSystem : public SystemFrame
{
public:
	GameRoomSystem();
	~GameRoomSystem();

	virtual void Update(GameUser*);
	virtual void Recv();
	virtual bool CheckPacket();
	virtual void Send();

	static unsigned int WINAPI Communication(void*);
};

