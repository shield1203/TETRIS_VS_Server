#pragma once
#include "stdafx.h"
#include "GameUser.h"
#include "RoomManager.h"

class SystemFrame
{
protected:
	GameUser* m_gameUser = nullptr;
	RoomManager* m_roomManager = nullptr;
public:
	virtual void Update(GameUser&) { };
	virtual void Recv() { };
	virtual bool CheckPacket() { return false; };
	virtual void Send() { };
};