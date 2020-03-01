#pragma once
class SystemFrame;
class PacketManager;

class GameResultSystem : public SystemFrame
{
public:
	GameResultSystem();
	~GameResultSystem();
	virtual void CheckPacket(void*);
};

