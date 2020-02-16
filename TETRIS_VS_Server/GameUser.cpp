#include "stdafx.h"
#include "GameUser.h"

#include "SystemFrame.h"
#include "LobbySystem.h"
//#include "RoomSystem.h"
//#include "Pl"

GameUser::GameUser(SOCKET socket, SOCKADDR_IN cliaddr)
{
	this->m_socket = socket;
	this->m_cliaddr = cliaddr;

	m_state = LOBBY;
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
		case LOBBY:
			m_systemFrame = new LobbySystem();
			break;
		case IN_ROOM:
			//m_systemFrame = new RoomSystem();
			break;
		case PLAY_GAME:
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