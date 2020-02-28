#include "stdafx.h"
#include "SystemFrame.h"
#include "GameRoomSystem.h"

#include "GameUser.h"
#include "RoomManager.h"
#include "PacketManager.h"

GameRoomSystem::GameRoomSystem()
{
}

GameRoomSystem::~GameRoomSystem()
{
}

void GameRoomSystem::CheckPacket(void* packetManager)
{
	PacketManager* pPacketManager = (PacketManager*)packetManager;

	switch (pPacketManager->m_gameRoomData->userReq)
	{
	case USER_ROOM::ROOM_GAME_START:
		GameStart(pPacketManager);
		break;
	}
}

void GameRoomSystem::GameStart(PacketManager* packetManager)
{
	RoomManager::getInstance()->GameStartRoom(packetManager->m_gameRoomData->roomNum);
	printf("[%d]게임룸 게임시작\n", packetManager->m_gameRoomData->roomNum);
}