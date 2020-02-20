#include "stdafx.h"
#include "SystemFrame.h"
#include "LobbySystem.h"

LobbySystem::LobbySystem()
{
}

LobbySystem::~LobbySystem()
{
}

void LobbySystem::Update(GameUser* gameUser)
{
	m_gameUser = gameUser;
	m_roomManager = RoomManager::getInstance();

	unsigned int threadID = 0;
	m_gameUser->m_threadHandle = (HANDLE)_beginthreadex(NULL, 0, Communication, (void*)this, 0, (unsigned*)&threadID);
}

void LobbySystem::Recv()
{
	// 방 입장, 연결끊김, 방 만들기
	char msg[MAX_MSG_LEN] = "";

	int check = recv(m_gameUser->m_socket, msg, sizeof(msg), 0);

	if (check > 0)
	{
		msg[sizeof(LobbyPacket)] = '\0';

		m_lobbyPacket = (LobbyPacket*)msg;
	}
	else
	{
		m_gameUser->m_state = USER_STATE::CLOSE_CONNECT;
	}
}

bool LobbySystem::CheckPacket()
{
	if (m_gameUser->m_state == USER_STATE::CLOSE_CONNECT)
	{
		printf("%s:%d와 통신 종료\n", inet_ntoa(m_gameUser->m_cliaddr.sin_addr), ntohs(m_gameUser->m_cliaddr.sin_port));
		return false;
	}

	switch (m_lobbyPacket->userReq)
	{
	case USER_LOBBY::LOBBY_IDLE:
		break;
	case USER_LOBBY::LOBBY_CREATE_ROOM:
		m_gameUser->m_state = USER_STATE::IN_ROOM;

		m_roomManager->CreateGameRoom(m_gameUser->m_gameRoom);
		m_lobbyPacket->b_enterRoom = true;
		m_lobbyPacket->n_roomNum = m_gameUser->m_gameRoom->m_roomNum;

		printf("%s:%d 가 게임룸 %d 생성\n", 
			inet_ntoa(m_gameUser->m_cliaddr.sin_addr), ntohs(m_gameUser->m_cliaddr.sin_port), m_lobbyPacket->n_roomNum);
		break;
	case USER_LOBBY::LOBBY_ENTER_ROOM:
		if (m_roomManager->EnterRoom(m_lobbyPacket->n_roomNum, m_gameUser->m_gameRoom))
		{
			m_gameUser->m_state = USER_STATE::IN_ROOM;

			m_lobbyPacket->b_enterRoom = true;

			printf("%s:%d 가 %d게임룸 입장\n", 
				inet_ntoa(m_gameUser->m_cliaddr.sin_addr), ntohs(m_gameUser->m_cliaddr.sin_port), m_lobbyPacket->n_roomNum);
		}
		else
		{
			m_lobbyPacket->b_enterRoom = false;

			printf("%s:%d 가 %d게임룸 입장실패\n", 
				inet_ntoa(m_gameUser->m_cliaddr.sin_addr), ntohs(m_gameUser->m_cliaddr.sin_port), m_lobbyPacket->n_roomNum);
		}
		break;
	}

	return true;
}

void LobbySystem::Send()
{
	// 방 수, 방 진입여부, 
	m_lobbyPacket->n_roomCount = m_roomManager->m_roomList.size();
	send(m_gameUser->m_socket, (char*)m_lobbyPacket, sizeof(LobbyPacket), 0);
	
	// 방 리스트
	for (auto i : m_roomManager->m_roomList)
	{
		send(m_gameUser->m_socket, (char*)i, sizeof(GameRoom), 0);
	}
	int k = 0; // ....????????
}

unsigned int WINAPI LobbySystem::Communication(void* lobbySystem)
{
	LobbySystem* m_lobbySystem = (LobbySystem*)lobbySystem;

	while (m_lobbySystem->m_gameUser->m_state == USER_STATE::LOBBY)
	{
		m_lobbySystem->Recv();

		if (!m_lobbySystem->CheckPacket())
		{
			break;
		}

		m_lobbySystem->Send();
	}

	m_lobbySystem->m_gameUser->bOn = false;

	return 0;
}