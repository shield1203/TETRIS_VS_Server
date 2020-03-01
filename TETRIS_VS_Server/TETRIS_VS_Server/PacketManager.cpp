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
	ClearBlockList();
}

void PacketManager::ClearBlockList()
{
	for (auto i : m_blockList)
	{
		SafeDelete(i);
	}
	m_blockList.clear();
}

void PacketManager::CopyPacket(PacketData* packetData)
{
	memcpy(m_packetData, packetData, sizeof(PacketData));
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
	ZeroMemory(m_playGameData, sizeof(PlayGameData));
	memcpy(m_playGameData, m_packetData->data, sizeof(PlayGameData));

	int size = sizeof(PlayGameData);

	ClearBlockList();
	while (size < m_packetData->size)
	{
		PlayGameData_Block* block = new PlayGameData_Block;
		memcpy(block, m_packetData->data + size, sizeof(PlayGameData_Block));
		m_blockList.push_back(block);

		size += sizeof(PlayGameData_Block);
	}
}

void PacketManager::GetPlayResultData()
{
	ZeroMemory(m_playGameData, sizeof(PlayGameData));
	memcpy(m_playGameData, m_packetData->data, sizeof(PlayGameData));
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
	m_packetData->size += static_cast<unsigned short>(sizeof(LobbyData));

	LobbyData_GameRoom* room = new LobbyData_GameRoom;
	for (auto roomList : m_roomManager->m_roomList)
	{
		room->roomNum = roomList->roomNum;
		room->userCount = roomList->gameUserList.size();
		memcpy(m_packetData->data + m_packetData->size, room, sizeof(LobbyData_GameRoom));
		m_packetData->size += static_cast < unsigned short>(sizeof(LobbyData_GameRoom));
	}
	SafeDelete(room);
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
					m_packetData->size += static_cast <unsigned short>(sizeof(GameRoomData));
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
		m_packetData->size += static_cast <unsigned short>(sizeof(GameRoomData));

		SafeDelete(gameRoomData);
	}
}

void PacketManager::SetPlayGameData()
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
					memcpy(m_packetData->data, userList->m_playGameData, sizeof(PlayGameData));
					m_packetData->size += static_cast <unsigned short>(sizeof(PlayGameData));

					for (auto block : userList->m_blockList)
					{
						memcpy(m_packetData->data + m_packetData->size, block, sizeof(PlayGameData_Block));
						m_packetData->size += static_cast <unsigned short>(sizeof(PlayGameData_Block));
					}
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
		PlayGameData* playGameData = new PlayGameData;
		playGameData->userReq = USER_PLAY::PLAY_WIN;

		memcpy(m_packetData->data, playGameData, sizeof(PlayGameData));
		m_packetData->size += static_cast <unsigned short>(sizeof(PlayGameData));

		SafeDelete(playGameData);
	}
}

void PacketManager::SetPlayResultData()
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
					memcpy(m_packetData->data, userList->m_playGameData, sizeof(PlayGameData));
					m_packetData->size += static_cast <unsigned short>(sizeof(PlayGameData));
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
		PlayGameData* playGameData = new PlayGameData;
		playGameData->userReq = USER_PLAY::PLAY_IDLE;

		memcpy(m_packetData->data, playGameData, sizeof(PlayGameData));
		m_packetData->size += static_cast <unsigned short>(sizeof(PlayGameData));

		SafeDelete(playGameData);
	}
}