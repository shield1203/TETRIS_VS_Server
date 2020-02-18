#include "stdafx.h"
#include "RoomManager.h"

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
}

void RoomManager::CreateGameRoom(GameRoom* newGameRoom)
{
	// ����ó�� ��
	// mutex
	 newGameRoom = new GameRoom;

	// ���ȣ
	for (int roomNum = 1; ; roomNum++)
	{
		bool bRoom = true;
		for (auto i : m_roomList)
		{
			if (i->m_roomNum == roomNum)
			{
				bRoom = false;
				break;
			}
		}

		if (bRoom)
		{
			newGameRoom->m_roomNum = roomNum;
			break;
		}
	}

	// �� �ο�
	newGameRoom->m_userCount++;
}

bool RoomManager::EnterRoom(int roomNum, GameRoom* gameRoom)
{
	for (auto i : m_roomList)
	{
		if (i->m_roomNum == roomNum)
		{
			if (i->m_userCount < 2)
			{
				i->m_userCount++;
				gameRoom = i;
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
