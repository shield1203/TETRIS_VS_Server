#include "stdafx.h"
#include "SystemFrame.h"
#include "LobbySystem.h"

#include "GameUser.h"
#include "RoomManager.h"
#include "PacketManager.h"

LobbySystem::LobbySystem()
{
}

LobbySystem::~LobbySystem()
{
}

void LobbySystem::CheckPacket(void* packetManager)
{
	PacketManager* pPacketManager = (PacketManager*)packetManager;

	switch (pPacketManager->m_lobbyData->userReq)
	{
	case USER_LOBBY::LOBBY_IDLE:
		break;
	case USER_LOBBY::LOBBY_CREATE_ROOM:
		LobbyCreateRoom(pPacketManager);
		break;
	case USER_LOBBY::LOBBY_ENTER_ROOM:
		LobbyEnterRoom(pPacketManager->m_lobbyData->roomNum, pPacketManager);
		break;
	}
}

void LobbySystem::LobbyCreateRoom(PacketManager* packetManager)
{
	RoomManager::getInstance()->CreateGameRoom(packetManager);
	printf("%d������ ���ӷ� %d ����\n", packetManager->m_userNum, packetManager->m_lobbyData->roomNum);
}

void LobbySystem::LobbyEnterRoom(int roomNum, PacketManager* packetManager)
{
	bool bEnter = RoomManager::getInstance()->EnterRoom(roomNum, packetManager);
	
	if (bEnter)
	{
		printf("%d������ ���ӷ� %d ���强��\n", packetManager->m_userNum, packetManager->m_lobbyData->roomNum);
	}
	else
	{
		printf("%d������ ���ӷ� %d �������\n", packetManager->m_userNum, packetManager->m_lobbyData->roomNum);
	}
}