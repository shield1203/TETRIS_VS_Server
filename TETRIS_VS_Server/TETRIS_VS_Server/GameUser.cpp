#include "stdafx.h"
#include "GameUser.h"

#include "RoomManager.h"
#include "SystemFrame.h"
#include "LobbySystem.h"
//#include "RoomSystem.h"
//#include "Pl"

GameUser::GameUser(SOCKET socket, SOCKADDR_IN cliaddr)
{
	this->m_socket = socket;
	this->m_cliaddr = cliaddr;

	m_state = USER_STATE::LOBBY;
}

GameUser::~GameUser()
{
	closesocket(m_socket);

	if (m_systemFrame != nullptr)
	{
		SafeDelete(m_systemFrame);
	}

	if (m_threadHandle != nullptr)
	{
		CloseHandle(m_threadHandle);
	}
}

void GameUser::Update()
{
	if (!bOn)
	{
		if (m_systemFrame != nullptr)
		{
			SafeDelete(m_systemFrame);
		}

		switch (m_state)
		{
		case  USER_STATE::LOBBY:
			m_systemFrame = new LobbySystem();
			break;
		case USER_STATE::IN_ROOM:
			//m_systemFrame = new RoomSystem();
			break;
		case USER_STATE::PLAY_GAME:
			//m_systemFrame = new PlayGameSystem();
			break;
		}

		StartThread();
	}
}

void GameUser::StartThread()
{
	if (m_threadHandle != nullptr)
	{
		CloseHandle(m_threadHandle);
	}

	m_systemFrame->Update(*this);
}

USER_STATE GameUser::GetUserState()
{
	return m_state;
}