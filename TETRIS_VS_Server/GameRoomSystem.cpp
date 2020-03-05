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
	case USER_ROOM::ROOM_BACK_LOBBY:
		BackLobby(pPacketManager->m_userNum);
		break;
	case USER_ROOM::ROOM_GAME_START:
		pPacketManager->m_gameRoomData->bReady = false;
		GameStart(pPacketManager);
		break;
	}
}

void GameRoomSystem::GameStart(PacketManager* packetManager)
{
	RoomManager::getInstance()->GameStartRoom(packetManager->m_gameRoomData->roomNum);
}

void GameRoomSystem::BackLobby(int userNum)
{
	m_mutex.lock();
	RoomManager::getInstance()->ExitRoom(userNum);
	m_mutex.unlock();
}