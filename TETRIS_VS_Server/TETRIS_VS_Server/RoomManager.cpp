#include "stdafx.h"
#include "RoomManager.h"
#include "PacketManager.h"

RoomManager* RoomManager::Inst = nullptr;

RoomManager* RoomManager::getInstance()
{
	if (Inst == nullptr) {
		Inst = new RoomManager();
	}

	return Inst;
}

RoomManager::RoomManager()
{
}

RoomManager::~RoomManager()
{
	for (auto room : m_roomList)
	{
		room->gameUserList.clear();

		SafeDelete(room);
	}
	m_roomList.clear();

	Inst = nullptr;
}

void RoomManager::CreateGameRoom(PacketManager* ownerUser)
{
	GameRoom* newGameRoom = new GameRoom;
	newGameRoom->gameUserList.push_back(ownerUser);

	// ���ȣ
	for (int roomNum = 1; ; roomNum++)
	{
		bool bRoom = true;
		for (auto i : m_roomList)
		{
			if (i->roomNum == roomNum)
			{
				bRoom = false;
				break;
			}
		}

		if (bRoom)
		{
			newGameRoom->roomNum = roomNum;

			ownerUser->m_lobbyData->roomNum = roomNum;
			ownerUser->m_gameRoomData->roomNum = roomNum;
			break;
		}
	}

	m_roomList.push_back(newGameRoom);
}

bool RoomManager::EnterRoom(int roomNum, PacketManager* gameUser)
{
	for (auto i : m_roomList)
	{
		if (i->roomNum == roomNum)
		{
			if (i->gameUserList.size() < FULL_USER_COUNT)
			{
				gameUser->m_lobbyData->bEnterRoom = true;
				i->gameUserList.push_back(gameUser);
				return true;
			}
			else
			{
				gameUser->m_lobbyData->bEnterRoom = false;
				return false;
			}
		}
	}

	gameUser->m_lobbyData->bEnterRoom = false;
	return false;
}

void RoomManager::ExitRoom(int userNum)
{
	for (auto room = m_roomList.begin(); room != m_roomList.end();)
	{
		for (auto user = (*room)->gameUserList.begin(); user != (*room)->gameUserList.end();)
		{
			if ((*user)->m_userNum == userNum)
			{
				user = (*room)->gameUserList.erase(user++);
				printf("[%d]������ [%d]���ӷ� ����\n", userNum, (*room)->roomNum);
				break;
			}
			else
			{
				user++;
			}
		}
		
		if ((*room)->gameUserList.empty())
		{
			printf("[%d]���ӷ� ����\n", (*room)->roomNum);
			room = m_roomList.erase(room++);
			break;
		}

		room++;
	}
}

void RoomManager::CheckRoom(GameRoom* gameRoom)
{
	// �� ����ֱ�
	if (gameRoom->gameUserList.empty())
	{
		for (auto room = m_roomList.begin(); room != m_roomList.end();)
		{
			if ((*room)->roomNum == gameRoom->roomNum)
			{
				printf("[%d]���ӷ� ����\n", gameRoom->roomNum);
				room = m_roomList.erase(room++);
				break;
			}
		}
	}
}

void RoomManager::GameStartRoom(int roomNum)
{
	for (auto room : m_roomList)
	{
		if (room->roomNum == roomNum)
		{
			printf("[%d]���ӷ� ���ӽ���\n", room->roomNum);

			for (auto user = room->gameUserList.begin(); user != room->gameUserList.end();)
			{
				(*user)->m_gameRoomData->userReq = USER_ROOM::ROOM_GAME_START;
				user++;
			}
			break;
		}
	}
}