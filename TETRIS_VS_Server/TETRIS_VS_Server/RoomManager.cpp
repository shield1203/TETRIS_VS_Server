#include "stdafx.h"
#include "RoomManager.h"
#include "GameUser.h"
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
		for (auto userList : room->gameUserList)
		{
			SafeDelete(userList);
		}
		room->gameUserList.clear();

		SafeDelete(room);
	}
	m_roomList.clear();

	Inst = nullptr;
}

void RoomManager::CreateGameRoom(GameUser* ownerUser)
{
	// 抗寇贸府 遏
	// mutex
	GameRoom* newGameRoom = new GameRoom;
	newGameRoom->gameUserList.push_back(ownerUser);

	// 规锅龋
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
			ownerUser->SetRoomNum(roomNum);
			newGameRoom->roomNum = roomNum;
			break;
		}
	}

	m_roomList.push_back(newGameRoom);
}

bool RoomManager::EnterRoom(int roomNum, GameUser* gameUser)
{
	for (auto i : m_roomList)
	{
		if (i->roomNum == roomNum)
		{
			if (i->gameUserList.size() < FULL_USER_COUNT)
			{
				gameUser->SetRoomNum(roomNum);
				i->gameUserList.push_back(gameUser);

				return true;
			}
			else
			{
				return false;
			}
		}
	}

	return false;
}

void RoomManager::ExitRoom(int roomNum, GameUser* gameUser)
{
	for (auto room : m_roomList)
	{
		if (room->roomNum == roomNum)
		{
			for (auto user = room->gameUserList.begin(); user != room->gameUserList.end();)
			{
				if ((*user)->GetUserNum() == gameUser->GetUserNum())
				{
					user = room->gameUserList.erase(user++);
				}
			}
			CheckRoom(room);

			break;
		}
	}
}

void RoomManager::CheckRoom(GameRoom* gameRoom)
{
	// 后 规
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