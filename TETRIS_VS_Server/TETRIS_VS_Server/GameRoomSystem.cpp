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

void GameRoomSystem::Update(GameUser* gameUser)
{
	m_gameUser = gameUser;
	m_roomManager = RoomManager::getInstance();

	unsigned int threadID = 0;
	m_gameUser->m_threadHandle = (HANDLE)_beginthreadex(NULL, 0, Communication, (void*)this, 0, (unsigned*)&threadID);
}

void GameRoomSystem::Recv()
{
	char msg[MAX_MSG_LEN] = "";
	int check = recv(m_gameUser->m_socket, msg, sizeof(msg), 0);

	if (check > 0)
	{
		msg[sizeof(GameRoomPacket)] = '\0';
		GameRoomPacket* gameRoomPacket = (GameRoomPacket*)msg;
		m_gameUser->m_packetManager->SetGameRoomPacket(gameRoomPacket);
	}
	else
	{
		m_gameUser->m_state = USER_STATE::CLOSE_CONNECT;
	}
}

bool GameRoomSystem::CheckPacket()
{
	if (m_gameUser->m_state == USER_STATE::CLOSE_CONNECT)
	{
		m_roomManager->ExitRoom(m_gameUser->m_packetManager->m_lobbyPacket->n_roomNum, m_gameUser);

		printf("%s:%d가 %d게임룸 퇴장\n",
			inet_ntoa(m_gameUser->m_cliaddr.sin_addr), ntohs(m_gameUser->m_cliaddr.sin_port), m_gameUser->m_packetManager->m_lobbyPacket->n_roomNum);

		printf("%s:%d와 통신 종료\n", inet_ntoa(m_gameUser->m_cliaddr.sin_addr), ntohs(m_gameUser->m_cliaddr.sin_port));
		return false;
	}

	switch (m_gameUser->m_packetManager->m_gameRoomPacket->userReq)
	{
	case USER_ROOM::ROOM_BACK_LOBBY:
		m_gameUser->m_state = USER_STATE::LOBBY;

		m_roomManager->ExitRoom(m_gameUser->m_packetManager->m_lobbyPacket->n_roomNum, m_gameUser);

		printf("%s:%d가 %d게임룸 퇴장\n",
			inet_ntoa(m_gameUser->m_cliaddr.sin_addr), ntohs(m_gameUser->m_cliaddr.sin_port), m_gameUser->m_packetManager->m_lobbyPacket->n_roomNum);
		break;
	case USER_ROOM::ROOM_GAME_START:
		// 게임시작 가능한지 체크하고 가능하면 패킷 보내고 스텝 변경
		break;
	}

	return true;
}

void GameRoomSystem::Send()
{
	// 상대 패킷
	bool bSend = false;
	for (auto roomList : m_roomManager->m_roomList)
	{
		if (roomList->roomNum == m_gameUser->m_packetManager->m_lobbyPacket->n_roomNum)
		{
			// 혼자 있을 때
			if (roomList->gameUserList.size() == 1)
			{
				GameRoomPacket* gameRoomPacket = new GameRoomPacket;
				gameRoomPacket->bOn = false;

				bSend = true;
				send(m_gameUser->m_socket, (char*)gameRoomPacket, sizeof(GameRoomPacket), 0);

				SafeDelete(gameRoomPacket);
			}
			else
			{
				for (auto userList : roomList->gameUserList)
				{
					if (userList->m_userNum != m_gameUser->m_userNum)
					{
						bSend = true;
						send(m_gameUser->m_socket, (char*)userList->m_packetManager->m_gameRoomPacket, sizeof(GameRoomPacket), 0);
					}
				}
			}
			break;
		}
	}

	if (!bSend)
	{
		send(m_gameUser->m_socket, (char*)m_gameUser->m_packetManager->m_gameRoomPacket, sizeof(GameRoom_Lobby), 0);
	}

	int k = 0;
}

unsigned int WINAPI GameRoomSystem::Communication(void* gameRoomSystem)
{
	GameRoomSystem* m_gameRoomSystem = (GameRoomSystem*)gameRoomSystem;

	while (m_gameRoomSystem->m_gameUser->m_state == USER_STATE::IN_ROOM)
	{
		m_gameRoomSystem->m_mutex.lock();

		m_gameRoomSystem->Recv();

		if (!m_gameRoomSystem->CheckPacket())
		{
			break;
		}

		m_gameRoomSystem->Send();

		m_gameRoomSystem->m_mutex.unlock();
	}

	m_gameRoomSystem->m_gameUser->bOn = false;

	return 0;
}