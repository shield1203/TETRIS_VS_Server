#pragma once
class SystemFrame;
class PacketManager;

class VersusPlaySystem : public SystemFrame
{
public:
	VersusPlaySystem();
	~VersusPlaySystem();
	virtual void CheckPacket(void*);
	void CheckGameOver(PacketManager*);
};

