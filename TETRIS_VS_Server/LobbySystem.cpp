#include "stdafx.h"
#include "GameUser.h"
#include "SystemFrame.h"
#include "LobbySystem.h"

LobbySystem::LobbySystem()
{
}

LobbySystem::~LobbySystem()
{
}

void LobbySystem::Update(GameUser& gameUser)
{
	m_gameUser = &gameUser;

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
		m_gameUser->m_state = CLOSE_CONNECT;
		printf("%s:%d와 통신 종료\n", inet_ntoa(m_gameUser->m_cliaddr.sin_addr), ntohs(m_gameUser->m_cliaddr.sin_port));
	}
}

void LobbySystem::Send()
{
	// 방 목록 보냄

	// 방개수
	// 방정보
}

unsigned int WINAPI LobbySystem::Communication(void* lobbySystem)
{
	LobbySystem* m_lobbySystem = (LobbySystem*)lobbySystem;

	m_lobbySystem->m_gameUser->bOn = true;

	while (true)
	{
		m_lobbySystem->Recv();

		if (m_lobbySystem->m_gameUser->m_state == CLOSE_CONNECT)
		{
			break;
		}

		m_lobbySystem->Send();

		if (m_lobbySystem->m_gameUser->m_state == IN_ROOM)
		{
			break;
		}
	}

	m_lobbySystem->m_gameUser->bOn = false;
}