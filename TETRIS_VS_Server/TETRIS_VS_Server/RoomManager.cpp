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

	// ¹æ¹øÈ£
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
	for (auto room : m_roomList)
	{
		for (auto user = room->gameUserList.begin(); user != room->gameUserList.end();)
		{
			if ((*user)->m_userNum == userNum)
			{
				user = room->gameUserList.erase(user++);
				printf("%dÀ¯Àú°¡ °ÔÀÓ·ë %d ÅðÀå\n", userNum, room->roomNum);
			}
		}
		CheckRoom(room);
	}
}

void RoomManager::CheckRoom(GameRoom* gameRoom)
{
	// ºó ¹æ¾ø¾Ö±â
	if (gameRoom->gameUserList.empty())
	{
		for (auto room = m_roomList.begin(); room != m_roomList.end();)
		{
			if ((*room)->roomNum == gameRoom->roomNum)
			{
				room = m_roomList.erase(room++);
				break;
			}
		}
	}
}