#include "stdafx.h"
#include "PacketManager.h"
#include "RoomManager.h"
#include "GameUser.h"

PacketManager::PacketManager(int userNum)
{
	m_userNum = userNum;
	m_roomManager = RoomManager::getInstance();

	m_packetData = new PacketData;
	m_lobbyData = new LobbyData;
	m_gameRoomData = new GameRoomData;
	m_playGameData = new PlayGameData;
}

PacketManager::~PacketManager()
{
	SafeDelete(m_packetData);
	SafeDelete(m_lobbyData);
	SafeDelete(m_gameRoomData);
	SafeDelete(m_playGameData);
}

void PacketManager::CopyPacket(PacketData* packetData)
{
	m_packetData->userState = packetData->userState;
	m_packetData->size = packetData->size;
	strcpy(m_packetData->data, packetData->data);
}

void PacketManager::GetData(USER_STATE userState)
{
	switch (userState)
	{
	case USER_STATE::USER_LOBBY:
		GetLobbyData();
		break;
	case USER_STATE::USER_GAME_ROOM:
		GetGameRoomData();
		break;
	case USER_STATE::USER_PLAY_GAME:
		GetPlayGameData(); // ¾ÆÁ÷
		break;
	}
}

void PacketManager::GetLobbyData()
{
	ZeroMemory(m_lobbyData, sizeof(LobbyData));
	memcpy(m_lobbyData, m_packetData->data, sizeof(LobbyData));
}

void PacketManager::GetGameRoomData()
{
	ZeroMemory(m_gameRoomData, sizeof(GameRoomData));
	memcpy(m_gameRoomData, m_packetData->data, sizeof(GameRoomData));
}

void PacketManager::GetPlayGameData()
{
	//ZeroMemory(m_gameRoomData, sizeof(GameRoomData));
}

void PacketManager::SetPacket(USER_STATE userState)
{
	switch (userState)
	{
	case USER_STATE::USER_LOBBY: 
		SetLobbyData();
		break;
	case USER_STATE::USER_GAME_ROOM:
		SetGameRoomData();
		break;
	case USER_STATE::USER_PLAY_GAME:
		SetPlayGameData();
		break;
	}
}

void PacketManager::SetLobbyData()
{
	m_packetData->size = 0;

	memcpy(m_packetData->data, m_lobbyData, sizeof(LobbyData));
	m_packetData += static_cast<unsigned short>(sizeof(LobbyData));

	for (auto roomList : m_roomManager->m_roomList)
	{
		memcpy(m_packetData->data + m_packetData->size, roomList, sizeof(GameRoom));
		m_packetData->size += static_cast < unsigned short>(sizeof(GameRoom));
	}
}

void PacketManager::SetGameRoomData()
{
	m_packetData->size = 0;

	for (auto roomList : m_roomManager->m_roomList)
	{
		if (m_gameRoomData->roomNum == roomList->roomNum)
		{
			for (auto userList : roomList->gameUserList)
			{
				if (m_userNum != userList->m_userNum)
				{
					memcpy(m_packetData->data, userList->m_gameRoomData, sizeof(GameRoomData));
					m_packetData += static_cast <unsigned short>(sizeof(GameRoomData));
					break;
				}
			}
		}

		if (m_packetData->size != 0)
		{
			break;
		}
	}

	if (m_packetData->size == 0)
	{
		GameRoomData* gameRoomData = new GameRoomData;
		gameRoomData->userReq = USER_ROOM::ROOM_IDLE;
		gameRoomData->bOn = false;

		memcpy(m_packetData->data, gameRoomData, sizeof(GameRoomData));
		m_packetData += static_cast <unsigned short>(sizeof(GameRoomData));

		SafeDelete(gameRoomData);
	}
}

void PacketManager::SetPlayGameData()
{

}