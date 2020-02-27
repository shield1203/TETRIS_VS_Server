#pragma once
class SystemFrame;

class GameRoomSystem : public SystemFrame
{
public:
	GameRoomSystem();
	~GameRoomSystem();
	virtual void CheckPacket(void*);
};

